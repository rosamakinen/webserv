#!/usr/local/bin/python3

import os
import sys
from datetime import datetime

# Get data from form
form_input = os.environ.get('QUERY_STRING')

if not form_input:
	date_time = datetime.now()
	html_content = f"<html>Hello this is CGI error </html>" #how to access the error pages here? can we put out our own error??
	response = f"HTTP/1.1 400 Bad Request\r\nDate: {date_time}\r\nContent-Type: text/html\r\nContent-Length: {len(html_content)} \r\n\r\n"
	sys.stdout.write(response + html_content)
	exit()


parts = form_input.split("&")

if len(parts) != 2:
	date_time = datetime.now()
	html_content = f"<html>Hello this is CGI error </html>" #how to access the error pages here? can we put out our own error??
	response = f"HTTP/1.1 400 Bad Request\r\nDate: {date_time}\r\nContent-Type: text/html\r\nContent-Length: {len(html_content)} \r\n\r\n"
	sys.stdout.write(response + html_content)
	exit()

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

date_time = datetime.now()
html_content = f"<html><head>Hello {first_name} {last_name} </head></html>"
response = f"HTTP/1.1 200 OK\r\nDate: {date_time}\r\nContent-Type: text/html\r\nContent-Length: {len(html_content)} \r\n\r\n"
sys.stdout.write(response + html_content)

# Write a valid httpresponse to stdout
