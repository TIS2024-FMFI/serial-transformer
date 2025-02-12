class UniversalTransformer {
public:
  char data;
  char buffer[100];
  UniversalTransformer() {}
  
  
  void readSerial(){
      // Read serial data
      if (Serial.available() > 0) {
          // Read the incoming byte:
          data = Serial.read();
          
          transformSerial();
          Serial2.println(data);
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