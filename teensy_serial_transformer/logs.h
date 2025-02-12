#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <stdio.h>
#include <time.h>
class SerialLogger {
  public:
    SerialLogger(){}
    void delete_all_logs() {
        //delete all files on sd card which start with log
        File root = SD.open("/");
        File file = root.openNextFile();
        while (file) {
            String filename = file.name();
            char filename_char[filename.length() + 1];
            filename.toCharArray(filename_char, filename.length() + 1);
            
            if (filename.startsWith("log")) {
                file.close();
                SD.remove(filename_char);
            }
            file = root.openNextFile();
        }
    }

    void log(String message) {

        //write to sd card file named log{today_date}.txt
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        String today_date = String(tm.tm_year + 1900) + "-" + String(tm.tm_mon + 1) + "-" + String(tm.tm_mday);
        String filename = "log" + today_date + ".txt";
        String today_time = String(tm.tm_hour) + ":" + String(tm.tm_min) + ":" + String(tm.tm_sec);
        message = today_time + " " + message;
        //filenname string to char*
        char filename_char[filename.length() + 1];
        filename.toCharArray(filename_char, filename.length() + 1);
        File logFile = SD.open(filename_char, FILE_WRITE);
        if (logFile) {
            Serial2.println("Writing to log file");
            logFile.println(message);
            logFile.close();
        } else {
            Serial2.println("Error opening file to write logs");
        }
    }
    String read_logs(int amount) {
        log("Reading logs");
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        String today_date = String(tm.tm_year + 1900) + "-" + String(tm.tm_mon + 1) + "-" + String(tm.tm_mday);
        String filename = "log" + today_date + ".txt";
        char filename_char[filename.length() + 1];
        filename.toCharArray(filename_char, filename.length() + 1);

        File logFile = SD.open(filename_char, FILE_READ);
        if (logFile) {
            if (amount == 0) {
                //read all logs
                String logs = "";
                while (logFile.available()) {
                    char c = logFile.read();
                    logs += c;
                }
                logFile.close();
                return logs;
            }
            //go to the end of file
            logFile.seek(logFile.size());
            //get the current position
            int current_position = logFile.position();
            int new_position = current_position;
            //go back amount of lines from the end of file
            while (amount > 0) {
                new_position--;
                logFile.seek(new_position);
                char c = logFile.read();
                if (c == '\n') {
                    amount--;
                }
                if (new_position == 0) {
                    break;
                }
            }
            String logs = "";
            //write lines to logs string until you reach end of file so you go forward
            for (int i = new_position; i < current_position; i++) {
                logFile.seek(i);
                char c = logFile.read();
                logs += c;
            }
            logFile.close();
            return logs;
        } else {
            Serial2.println("Error opening file");
            return "";
        }
    };
};