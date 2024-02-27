#!/usr/local/bin/python3

import os
import sys
from datetime import datetime

# Get data from form
form_input = os.environ.get('QUERY_STRING')

if not form_input:
	date_time = datetime.now()
	html_content = f"""<!DOCTYPE html>
	<html lang="en">
	<head>
		<meta charset="UTF-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<title>500 Internal Server Error</title>
	</head>
	<body>
	<center>
		<h1>Internal Server Error</h1>
	</center>
  	</body>
	</html>"""
	response = f"HTTP/1.1 502 Error in CGI\r\nDate: {date_time}\r\nContent-Type: text/html\r\nContent-Length: {len(html_content)} \r\n\r\n"
	sys.stdout.write(response + html_content)
	exit()


parts = form_input.split("&")

if len(parts) != 2:
	date_time = datetime.now()
	html_content = f"""<!DOCTYPE html>
	<html lang="en">
	<head>
		<meta charset="UTF-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<title>500 Internal Server Error</title>
	</head>
	<body>
	<center>
		<h1>Internal Server Error</h1>
	</center>
  	</body>
	</html>"""
	response = f"HTTP/1.1 502 Error in CGI\r\nDate: {date_time}\r\nContent-Type: text/html\r\nContent-Length: {len(html_content)} \r\n\r\n"
	sys.stdout.write(response + html_content)
	exit()

first_name = parts[0].split("=")[1]
last_name = parts[1].split("=")[1]

# this is to test cgi_timeout
#value = 1
#while (value):
#	date_time = datetime.now()

date_time = datetime.now()
html_content = f"<html><head>Hello {first_name} {last_name}  nice to meet you! </head></html>"
response = f"HTTP/1.1 200 OK\r\nDate: {date_time}\r\nContent-Type: text/html\r\nContent-Length: {len(html_content)} \r\n\r\n"
sys.stdout.write(response + html_content)

# Write a valid httpresponse to stdout
