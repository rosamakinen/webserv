HTTP Server 🌐

This project aims to implement an HTTP server based on the Hypertext Transfer Protocol (HTTP) specifications, focusing on serving static websites, handling dynamic content through CGI (Common Gateway Interface), and supporting various HTTP methods like GET, POST, and DELETE.

Introduction 🚀

The Hypertext Transfer Protocol (HTTP) is fundamental for distributed, collaborative, hypermedia information systems, forming the backbone of communication for the World Wide Web. HTTP facilitates the transfer of hypertext documents, allowing users to access resources through hyperlinks via web browsers.

Requirements 📋

Configuration File Handling: The program should accept a configuration file as an argument or use a default path.

Non-blocking I/O: The server must never block, using only one poll() (or equivalent) for all I/O operations.

Compatibility: The server should be compatible with web browsers and adhere to HTTP standards (e.g., HTTP 1.1 compliance similar to NGINX).

Error Handling: Proper HTTP response status codes and default error pages must be implemented.

Static Website Serving: The server must be able to serve a fully static website.

HTTP Methods: Support for at least GET, POST, and DELETE methods is required.

File Upload: Clients should be able to upload files.

Configuration Flexibility: The configuration file should allow for setting up servers, defining routes, limiting client body size, etc.

CGI Execution: Execute CGI scripts based on certain file extensions (e.g., .php).

Resilience: The server should remain available under stress and should not hang forever on requests.


Configuration File ⚙️

The configuration file should resemble NGINX's configuration format, allowing users to specify server settings, routes, error pages, etc. Some key features of the configuration file include:

Choosing port and host for each server.
Defining server names and default servers.
Setting up default error pages.
Limiting client body size.
Configuring routes with accepted HTTP methods, redirections, file directories, CGI execution, etc.
Conclusion 🎉
This HTTP server implementation aims to provide a resilient, feature-rich solution for serving web content while adhering to HTTP standards and ensuring compatibility across different platforms and browsers. For any questions or concerns, please refer to the provided documentation or testing resources.
