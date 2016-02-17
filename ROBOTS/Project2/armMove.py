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
angles2 = [128,158,45,100,190] #SAME AS PICTURE

#angles2 = [0,-.3665,1.029744,.34906,-.76794]


angles2[0] = input("Angle for servo 1 ")
angles2[1] = 0
angles2[2] = input("Angle for servo 3 ")
angles2[3] = 0
angles2[4] = input("Angle for servo 5 ")

#1
position = Amatrix(10,0,0,angles2[0])

#2
position = np.dot(position , Amatrix(9,0, np.pi/2, angles2[1]))

#3
position = np.dot(position , Amatrix(4,0, 0, angles2[2]))

#4
position = np.dot(position , Amatrix(0,0, -np.pi/2, angles2[3] ))

#5
position = np.dot(position , Amatrix(0,7.5, 0, -angles2[4] ))
print(position)
angle1 =  127 - radiansToTicks(angles2[0]) 
if angle1 <= 0:
	angle1 = 1





 #Connects to the USB port
beta = np.arcsin(-position[2][0])
phi = np.arcsin(position[2][1]/np.cos(beta))
otherPhi = np.arcsin(position[1][0]/np.cos(beta))

print("yaw: " + str(otherPhi))
print("pitch: " + str(beta))
print("roll: " + str(phi))


p2 =[ 
	[ np.cos(otherPhi)*np.cos(beta), np.cos(otherPhi)*np.sin(beta)*np.sin(phi) - np.sin(otherPhi)*np.cos(phi) ,  np.cos(otherPhi)*np.sin(beta)*np.cos(phi) + np.sin(otherPhi)*np.sin(phi) ,position[0][3]], 
	[ np.sin(otherPhi)*np.cos(beta), np.sin(otherPhi)*np.sin(beta)*np.sin(phi) + np.cos(otherPhi)*np.cos(phi) ,  np.sin(otherPhi)*np.sin(beta)*np.cos(phi) - np.cos(otherPhi)*np.sin(phi) ,position[1][3]],
	[ -np.sin(beta), np.cos(beta)*np.sin(phi), np.cos(beta)*np.cos(phi),position[2][3]],
	[0,0,0,1]
	]
angleGuess3 = np.arccos(p2[2][2])
print("Angle 3 Guess  " + str(angleGuess3))
print("Angle 3 Actual " + str(angles2[2]))

angleGuess5 = np.arccos(p2[2][0]/np.sin(angleGuess3))
print("Angle 5 Guess  " + str(angleGuess5))
print("Angle 5 Actual " + str(angles2[4]))

angleGuess1 = -np.arccos(p2[0][2]/np.sin(angleGuess3))
print("Angle 1 Guess  " + str(angleGuess1))
print("Angle 1 Actual " + str(angles2[0]))




print(p2)
