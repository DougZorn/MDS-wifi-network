import serial
ser = serial.Serial(2)
print ser.name
ser.flushInput()
f = open('testwrite.csv' , 'w')

#loop runs from 1 to n-1. so if you pass a param for the number of nodes be sure to add one to the loop
for x in range(1,5):
	while ser.read() != str(x):
		pass
	
	f.write(str(x))
	f.write(ser.readline())

f.close()