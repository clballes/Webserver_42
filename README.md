# HTTP Server Implementation in C++

## Overview
This project focuses on implementing an HTTP server in C++ to facilitate communication between clients (web browsers or web crawlers) and a web server. The HTTP protocol is fundamental for distributed, collaborative, hypermedia information systems, forming the backbone of data communication for the World Wide Web.

## Key Features
- **Web Server Functions:** The primary role of the server is to store, process, and deliver web pages to clients using the HTTP protocol.
- **Resource Delivery:** Communication between the client and server occurs through HTTP, with the server responding by delivering the content of requested resources (typically HTML documents, images, style sheets, and scripts).
- **High-Traffic Website Support:** The system supports multiple web servers to handle high-traffic websites efficiently.

## Chapter II: General Rules
1. **Stability:** The program should not crash under any circumstances, including running out of memory. Unexpected quitting is not acceptable.
2. **Makefile:** Submission must include a Makefile for compiling source files without relinking.
3. **C++ 98 Standard:** Code must comply with the C++ 98 standard and remain compilable with the flag -std=c++98.

## Chapter III: Mandatory part - Webserv
Develop a HTTP server in C++ 98. Some requirements include:

- Take a configuration file as an argument or use a default path.
- The server must never block, and the client can be bounced properly if necessary.
- Use non-blocking I/O operations with a single `poll()` (or equivalent).
- Implement necessary HTTP methods (GET, POST, DELETE).
- Stress-test the server to ensure availability.
- Support multiple ports in the configuration file.

### III.1 Requirements
- A request to your server should never hang forever.
- The server must be compatible with a chosen web browser.
- HTTP response status codes must be accurate.
- Serve default error pages if none are provided.
- Clients must be able to upload files.

### III.3 Configuration file
- Configure server ports, hosts, and server names.
- Set up default error pages.
- Limit client body size.
- Define routes with accepted HTTP methods, redirections, directories, etc.

### Conclusion
This project revolves around creating a custom HTTP server in C++ 98, named "webserv." The server is designed to handle various HTTP protocols, allowing it to store, process, and deliver web pages to clients. The key requirements include non-blocking operations, compatibility with different web browsers, accurate HTTP response status codes, default error pages, support for serving static websites, and the ability to handle GET, POST, and DELETE methods.

The server configuration is defined in a configuration file, inspired by NGINX configuration. Users can specify ports, hosts, server names, default error pages, limit client body size, set up routes with specific rules (accepted HTTP methods, redirection, directory listing, etc.), and configure CGI execution for certain file extensions. The server must also handle uploaded files, ensuring proper CGI execution, and maintain resilience by never hanging indefinitely.