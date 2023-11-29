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
3. **Makefile Rules:** The Makefile must contain the rules $(NAME), all, clean, fclean, and re.
4. **Compilation Flags:** Code should be compiled with c++ using the flags -Wall -Wextra -Werror.
5. **C++ 98 Standard:** Code must comply with the C++ 98 standard and remain compilable with the flag -std=c++98.
6. **Development Practices:** Use C++ features whenever possible; prefer C++ versions over C functions. External libraries and Boost libraries are prohibited.

## Conclusion
This HTTP server implementation adheres to strict coding standards, emphasizing stability, efficient resource delivery, and compliance with the C++ 98 standard. The Makefile ensures easy compilation, and development practices encourage the use of C++ features while avoiding external libraries and Boost libraries.