import serial
ser = serial.Serial(2)
print ser.name
line1 = ser.readline()
line2 = ser.readline()
line3 = ser.readline()
line4 = ser.readline()
line5 = ser.readline()
line6 = ser.readline()
line7 = ser.readline()
line8 = ser.readline()
line9 = ser.readline()
line10 = ser.readline()
line11 = ser.readline()


import serial
ser = serial.Serial(2)
print ser.name
f = open('testwrite.csv' , 'w')
f.write(ser.readline())
f.write(ser.readline())
f.write(ser.readline())
f.write(ser.readline())
f.write(ser.readline())


f.close()