#!/usr/bin/env python3

import cgi
import os
# Function to handle GET request
def handle_get_request():
    # Set content type to text/html
    print("Content-Type: text/html\n\r")
    print("Status: 200\n\r")

    # Read the contents of the index.html file
    with open('/Users/clara/Desktop/web_server_2/www/index.html', 'r') as file:
        content = file.read()
    print(content)
# Main entry point
if __name__ == "__main__":
    handle_get_request()
