import serial
import sys
ser = serial.Serial(int(sys.argv[1])-1)
print ser.name
ser.flushInput()
f = open('testwrite.csv' , 'w')

#loop runs from 1 to n-1. so if you pass a param for the number of nodes be sure to add one to the loop
loop = (int(sys.argv[2])+1)
for x in range(1,loop):
	while True:
		y = ser.readline()
		if y[0] == str(x):
			f.write(y)
			break


f.close()
