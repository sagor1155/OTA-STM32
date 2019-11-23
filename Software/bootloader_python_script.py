import serial
import time

ser = serial.Serial('COM10', 9600, timeout=0)

with open("example.hex") as f:
    content = f.read()
content = [x.strip() for x in content]

number_of_line = content.count(':')
for x in range(0, number_of_line):
    index = content.index('')
    line = content[0: index]
    msg = ''.join(line)
    ser.write(msg.encode())
    #ser.write(b'\n')
    #print (msg)
    
    data = ser.readline()
    print(data)
    time.sleep(0.01)
    #print(str(line))
    del content[0:index+1]

#print(str(content))
print("END OF LINE")
ser.close()
