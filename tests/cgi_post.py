#!/usr/bin/env python3

import cgi

# Print the HTTP header
print("Content-type: text/html\r\n\r\n")
print("<html>")
print("<head>")
print("<title>POST Method CGI</title>")
print("</head>")
print("<body>")
print("<h2>POST Method CGI</h2>")

# Get the form data
form = cgi.FieldStorage()

# Check if the form has any data
if form:
    print("<p>POST Data:</p>")
    print("<pre>")
    # Print each field and its value
    for field in form.keys():
        value = form[field].value
        print(f"{field}: {value}")
    print("</pre>")
else:
    print("<p>No POST data received.</p>")

print("</body>")
print("</html>")
