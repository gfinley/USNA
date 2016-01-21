import numpy as np
import sys
import os
class HomogeneousCoordinate:
	def __init__(self,xsize,ysize):
		self.Matrix = [[0 for x in range(int(ysize))] for y in range(int(xsize))] #https://stackoverflow.com/questions/6667201/how-to-define-two-dimensional-array-in-python	 
 		self.x = int(xsize)
 		self.y = int(ysize)
 	def fillMatrix(self,x,y):
 		for x_temp in range(0,int(x)):
 			data = raw_input("please enter " + str(y) + " numbers of space seperated numbers for row\n").split()
 			while len(data) != int(y):
 				#print("that was the wrong number of numbers")
 				data = raw_input("please enter " + str(y) + " numbers of space seperated numbers\n").split()

 			for y_temp in range(0,int(y)):
 				#print("x:" + str(x_temp) + " y:" + str(y_temp)+ " value:" + str(data[y_temp]))
 				self.Matrix[int(x_temp)][int(y_temp)] = int(data[y_temp])
 			print(self.Matrix[x_temp])
 		

 	def fillMatrix2(self,x,y):
 		temp = []
 		for x_temp in range(0,int(x)):
 			data = raw_input("please enter " + str(y) + " numbers of space seperated numbers\n").split()
 			while len(data) != int(y):
 				print("that was the wrong number of numbers")
 				data = raw_input("please enter " + str(y) + " numbers of space seperated numbers\n").split()
 			data = map(int, data)
 			temp.append(data)
 		self.Matrix = np.matrix(temp)
 		return self.Matrix

x1 = raw_input("please enter the demenstions of the first matrix, example \"4 4\"\n").split()
x2 = raw_input("please enter the demenstions of the first matrix, example \"3 4\"\n").split()
while len(x1) != 2 or len(x2) != 2 or int(x1[1]) != int(x2[0]):
	print("The inputed matrixes cannot be multiplied with each other \n")
	x1 = raw_input("please enter the demenstions of the first matrix, example \"4 4\"\n").split()
	x2 = raw_input("please enter the demenstions of the second matrix, example \"3 4\"\n").split()

matrix1 = HomogeneousCoordinate(x1[0],x1[1])
matrix2 = HomogeneousCoordinate(x2[0],x2[1])
a = matrix1.fillMatrix2(x1[0], x1[1])
print("Matrix 1\n")
print(a)
print("\nMatrix 2 \n")
b = matrix1.fillMatrix2(x2[0], x2[1])
print(b)
print("\n(Matrix 1) * (Matrix 2)\n")
print(np.dot(a,b))