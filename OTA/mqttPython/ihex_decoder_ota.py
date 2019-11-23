import time
import os
import serial
import binascii
import winsound

ser = serial.Serial('COM10', 9600)
ser.close()
ser.open()
print("serial connected")

fp = open('example.hex')
while True:
    line = fp.readline()
    if not line:
        break

    try:
        #byte_string = line.split(':', 1)
        ser.write(str(line).encode())
        rd = ser.readline()
        print(rd)
        #time.sleep(0.001)

    except Exception as e:
        ser.close()
        print (e)

winsound.PlaySound("SystemExit", winsound.SND_ALIAS)
ser.close()
print("serial disconnected")
