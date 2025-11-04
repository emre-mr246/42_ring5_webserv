<div align="center">
  <img src="img/webserv.png" alt="Logo" width="150" height="150">
  <h2>42 Webserv Project</h2>
    <a href= https://github.com/emre-mr246/42-evaluation><img src="https://img.shields.io/badge/score-100%20%2F%20100-success?style=for-the-badge"/></a>
    <a href= https://github.com/emre-mr246/42-evaluation><img src="https://img.shields.io/badge/circle-5-magenta?style=for-the-badge"/></a>
    <a href="https://42istanbul.com.tr/"><img src="https://img.shields.io/badge/Lang-C++-blue?style=for-the-badge"/></a>
    <a href= https://github.com/emre-mr246/42-evaluation><img src="https://img.shields.io/badge/42-Evaluation-red?style=for-the-badge"/></a>
    <a href= https://github.com/emre-mr246/42_ring5_webserv><img src="https://img.shields.io/github/last-commit/emre-mr246/42_ring5_webserv?style=for-the-badge"/></a>
    <a href="https://42istanbul.com.tr/"><img src="https://img.shields.io/badge/42-ISTANBUL-white?style=for-the-badge"/></a>
   
<h4>
    <a href="https://github.com/emre-mr246/42_ring5_webserv/issues">❔ Ask a Question</a>
  <span> · </span>
    <a href="https://github.com/emre-mr246/42_ring5_webserv/issues">🪲 Report Bug</a>
  <span> · </span>
    <a href="https://github.com/emre-mr246/42_ring5_webserv/issues">💬 Request Feature</a>
</h4>
</div>


## Introduction 🚀

Webserv is a custom HTTP/1.1 web server implementation written in C++98. This project is part of the 42 curriculum and aims to build a fully functional web server capable of handling multiple connections, serving static content, processing CGI scripts, and managing various HTTP methods.

The server follows the HTTP/1.1 protocol specifications and supports features like virtual hosts, custom error pages, file uploads, and CGI execution.

## RaRe Server is a Group Project 🙅🏽‍♀️🙅🏽

In the 42 curriculum, there are various group projects that must be completed with a specified number of participants. This requirement helps us develop our collaboration skills in project settings, preparing us to adapt more effectively to future jobs.

Our project is named RaRe Server, derived from the last two letters of the names of the project creators, [MısRa](https://github.com/misratasci) and [EmRe](https://github.com/emre-mr246).

## Features 🔍

- **HTTP/1.1 Protocol Support**: Full implementation of GET, POST, PUT, and DELETE methods
- **Multiple Server Configurations**: Support for virtual hosts with different server names and ports
- **CGI Execution**: Process dynamic content through CGI scripts (Python, Perl, etc.)
- **File Upload/Download**: Handle file uploads with configurable size limits
- **Custom Error Pages**: Serve custom error pages for different HTTP status codes (400, 403, 404, 405, 413, 500, 504)
- **Location-based Routing**: Configure different behaviors for different URL paths
- **Poll-based I/O Multiplexing**: Efficient handling of multiple concurrent connections
- **Client Timeout Management**: Automatic cleanup of inactive connections
- **Chunked Transfer Encoding**: Support for chunked request/response bodies
- **MIME Type Detection**: Automatic content-type detection based on file extensions
- **Configuration File**: Nginx-like configuration syntax for easy server setup
- **C++98 Standard**: Fully compliant with C++98 standard

## Usage ⚙️

### Build and Run

1.  **Clone the repository:**
    ```bash
    git clone <repository_url> webserv
    cd webserv
    ```

2.  **Build the project:**
    ```bash
    make
    ```

3.  **Run the server:**
    ```bash
    ./webserv [config_file]
    ```
    If no config file is specified, the default configuration will be used.

4.  **Test the server:**
    Open your browser and navigate to:
    - `http://localhost:8080` - Hogwarts server (CGI enabled)
    - `http://localhost:8181` - Hisar server (static content)
    - `http://localhost:4242` - YoupiBanane test server

### Configuration File

The server uses an Nginx-like configuration syntax. Here's an example:

```nginx
server {
    listen 8080;
    server_name example.com;
    root /path/to/webroot;
    client_max_body_size 1048576;

    error_page 404 /error_pages/404.html;
    error_page 500 /error_pages/500.html;

    location / {
        index index.html;
        accepted_methods GET POST;
    }

    location /upload {
        accepted_methods POST DELETE;
        root /path/to/uploads;
        client_max_body_size 204800;
    }

    location /cgi {
        cgi_pass .py /usr/bin/python3;
        cgi_pass .pl /usr/bin/perl;
    }
}
```

### Configuration Directives

- `listen` - Port and/or IP address to bind to
- `server_name` - Virtual host names
- `root` - Document root directory
- `client_max_body_size` - Maximum request body size in bytes
- `error_page` - Custom error page paths
- `index` - Default index file
- `accepted_methods` - Allowed HTTP methods for the location
- `cgi_pass` - CGI script interpreter configuration

### Project Structure

```
webserv/
├── inc/                    # Header files
│   ├── webserv.hpp
│   └── http.hpp
├── src/                    # Source files
│   ├── config/            # Configuration parser
│   ├── http/              # HTTP request/response handling
│   ├── cgi/               # CGI execution
│   └── *.cpp              # Core server files
├── www/                    # Web content directories
│   ├── hogwarts/
│   ├── hisar/
│   └── YoupiBanane/
├── config/                 # Configuration files
│   └── config.rare
├── Makefile
└── README.md
```

### Testing

To test different features:

1. **Static file serving:**
   ```bash
   curl http://localhost:8181/
   ```

2. **CGI execution:**
   ```bash
   curl http://localhost:8080/
   ```

3. **File upload:**
   ```bash
   curl -X POST -F "file=@test.txt" http://localhost:8080/upload
   ```

4. **File deletion:**
   ```bash
   curl -X DELETE http://localhost:8080/upload/test.txt
   ```

### Clean Up

```bash
make clean   # Kill running webserv processes
make fclean  # Remove executable and kill processes
make re      # Rebuild from scratch
```
