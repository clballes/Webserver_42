#!/usr/bin/env python3

import cgi
import os

def handle_get_request():
    # Set content type to text/html
    print("Content-Type: text/html        ")
    print("Status:                   200\n")
    with open('/Users/clballes/Desktop/web/www/index.html', 'r') as file:
        content = file.read()
    print(content)
# Main entry point
if __name__ == "__main__":
    handle_get_request()
