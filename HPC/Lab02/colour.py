#open the file with the data
import sys, os
import traceback
import Image
import sys


def getIndex(number, arr):
	for x in range(0,len(arr)):
		if number <= arr[x]:
			return x
		if number < arr[x]:
			return x
	return 0
		#print("nothing was found")
		#print(str(arr[0][0]) + "is checked against" + num)
def getColor(divisions, index):
	rang = (255 + 255 )/ divisions
	val1 = rang*index
	if index == 31:
		return (0,255,0)
	if (255 - val1) < 0:
		return (0, 0,val1)
	return (255 - val1, 0 ,val1)

try:
	img = Image.new( 'RGB', (500,500), "black") # create a 300x300 image 
	pixels = img.load() 
	cmapFile = open(sys.argv[1])
	cMap = cmapFile.read()
	cMap = cMap.split()
	cMap = map(int,cMap)
	#print(cMap)


	f = open(sys.argv[2])
	text = f.read()
	inputNumbers = text.replace("\n", ", ")
	inputNumbers = inputNumbers.split(',')
	inputNumbers.pop()
	print(len(inputNumbers))
	result = map(int, inputNumbers)
	tempNum = 0
	index = 0
	for x in range(0,500):
		for y in range(0,500):
			z = result[tempNum]
			index = getIndex(z, cMap)
			temp = getColor(len(cMap), index)
			pixels[x,y] = temp# set a made-up color (R, G, B)
			tempNum = tempNum + 1
	'''for x in range(1,1001):
		if result.count(x) != 0:
			if result.count(x) > 1000 or temp > 1000:
				print(x)
				temp = 0;
			else:
				temp = temp + result.count(x)''' 
	#print(str(len(inputNumbers)))
	#print(str(len(results)))
	img.show() # to display the image
	img.save('myimg.bmp') # to save the image in a file

except Exception as e:
	tb = sys.exc_info()[2]
	print("line:" + str(tb.tb_lineno) + " " + str(e))