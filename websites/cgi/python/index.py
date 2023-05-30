#!/usr/bin/env python
print("HTTP/1.0 200 OK")
print("Content-Type: text/html")
print("Content-Length: 138")
print()  # Print an extra newline to separate headers from the body

print("<html>")
print("<head>")
print("<title>CGI Example</title>")
print("</head>")
print("<body>")
print("<h1>CGI Example</h1>")
print("<p>This is a simple CGI script written in Python.</p>")
print("</body>")
print("</html>")