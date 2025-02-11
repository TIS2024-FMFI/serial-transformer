#include <string>
#include <Arduino.h>



int speeds[] = {-4000, -2000, -300, 37, 38, 39, 40, 41, 42, 43, 44, 80, 300, 2000, 4000, 0};
class AutomaticSerialTransformer {
  public:
    SerialLogger* logger;
    double ra_ratio = 1.0;
    double dec_ratio = 1.0;
    int original_speed_ra = 15;
    int original_speed_dec = 15;
    int original_speed_ra_steps = 0;
    int original_speed_dec_steps = 0;
    int transformed_speed_ra = 15;
    int transformed_speed_dec = 15;
    int transformed_speed_ra_steps = 0;
    int transformed_speed_dec_steps = 0;
    int speed_changes = 0;

    double ra_original_distance = 0.0;
    double dec_original_distance = 0.0;

    double ra_transformed_distance = 0.0;
    double dec_transformed_distance = 0.0;
    AutomaticSerialTransformer(double ra, double dec, SerialLogger* ptr_logger) {
      ra_ratio = ra;
      dec_ratio = dec;
      logger = ptr_logger;
    }

    void reset(){
      original_speed_ra = 15;
      original_speed_dec = 15;
      original_speed_ra_steps = 0;
      original_speed_dec_steps = 0;
      transformed_speed_ra = 15;
      transformed_speed_dec = 15;
      transformed_speed_ra_steps = 0;
      transformed_speed_dec_steps = 0;
      speed_changes = 0;

      ra_original_distance = 0.0;
      dec_original_distance = 0.0;

      ra_transformed_distance = 0.0;
      dec_transformed_distance = 0.0;
    }

    void setRaRatio(double ra) {
        ra_ratio = ra;
        logger->log("RA ratio set to " + String(ra));
    }
    void setDecRatio(double dec) {
        dec_ratio = dec;
        logger->log("DEC ratio set to " + String(dec));
    }
    void readSerial(){
      // Read serial data
      if (Serial.available() > 0) {
          // Read the incoming byte:
          input = Serial.read();
          
          transformSerial();
          Serialprintln(input);
      }
    }
  private:
    //state
    //0: start of message
    //1: module address
    //2: data length
    //3: data
    //4: checksum
    //5: end of message
    int state = 5;
    int last_ra_transformed = 0;
    int last_dec_transformed = 0;

    String module_address = "";
    String data_length = "";
    String data = "";
    String checksum = "";
    String end_of_message = "";

    char input;
    char buffer[100];

    void setSpeedRa(int speed, int new_speed){
      original_speed_ra = speed;
      original_speed_ra_steps = speeds[speed];
      transformed_speed_ra = new_speed;
      transformed_speed_ra_steps = speeds[new_speed];
      logger->log("Speed RA set from " + String(original_speed_ra) + " to " + String(transformed_speed_ra));
      speed_changes++;
      //now time
      time_t t = time(NULL);
      struct tm tm = *localtime(&t);
      int new_last_ra_transformed = tm.tm_hour*3600 + tm.tm_min*60 + tm.tm_sec;
      int time_diff = new_last_ra_transformed - last_ra_transformed;
      ra_original_distance += original_speed_ra_steps*time_diff;
      ra_transformed_distance += transformed_speed_ra_steps*time_diff;
      last_ra_transformed = new_last_ra_transformed;
    }

    void setSpeedDec(int speed, int new_speed){
      original_speed_dec = speed;
      original_speed_dec_steps = speeds[speed];
      transformed_speed_dec = new_speed;
      transformed_speed_dec_steps = speeds[new_speed];
      logger->log("Speed DEC set from " + String(original_speed_dec) + " to " + String(transformed_speed_dec));
      speed_changes++;
      //now time
      time_t t = time(NULL);
      struct tm tm = *localtime(&t);
      int new_last_dec_transformed = tm.tm_hour*3600 + tm.tm_min*60 + tm.tm_sec;
      int time_diff = new_last_dec_transformed - last_dec_transformed;
      dec_original_distance += original_speed_dec_steps*time_diff;
      dec_transformed_distance += transformed_speed_dec_steps*time_diff;
      last_dec_transformed = new_last_dec_transformed;
    }



