class AutomaticSerialTransformer {
  public:
    double ra_ratio = 1.0;
    double dec_ratio = 1.0;

    double ra_original_distance = 0.0;
    double dec_original_distance = 0.0;

    double ra_transformed_distance = 0.0;
    double dec_transformed_distance = 0.0;

    char data;
    char buffer[100];

  AutomaticSerialTransformer(double ra, double dec) {
      ra_ratio = ra;
      dec_ratio = dec;
  }

  void readSerial(){
      // Read serial data
      if (Serial2.available() > 0) {
          // Read the incoming byte:
          data = Serial2.read();
          
          transformSerial();
          Serial.println(data);
      }
  }
  void transformSerial(){
    //raise "Not implemented"
    //throw "Not implemented";
    return;

  }
  void writeSerial(){
      // Write serial data
      Serial3.write(data);
  }
};
