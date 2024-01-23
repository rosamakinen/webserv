#!/usr/bin/pythons

# HTTP Header
print("Content-Type:application/octet-stream; name = \"FileName\"\r\n")
print("Content-Disposition: attachment; filename = \"FileName\"\r\n\n")
# Original File
my_file = open("textFile.txt", "rb")
# read the file content
text = my_file.read()
print(text)

# Close opened file
my_file.close()
