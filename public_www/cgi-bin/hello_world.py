#!/usr/bin/python

import os
import sys

# Get data from fields
form_input = os.environ.get('QUERY_STRING')

if form_input == None:
	print("CGI service error")
	sys.exit()

parts = form_input.split("&")

if len(parts) != 2:
	print("CGI service error")
	sys.exit()

first_name = parts[0].split("=")[1]
last_name = parts[1].split("=")[1]

print ("Content-type:text/html")
print()
print ("<html>")
print ('<head>')
print ("<title>Hello - Second CGI Program</title>")
print ('</head>')
print ('<body>')
print ("<h2>Hello %s %s</h2>" % (first_name, last_name))
print ('</body>')
print ('</html>')

string = 'OKAYYYY IS THIS WORKINGGG??'

sys.stdout.write(string)

# Write a valid httpresponse to stdout
