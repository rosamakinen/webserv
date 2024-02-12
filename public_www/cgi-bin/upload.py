#!/usr/bin/pythons

import cgi, os

form = cgi.FieldStorage()
fileitem = form['filename']

# Check if the file has been uploaded
if fileitem.filename:
	open(os.getcwd() + '/cgi-bin/upload/' + os.path.basename(fileitem.filename), 'wb').write(fileitem.file.read())
	message = 'The file "' + os.path.basename(fileitem.filename) + '" was uploaded to ' + os.getcwd() + 'cgi-bin/upload'
else
	message = 'Uploading file failed'

# Write html response to stdout
print("HTTP/1.1 200 OK\r\n")
print("Content-Type: text/html;charset=utf-8\r\n")
print("<h1>" + message + "</h1>\r\n")
