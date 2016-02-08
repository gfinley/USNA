#open the file with the data
import sys, os
import traceback
import Image

try:

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
	for x in range(1,1001):
		if result.count(x) != 0:
			if result.count(x) > 500 or temp > 500:
				print(x)
				temp = 0;
			else:
				temp = temp + result.count(x)
	#print(str(len(inputNumbers)))
	#print(str(len(results)))
	#print(inputNumbers)
except Exception as e:
	tb = sys.exc_info()[2]
	print("line:" + str(tb.tb_lineno) + " " + str(e))