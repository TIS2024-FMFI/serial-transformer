#include <string>
#include <Arduino.h>



int speeds[] = {-4000, -2000, -300, 37, 38, 39, 40, 41, 42, 43, 44, 80, 300, 2000, 4000, 0};
class AutomaticSerialTransformer {
  public:
    SerialLogger* logger;
    double ra_ratio = 1.03;
    double dec_ratio = 1.0;
    int original_speed_ra = 15;
    int original_speed_ra_steps = 0;
    int transformed_speed_ra = 15;
    int transformed_speed_ra_steps = 0;
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
      if (data.length()==data_length.toInt()){
        if (data.length()==1){
          char b = data[0];
          if ((b >> 7) & 1){
            Serial.println("First bit is 1");
            int requested_speed;
            //set the requested speed to the value of the last 4 bits
            requested_speed = b & 0x0F;
            int new_speed = requested_speed;
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
            //set 4 last bits to new speed
            b = (b & 0xF0) | new_speed;
            
            setSpeedRa(requested_speed, new_speed); 

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
