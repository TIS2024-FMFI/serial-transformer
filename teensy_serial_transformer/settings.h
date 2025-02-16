#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

class Settings {
private:
  double ra_ratio = 1.0;
  double dec_ratio = 1.0;

  void readConfigSD() {
    File configFile = SD.open("config.txt", FILE_READ);
    if (configFile) {
      while (configFile.available()) {
        String line = configFile.readStringUntil('\n');
        ra_ratio = (double)line.toFloat();
        dec_ratio = (double)line.toFloat();
      }
      configFile.close();
    } else {
      Serial.println("No config file found yet");
    }
  }

  void writeConfigToSD() {
    File configFile = SD.open("config.txt", FILE_WRITE);
    if (configFile) {
      //delete all content
      configFile.truncate(0);
      configFile.println(String(ra_ratio));
      configFile.println(String(dec_ratio));
    } else {
      Serial.println("Error opening file to write config");
    }
  }
public:
  Settings() {
    readConfigSD();
  }
  void setRaRatio(double ra) {
    ra_ratio = ra;
    writeConfigToSD();
  }
  void setDecRatio(double dec) {
    dec_ratio = dec;
    writeConfigToSD();
  }

  double getRaRatio() {
    return ra_ratio;
  }
  double getDecRatio() {
    return dec_ratio;
  }
};