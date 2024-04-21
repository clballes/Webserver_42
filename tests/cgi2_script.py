#!/usr/bin/env python3

print("Content-Type: text/plain\r\n")  # CGI field: Content-Type
print("Status: 200 OK\r\n\r\n")  # CGI field: Status

# Content
print("This is the response content.")