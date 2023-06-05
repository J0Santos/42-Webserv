#include "http/Response.hpp"

#include <sstream>

namespace http {

Response::Response(Response const& src) { *this = src; }

Response::Response(int code, std::map<std::string, std::string> headers,
                   std::string body, std::string version)
    : m_code(code), m_header(headers), m_body(body), m_version(version) {

    if (m_header.find("Content-Length") == m_header.end()) {
        std::stringstream ss;
        ss << m_body.size();
        m_header["Content-Length"] = ss.str();
    }
    if (m_header.find("Content-Type") == m_header.end()) {
        m_header["Content-Type"] = "text/plain";
    }
}

Response::~Response(void) {}

Response& Response::operator=(Response const& rhs) {
    if (this != &rhs) {
        m_code = rhs.m_code;
        m_header = rhs.m_header;
        m_body = rhs.m_body;
    }
    return (*this);
}

int Response::getCode(void) const { return (m_code); }

std::map<std::string, std::string> Response::getHeaders(void) const {
    return (m_header);
}

std::string Response::getHeader(std::string const& key) const {
    return (m_header.at(key));
}

std::string Response::getBody(void) const { return (m_body); }

std::string Response::getVersion(void) const { return (m_version); }

std::string Response::getReason(void) const { return (m_code); }

std::string const Response::toString(void) const {

    // converting m_code to string
    std::stringstream ss;
    ss << int(m_code);
    std::string code = ss.str();

    // converting m_header to string
    std::string headers;
    for (std::map<std::string, std::string>::const_iterator it =
             m_header.begin();
         it != m_header.end(); it++)
        headers += (*it).first + ":" + SP + (*it).second + CRLF;

    return (m_version + SP + code + SP + std::string(m_code) + CRLF + headers +
            CRLF + m_body);
}

} // namespace http
