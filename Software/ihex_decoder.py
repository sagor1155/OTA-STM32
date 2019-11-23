import time
import os
import serial
import binascii
import winsound

ser = serial.Serial('COM10', 9600)
ser.close()
ser.open()
print("serial connected")

fp = open('ClockModuleStudy.hex')
while True:
    msg = ""
    for i in range(0, 1, 1):
        line = fp.readline()
        msg = msg + line
    if not line:
        break

    try:
        #byte_string = line.split(':', 1)
        ser.write(line.encode())
        rd = ser.readline()
        print(rd)
        #time.sleep(0.001)

    except Exception as e:
        ser.close()
        print (e)

winsound.PlaySound("SystemExit", winsound.SND_ALIAS)
ser.close()
print("serial disconnected")
