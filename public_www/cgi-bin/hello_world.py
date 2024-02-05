#!/usr/local/bin/python3

import os
import sys

# Get data from fields
form_input = os.environ.get('QUERY_STRING')

# form_input = "first_name=rosa&last_name=rmrmr"

if form_input == None:
	output = "500 internal CGI error"
	sys.stdout.write(output)
	sys.exit

parts = form_input.split("&")

if len(parts) != 2:
	output = "500 internal CGI error"
	sys.stdout.write(output)
	sys.exit

first_name = parts[0].split("=")[1]
last_name = parts[1].split("=")[1]

# print ("Content-type:text/html")
# print()
# print ("<html>")
# print ('<head>')
# print ("<title>Hello - Second CGI Program</title>")
# print ('</head>')
# print ('<body>')
# print ("<h2>Hello %s %s</h2>" % (first_name, last_name))
# print ('</body>')
# print ('</html>')

html_content = f"<html>Hello {first_name} {last_name} </html>"
response = f"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: {len(html_content)} \r\n\r\n"
sys.stdout.write(response + html_content)

# Write a valid httpresponse to stdout
