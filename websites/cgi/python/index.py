#!/usr/bin/env python

import os

def generate_autoindex(directory):
    # Get a list of files and directories in the given directory
    entries = os.listdir(directory)

    # Sort the entries alphabetically
    entries.sort()

    # Generate the HTML for the autoindex page
    html = '<html><head><title>Autoindex</title></head><body>'
    html += '<h1>Index of {0}</h1>'.format(directory)
    html += '<ul>'

    # Add an entry for each file or directory
    for entry in entries:
        # Get the full path of the entry
        entry_path = os.path.join(directory, entry)

        # Determine if the entry is a file or directory
        if os.path.isfile(entry_path):
            # Add a link for files
            html += '<li><a href="{0}">{1}</a></li>'.format(entry, entry)
        elif os.path.isdir(entry_path):
            # Add a link for directories
            html += '<li><a href="{0}/">{1}/</a></li>'.format(entry, entry)

    html += '</ul></body></html>'

    return html

# Specify the directory for which to generate the autoindex
directory_path = '/path/to/directory/'

# Generate the autoindex HTML for the specified directory
autoindex_html = generate_autoindex(directory_path)

# Print the autoindex HTML as the CGI response
print('Content-Type: text/html')
print()
print(autoindex_html)