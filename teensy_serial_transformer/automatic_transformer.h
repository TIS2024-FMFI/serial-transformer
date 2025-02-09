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
      if (Serial2.available() > 0) {
          // Read the incoming byte:
          input = Serial2.read();
          
          transformSerial();
          Serial.println(input);
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
      Serial.println("State chnaged to 0");
      return;
    }
    if (state==0){
      module_address=String(input);
      state=1;
      writeSerial(input);
      Serial.println("State chnaged to 1");
      return;
    }
    if (state==1){
      module_address+=String(input);
      state=2;
      writeSerial(input);
      Serial.println("State chnaged to 2");
      return;
    }
    if (state==2 && data_length==""){
      data_length=String(input);
      return;
    }
    if (state==2 && data_length!=""){
      data_length+=String(input);
      Serial.println("Data length: " + data_length);
      if (data_length.toInt()==0){
        state=4;
        Serial.println("State chnaged to 4");
      }
      else{
        state=3;
        Serial.println("State chnaged to 3");
      }
      writeSerial(input);
      return;
    }
    if (state==3){
      data+=String(input);
      if ((long) data.length()==data_length.toInt()){
        if (data.length()==1){
          char b = data[0];
          if ((b >> 7) & 1){
            Serial.println("First bit is 1");
            int requested_speed;
            //set the requested speed to the value of the last 4 bits
            requested_speed = b & 0x0F;
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
            b = (b & 0xF0) | new_speed;
            writeSerial(b);
            if (module_address=="33"){
              setSpeedRa(requested_speed, new_speed); 
            }
            if (module_address=="35"){
              setSpeedDec(requested_speed, new_speed);
            }
          }
          
        }
        if (data.length()==6){
          char b = data[1];
          int requested_speed1 = b >> 4;
          int requested_speed2 = b & 0x0F;
          int speed_high = speeds[requested_speed1];
          int speed_low = speeds[requested_speed2];

          int new_speed_high = speed_high;
          int new_speed_low = speed_low;

          
          int time_high = (data[2] << 8) | data[3];
          int time_low = (data[4] << 8) | data[5];

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
          writeSerial(data[0]);
          writeSerial((requested_speed1 << 4) | requested_speed2);
          writeSerial((new_time_high >> 8) & 0xFF);
          writeSerial(new_time_high & 0xFF); 
          writeSerial((new_time_low >> 8) & 0xFF);
          writeSerial(new_time_low & 0xFF);

          if (module_address=="33"){
            setSpeedRa(real_speed, new_real_speed);
          }
          if (module_address=="35"){
            setSpeedDec(real_speed, new_real_speed);
          }


        }
        state=4;
        Serial.println("State chnaged to 4");
        return;
      }
    }
    if (state==4 && checksum==""){
      checksum=String(input);
      writeSerial(input);
      Serial.println("State chnaged to 4");
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

      Serial.println("State chnaged to 5");
      writeSerial(input);
    }
  }
  void writeSerial(char input){
      // Write serial data
      Serial3.write(input);
  }
};
