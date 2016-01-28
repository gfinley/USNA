#!/usr/bin/python
import serial
import numpy as np


def Amatrix(l,d,alpha,theta):
	matrix1 = [ [ np.cos(theta),-np.sin(theta),0,0], 
				[ np.sin(theta), np.cos(theta),0,0],
				[0,0,1,0],
				[0,0,0,1]
			]
	matrix2 = [ [1,0,0,l],
				[0,1,0,0],
				[0,0,1,d],
				[0,0,0,1]
			]
	matrix3 = [ [1,0,0,0],
				[ 0,np.cos(alpha),-np.sin(alpha),0], 
				[ 0,np.sin(alpha), np.cos(alpha),0],
				[0,0,0,1]	
			]
	temp = np.dot(matrix1,matrix2)
	temp = np.dot(temp,matrix3)
	print(temp)
	return temp
	#print(temp[0][3],temp[1][3],temp[2][3])

def numConvert(numberTicks):
	temp = np.pi/2-numberTicks*180/255*0.0174533

	return temp

#print(Amatrix(9.5,0,0,0))
angles = [63,230,5,20,128]
#angles = [128,128,128,128,128]
print(numConvert(angles[0]))
print(numConvert(angles[1]))
#1
position = Amatrix(10,0,0,numConvert(angles[0]))
print(position[0][3],position[1][3],position[2][3])
#2
position = np.dot(position , Amatrix(9,0, 90, numConvert(angles[1])))
print(position[0][3],position[1][3],position[2][3])
#3
position = np.dot(position , Amatrix(4,0, 0, numConvert(angles[2])))
print(position[0][3],position[1][3],position[2][3])
#4
position = np.dot(position , Amatrix(0,0, np.pi/2, numConvert(angles[3]) - np.pi/2))
print(position[0][3],position[1][3],position[2][3])
#5
position = np.dot(position , Amatrix(0,7.5, 0, numConvert(angles[4])))
print(position[0][3],position[1][3],position[2][3])
print(numConvert(angles[4]))


ser = serial.Serial("/dev/ttyUSB0") #Connects to the USB port
temp = numConvert(0)
command1 = "SV1 M" + str(angles[0]) + "\r"
command2 = "SV2 M" + str(angles[1]) + "\r"
command3 = "SV3 M" + str(angles[2]) + "\r"
command4 = "SV4 M" + str(angles[3]) + "\r"
command5 = "SV5 M" + str(angles[4]) + "\r"

ser.write(command1) #First command "wakes up" the controller board
ser.write(command2)
ser.write(command3)
ser.write(command4)
ser.write(command5)

#more ser.write commands here
#ser.write('SV1 M255\r')# (but with no angle brackets).
ser.close()