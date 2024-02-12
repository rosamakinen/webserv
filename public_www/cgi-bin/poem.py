#!/usr/local/bin/python3

import sys

html_content = f"""<!DOCTYPE html>
	<html lang="en">
	<head>
		<meta charset="UTF-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<title>** poem **</title>
	</head>
	<body>
	<center>
		<h1>roses are red<br>violets are blue<br>why then, oh why<br>loo you not at me<br>well, someone has to be<br>eyes<br><br> poem created by the GPT-3</h1>
	</center>
  	</body>
	</html>"""
date_time = datetime.now()
response = f"HTTP/1.1 200 OK\r\nDate: {date_time}\r\nContent-Type: text/html\r\nContent-Length: {len(html_content)} \r\n\r\n"
sys.stdout.write(response + html_content)
