#open the file with the data
import sys, os
import traceback
import Image

def getIndex(num, arr):
	for x in range(0,len(arr)):
		if int(num) <= arr[x][0]:
			return x
		if num < arr[x][0]:
			return x
		#print("nothing was found")
		#print(str(arr[0][0]) + "is checked against" + num)
try:
	img = Image.new( 'RGB', (500,500), "black") # create a 300x300 image 
	pixels = img.load() 
	cmapFile = open('cMap.data')
	cMap = cmapFile.read()
	cMap = cMap.split("\n")
	for x in range(0,len(cMap)):
		cMap[x] = cMap[x].split(" ")
		cMap[x] = map(int,cMap[x])
	print(cMap)
	#print(cMap)


	f = open('Lab3Mystery.data')
	text = f.read()
	inputNumbers = text.replace("\n", ", ")
	inputNumbers = inputNumbers.split(',')
	inputNumbers.pop()
	print(len(inputNumbers))
	try:
		result = map(int, inputNumbers)
	except:
		pass
	temp = 0
	temp = 0
	#print(cMap)
	for x in range(0,500):
		for y in range(0,500):
			#print(temp)
			index = getIndex(inputNumbers[temp], cMap)
			#print(inputNumbers[temp])
			#print(index)
			pixels[x,y] = (cMap[index][1], cMap[index][2], cMap[index][3]) # set a made-up color (R, G, B)
			temp = temp + 1
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