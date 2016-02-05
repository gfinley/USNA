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
	#print(temp)
	return temp
	#print(temp[0][3],temp[1][3],temp[2][3])

def numConvert(numberTicks):
	temp = np.pi/2-numberTicks*180/255*0.0174533

	return temp

def radiansToTicks(rads):
	angle = np.round(rads/np.pi*255)
	return(angle)


#print(Amatrix(9.5,0,0,0))
angles = [128,158,45,100,190] #SAME AS PICTURE

#angles2 = [0,-.3665,1.029744,.34906,-.76794]
angles2 = [0,0,0,-np.pi/4,0]
print(angles2)
angles2[0] = input("Angle for servo 1 ")
angles2[1] = input("Angle for servo 2 ")
angles2[2] = input("Angle for servo 3 ")
angles2[3] = input("Angle for servo 4 ")
angles2[4] = input("Angle for servo 5 ")
print(angles2)

#1
position = Amatrix(10,0,0,angles2[0])
print(position)
print()
#2
position = np.dot(position , Amatrix(9,0, np.pi/2, angles2[1]))
print(position)
print()
#3
position = np.dot(position , Amatrix(4,0, 0, angles2[2]))
print(position)
print()
#4
position = np.dot(position , Amatrix(0,0, -np.pi/2, angles2[3] ))
print(position)
print()
#5
position = np.dot(position , Amatrix(0,7.5, 0, angles2[4] ))
print(position)
ser = serial.Serial("/dev/ttyUSB0")

angle1 =  127 - radiansToTicks(angles2[0]) 
if angle1 <= 0:
	angle1 = 1

command1 = "SV1 M" + str(angle1) + "\r"
ser.write(command1) 


angle2 =  127 - radiansToTicks(angles2[1])
command2 = "SV2 M" + str(angle2) + "\r"
ser.write(command2)

angle3 =  127 - radiansToTicks(angles2[2])
command3 = "SV3 M" + str(angle3) + "\r"
ser.write(command3)


angle4 =  255 + radiansToTicks(angles2[3])
if angle4 == 0:
	angle4 = 1;
command4 = "SV4 M" +  str(angle4) + "\r"
ser.write(command4)


angle5 =  127 - radiansToTicks(angles2[4])
command5 = "SV5 M" + str(angle5) + "\r"
ser.write(command5)
print(angle1)
print(angle2)
print(angle3)
print(angle4)
print(angle5)



 #Connects to the USB port
beta = np.arcsin(-position[2][0])
phi = np.arcsin(position[2][1]/np.cos(beta))
otherPhi = np.arcsin(position[2][0]/np.cos(beta))

print("yaw: " + str(otherPhi))
print("pitch: " + str(beta))
print("roll: " + str(phi))






#First command "wakes up" the controller board





#more ser.write commands here
#ser.write('SV1 M255\r')# (but with no angle brackets).
ser.close()