  void transformSerial(){
    if (state == 5 && input=='U'){
      state=0;
      writeSerial(input);
      Serial2.println("State chnaged to 0");
      return;
    }
    if (state==0){
      module_address=String(input);
      state=1;
      writeSerial(input);
      Serial2.println("State chnaged to 1");
      return;
    }
    if (state==1){
      module_address+=String(input);
      state=2;
      writeSerial(input);
      Serial2.println("State chnaged to 2");
      return;
    }
    if (state==2 && data_length==""){
      data_length=String(input);
      return;
    }
    if (state==2 && data_length!=""){
      data_length+=String(input);
      Serial2.println("Data length: " + data_length);
      if (data_length.toInt()==0){
        state=4;
        Serial2.println("State changed to 4");
      }
      else{
        state=3;
        Serial2.println("State changed to 3");
      }
      writeSerial(input);
      return;
    }
    if (state==3){
      data+=String(input);
      if ((long) data.length()==data_length.toInt()*2){
        if (data.length()==2){
          char b = data[0];
          if (b >= 56){
            Serial2.println("First bit is 1");
            int requested_speed;
            //set the requested speed to the value of the last 4 bits
            requested_speed = data[1];
            if (requested_speed > 57){
              requested_speed = requested_speed - 7;
            }
            requested_speed = requested_speed - 48;
            int new_speed = requested_speed;
            if (module_address=="33"){
              if (ra_transformed_distance <= ra_original_distance*ra_ratio){
                if (requested_speed!=3){
                  //set new speed to closest higher speed than speeds[requested_speed] * ra_ratio 
                  for (int i = 0; i < 16; i++){
                    if (speeds[i] > speeds[requested_speed]*ra_ratio){
                      new_speed = i;
                      break;
                    }
                  }
                } else {new_speed = 15;};
              } else {
                if (requested_speed!=15 || requested_speed!= 14){
                  //set new speed to closest lower speed than speeds[requested_speed] * ra_ratio 
                  for (int i = 14; i >= 0; i--){
                    if (speeds[i] < speeds[requested_speed]*ra_ratio){
                      new_speed = i;
                      break;
                    }
                  }
                } else {new_speed = 3;}; 
              }
            }
            if (module_address=="35"){
              if (dec_transformed_distance <= dec_original_distance*dec_ratio){
                if (requested_speed!=3){
                  //set new speed to closest higher speed than speeds[requested_speed] * ra_ratio 
                  for (int i = 0; i < 16; i++){
                    if (speeds[i] > speeds[requested_speed]*dec_ratio){
                      new_speed = i;
                      break;
                    }
                  }
                } else {new_speed = 15;};
              } else {
                if (requested_speed!=15 || requested_speed!= 14){
                  //set new speed to closest lower speed than speeds[requested_speed] * ra_ratio 
                  for (int i = 14; i >= 0; i--){
                    if (speeds[i] < speeds[requested_speed]*dec_ratio){
                      new_speed = i;
                      break;
                    }
                  }
                } else {new_speed = 3;}; 
              }
            }
            //set 4 last bits to new speed
            char speed_char = new_speed;
            speed_char = speed_char + 48;
            if (speed_char > 57){
              speed_char = speed_char + 7;
            }
            writeSerial(data[0]);
            writeSerial(speed_char);
            if (module_address=="33"){
              setSpeedRa(requested_speed, new_speed); 
            }
            if (module_address=="35"){
              setSpeedDec(requested_speed, new_speed);
            }
          }
          
        }
        if (data.length()==12){
          int requested_speed1 = data[2];
          int requested_speed2 = data[3];
          if (requested_speed1 > 57){
            requested_speed1 = requested_speed1 - 7;
          }
          if (requested_speed2 > 57){
            requested_speed2 = requested_speed2 - 7;
          }
          requested_speed1 = requested_speed1 - 48;
          requested_speed2 = requested_speed2 - 48;
          int speed_high = speeds[requested_speed1];
          int speed_low = speeds[requested_speed2];

          int new_speed_high = speed_high;
          int new_speed_low = speed_low;

          for (int i=4; i<=11; i++){
            if (data[i] > 57){
              data[i] = data[i] - 7;
            }
            data[i] = data[i] - 48;
          }
          
          int time_high = (data[4] << 12) | (data[5] << 8) | (data[6] << 4) | data[7];
          int time_low = (data[8] << 12) | (data[9] << 8) | (data[10] << 4) | data[11];
          Serial2.println("Time high: " + String(time_high) + " Time low: " + String(time_low) + " Speed high: " + String(requested_speed1) + " Speed low: " + String(requested_speed2));

          int new_time_high = time_high;
          int new_time_low = time_low;

          //calculate real speed based on the ratio of times
          int real_speed = (speed_high*time_high + speed_low*time_low)/(time_high+time_low);
          int new_real_speed = real_speed;
          
          if (module_address=="33"){
            new_real_speed = real_speed*ra_ratio;
          } else if (module_address=="35"){
            new_real_speed = real_speed*dec_ratio;
          }
          //if new real speed is higher than speed high, set new high speed to the closest higher speed than high speed
          //if new real speed is lower than speed low, set new low speed to the closest lower speed than low speed
          
          if (new_real_speed > speed_high){
            for (int i = 0; i < 16; i++){
              if (speeds[i] > new_real_speed){
                requested_speed1 = i;
                new_speed_high = speeds[i];
                break;
              }
            }
          } else if (new_real_speed < speed_low){
            for (int i = 15; i >= 0; i--){
              if (speeds[i] < new_real_speed){
                requested_speed2 = i;
                new_speed_low = speeds[i];
                break;
              }
            }
          }

          new_time_high = time_high*new_speed_high/new_real_speed;
          new_time_low = time_low*new_speed_low/new_real_speed;
        
          //send the new speeds and times
          data[2] = requested_speed1 + 48;
          if (data[2] > 57){
            data[2] = data[2] + 7;
          }
          data[3] = requested_speed2 + 48;
          if (data[3] > 57){
            data[3] = data[3] + 7;
          }
          data[4] = (new_time_high >> 12) & 0x0F;
          data[5] = (new_time_high >> 8) & 0x0F;
          data[6] = (new_time_high >> 4) & 0x0F;
          data[7] = new_time_high & 0x0F;
          data[8] = (new_time_low >> 12) & 0x0F;
          data[9] = (new_time_low >> 8) & 0x0F;
          data[10] = (new_time_low >> 4) & 0x0F;
          data[11] = new_time_low & 0x0F;
          for (int i=4; i<=11; i++){
            data[i] = data[i] + 48;
            if (data[i] > 57){
              data[i] = data[i] + 7;
            }
          }
          for (int i=0; i<12; i++){
            writeSerial(data[i]);
          }

          if (module_address=="33"){
            setSpeedRa(real_speed, new_real_speed);
          }
          if (module_address=="35"){
            setSpeedDec(real_speed, new_real_speed);
          }


        }
        if (data.length()!=12 && data.length()!=2){
          for (int i=0; i<data.length(); i++){
            writeSerial(data[i]);
          }
        }
        state=4;
        Serial2.println("State chnaged to 4");
        return;
      }
    }
    if (state==4 && checksum==""){
      checksum=String(input);
      writeSerial(input);
      Serial2.println("State chnaged to 4");
      return;
    }
    if (state==4 && checksum!=""){
      checksum+=String(input);
      state=5;
      module_address="";
      data_length="";
      data="";
      checksum="";
      end_of_message="";

      Serial2.println("State chnaged to 5");
      writeSerial(input);
    }
  }
  void writeSerial(char input){
      // Write serial data
      Serial3.write(input);
  }
};
