#!/usr/bin/python3

import json
import codecs
import getopt
import class_parts as c
import sys
import pprint
pp = pprint.PrettyPrinter(indent=4)

input_file = ""

try:
	opts, args = getopt.getopt(sys.argv[1:], "ho:vi:", ["help", "output="])
except getopt.GetoptError as err:
	print(err)

for o,a in opts:
		if o == "-i":
			input_file = a

content = ""
with codecs.open(input_file, "r", "utf8") as f:
	content = f.read()

dic = {}
try:
	dic = json.loads(content)
except:
	## throw exception
	print("Error while parsing file \""+input_file+"\"")
	
print(type(dic))
print()

ret = c.generateHeaders()
ret += c.generateClassHeadAndConstructor("Tpa")
ret += c.generateTableName()
ret += c.generateFields(dic['fields'])
ret += c.generateGetFromDB(dic['fields'])
ret += c.generateStaticClassContent()
ret += c.generateClassEnd()
ret += c.generateFieldDefinitions(dic['fields'],dic['tableName'])

print(ret)
