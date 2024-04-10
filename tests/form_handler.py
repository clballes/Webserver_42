#!/usr/bin/env python3

import cgi

# Print Content-type header
print("Content-type: text/html\n")

# Retrieve form data
form = cgi.FieldStorage()

# Check if form fields are present
if "name" not in form or "email" not in form or "message" not in form:
    print("<h1>Error: All fields are required!</h1>")
else:
    # Retrieve form data
    name = form["name"].value
    email = form["email"].value
    message = form["message"].value

    # Process the form data
    print("<h1>Form Submitted Successfully!</h1>")
    print("<p><strong>Name:</strong> {}</p>".format(name))
    print("<p><strong>Email:</strong> {}</p>".format(email))
    print("<p><strong>Message:</strong> {}</p>".format(message))