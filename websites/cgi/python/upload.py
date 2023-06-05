#!/usr/bin/env python

import cgi
import os

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get the file data from the field named 'file'
fileitem = form['file']

# Check if a file was uploaded
if fileitem.filename:
    # Set the destination directory to store the uploaded file
    upload_dir = '/path/to/upload/directory/'

    # Create the directory if it doesn't exist
    if not os.path.exists(upload_dir):
        os.makedirs(upload_dir)

    # Get the filename
    filename = os.path.basename(fileitem.filename)

    # Set the file path
    filepath = os.path.join(upload_dir, filename)

    # Save the file to the server
    with open(filepath, 'wb') as file:
        file.write(fileitem.file.read())

    # Print a success message
    print('Content-Type: text/html')
    print()
    print('<html><body>')
    print('<h2>File uploaded successfully.</h2>')
    print(f'<p>Filename: {filename}</p>')
    print('</body></html>')
else:
    print('Content-Type: text/html')
    print()
    print('<html><body>')
    print('<h2>No file uploaded.</h2>')
    print('</body></html>')