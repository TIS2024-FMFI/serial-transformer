import serial
import time

#connect to serial COM1 with baudrate 9600

ser = serial.Serial('COM6', 9600)

#send data to serial port
ser.write(b'UFF018C')
# char = 136
# ser.write(bytes([char]))
ser.write(b'1111')

#wait for 1 second
time.sleep(1)

ser.write(b'UFF01')
char = 136
ser.write(bytes([char]))
ser.write(b'1111')
#close serial port
ser.close()
