#!/usr/bin/env python3

import os

print("Content-Type: text/html\n")  # Required header indicating HTML content

print("<html><head><title>CGI Python Script</title></head><body>")
print("<h1>Hello CGI World!</h1>")
print("<p>This is a simple CGI Python script.</p>")

for key, value in os.environ.items():
    print(f'<p>{key}={value}</p>')

print("</body></html>")
