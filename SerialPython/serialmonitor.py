import serial
ser = serial.Serial(2)
print ser.name
f = open('testwrite.csv' , 'w')
while ser.read() != '1':
	pass

f.write('1')
f.write(ser.readline())
while ser.read() != '2':
	pass

f.write('2')
f.write(ser.readline())
f.close()