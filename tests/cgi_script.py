#!/usr/bin/env python3

import cgi

print("Content-type: text/html\n")

print("<html>")
print("<head>")
print("<title>CGI Tester</title>")
print("</head>")
print("<body>")
print("<h1>CGI Tester</h1>")
print("<p>This is a simple CGI tester script.</p>")

form = cgi.FieldStorage()

if "file" in form:
    file_path = form["file"].value
    try:
        with open(file_path, "r") as f:
            file_content = f.read()
        print("<h2>File Content:</h2>")
        print("<pre>{}</pre>".format(file_content))
    except IOError:
        print("<p>Error: Unable to open file '{}'.</p>".format(file_path))
else:
    print("<p>No file specified.</p>")

print("</body>")
print("</html>")