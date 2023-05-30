import sys
import os
import urllib.parse

def test_post_request():
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    post_data = sys.stdin.read(content_length)

    query_string = os.environ.get('QUERY_STRING', '')
    query_params = urllib.parse.parse_qs(query_string)

    response = f"POST Data: {post_data}\n"
    response += "Query Parameters:\n"
    for name, values in query_params.items():
        response += f"{name}: {', '.join(values)}\n"

    # Prepare the response headers
    response_headers = [
        ('Content-Type', 'text/plain'),
        ('Content-Length', str(len(response)))
    ]

    # Send the response headers
    for header in response_headers:
        print(f'{header[0]}: {header[1]}')

    # Send the blank line to indicate the end of the headers
    print()

    # Send the response content
    print(response)

# Entry point of the script
if __name__ == '__main__':
    if os.environ.get('REQUEST_METHOD') == 'POST':
        test_post_request()