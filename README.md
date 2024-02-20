# HTTP Server Implementation README 🌐

This project aims to implement an **HTTP server** based on the *Hypertext Transfer Protocol (HTTP)* specifications, focusing on serving static websites, handling dynamic content through *CGI* (Common Gateway Interface), and supporting various **HTTP methods** like **GET**, **POST**, and **DELETE**.

## Introduction 🚀

The *Hypertext Transfer Protocol (HTTP)* is fundamental for distributed, collaborative, hypermedia information systems, forming the backbone of communication for the World Wide Web. HTTP facilitates the transfer of hypertext documents, allowing users to access resources through hyperlinks via web browsers.

## Requirements 📋

- **Configuration File Handling**: The program should accept a configuration file as an argument or use a default path.
- **Non-blocking I/O**: The server must never block, using only one \`poll()\` (or equivalent) for all I/O operations.
- **Compatibility**: The server should be compatible with web browsers and adhere to HTTP standards (e.g., *HTTP 1.1* compliance similar to **NGINX**).
- **Error Handling**: Proper *HTTP response status codes* and default error pages must be implemented.
- **Static Website Serving**: The server must be able to serve a fully static website.
- **HTTP Methods**: Support for at least **GET**, **POST**, and **DELETE** methods is required.
- **File Upload**: Clients should be able to upload files.
- **Configuration Flexibility**: The configuration file should allow for setting up servers, defining routes, limiting client body size, etc.
- **CGI Execution**: Execute *CGI scripts* based on certain file extensions (e.g., \`.php\`).
- **Resilience**: The server should remain available under stress and should not hang forever on requests.

## Configuration File ⚙️

The configuration file should resemble **NGINX's** configuration format, allowing users to specify server settings, routes, error pages, etc. Some key features of the configuration file include:

- Choosing port and host for each server.
- Defining server names and default servers.
- Setting up default error pages.
- Limiting client body size.
- Configuring routes with accepted HTTP methods, redirections, file directories, CGI execution, etc.

## Conclusion 🎉

This **HTTP server implementation** aims to provide a resilient, feature-rich solution for serving web content while adhering to HTTP standards and ensuring compatibility across different platforms and browsers.

# Setting Up the Project with Makefile

## 1. Clone the Repository 📁

- git clone <repository_url>

## 2. Set Up the Project 🚀

- Make sure you have a C++ compiler installed (preferably \`g++\`).
- Navigate to the project directory.

## 3. Makefile Usage 🛠️

- **\`make all\`**: Compiles the project.
- **\`make clean\`**: Removes all object files.
- **\`make fclean\`**: Removes all object files and the executable.
- **\`make re\`**: Performs a clean build (removes all object files and recompiles).

## 4. Compilation 🖥️

- Run \`make all\` to compile the project.
- The compiled executable will be named \`webserv\`.

## 5. Cleaning 🧹

- Run \`make clean\` to remove all object files.
- Run \`make fclean\` to remove all object files and the compiled executable.

## 6. Rebuilding 🔄

- Run \`make re\` to perform a clean build (removes all object files and recompiles the project).

## 7. Customization 🛠️

- Adjust \`CFLAGS\` in the Makefile for any specific compilation flags you require.
- Modify source files (\`*.cpp\`) and header files (\`*.hpp\`) in the \`src/\` and \`include/\` directories respectively to customize the project.

## 8. Compilation Output 💻

- The compiled executable \`webserv\` will be created in the project directory upon successful compilation.
