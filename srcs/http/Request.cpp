#include "http/Request.hpp"

#include "utils/Logger.hpp"

namespace http {

Request::Request(std::string const& reqStr) : m_reqStr(reqStr) {
    std::vector<std::string> subs;

    size_t endPos = reqStr.find("\r\n\r\n");
    if (endPos == std::string::npos) {
        LOG_W("Malformed request: failed to find end of headers");
        throw(MalformedRequestException());
    }

    std::istringstream iss(reqStr);
    std::string        startLine;
    getline(iss, startLine); // getting first line

    // handle headers
    std::string line;
    while (getline(iss, line) && line != "\r") {
        if (line.find("\r") != line.size() - 1) {
            LOG_W("Malformed request: invalid header end");
            throw(MalformedRequestException());
        }
        line = line.substr(0, line.size() - 1);
        size_t pos = line.find(": ");
        if (pos == std::string::npos || !pos || pos == line.size() - 2) {
            LOG_W("Malformed request: failed to parse headers");
            throw(MalformedRequestException());
        }
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 2);
        m_headers[key] = value;
    }

    // handle start line
    if (startLine.empty()) {
        LOG_W("Malformed request: failed to find start line");
        throw(MalformedRequestException());
    }
    // getting startLine without \r
    if (startLine.find("\r") != startLine.size() - 1) {
        LOG_W("Malformed request: invalid start line end");
        throw(MalformedRequestException());
    }
    startLine = startLine.substr(0, startLine.size() - 1);

    subs = ft::string::split(startLine, " ");
    if (subs.size() != 3) {
        LOG_W("Malformed request: invalid start line number of arguments");
        throw(MalformedRequestException());
    }
    if (convertMethod(subs[0]) == UNKNOWN_METHOD) {
        LOG_W("Malformed request: unknown method");
        throw(MalformedRequestException());
    }
    m_method = subs[0];
    m_uri = smt::make_shared(new Uri(subs[1]));
    if (convertVersion(subs[2]) == UNKNOWN_VERSION) {
        LOG_W("Malformed request: unknown version");
        throw(MalformedRequestException());
    }
    m_version = subs[2];

    // handle body
    std::string body = reqStr.substr(endPos + 4);
    // convert Content-Length to int
    if (m_headers.find("Content-Length") != m_headers.end()) {
        size_t            len;
        std::stringstream ss(m_headers["Content-Length"]);
        ss >> len;
        if (body.size() != len) {
            LOG_W("Malformed request: invalid body size");
            throw(MalformedRequestException());
        }
        m_body = body;
    }
    // if (reqStr.size() > endPos + 4) { m_body = reqStr.substr(endPos + 4); }
}

Request::Request(Request const& src) { *this = src; }

Request::~Request(void) {}

Request& Request::operator=(Request const& rhs) {
    if (this != &rhs) {
        m_method = rhs.m_method;
        m_version = rhs.m_version;
        m_headers = rhs.m_headers;
        m_body = rhs.m_body;
        m_uri = rhs.m_uri;
        m_reqStr = rhs.m_reqStr;
    }
    return (*this);
}

std::string const& Request::getMethod(void) const { return (m_method); }

std::string const& Request::getVersion(void) const { return (m_version); }

std::string const Request::getHeader(std::string const& key) const {
    if (m_headers.find(key) == m_headers.end()) { return (""); }
    return (m_headers.at(key));
}

std::string const& Request::getBody(void) const { return (m_body); }

std::string Request::getScheme(void) const { return (m_uri->getScheme()); }

std::string Request::getHost(void) const { return (m_uri->getHost()); }

std::string Request::getPort(void) const { return (m_uri->getPort()); }

ft::file Request::getPath(void) const { return (m_uri->getPath()); }

std::string Request::getQuery(void) const { return (m_uri->getQuery()); }

std::string const& Request::toString(void) const { return (m_reqStr); }

char const* Request::MalformedRequestException::what(void) const throw() {
    return ("Request: malformed request.");
}

std::string getRequest(std::string const& reqStr) {
    static std::string buffer;
    std::string        ret;

    if (!reqStr.empty()) { buffer = reqStr; }

    // getting request until the end of headers
    size_t endHeaders = buffer.find("\r\n\r\n");
    if (endHeaders == std::string::npos) {
        ret = buffer;
        buffer.clear();
        return (ret);
    }
    ret = buffer.substr(0, endHeaders + 4);

    // getting content-length
    size_t pos = ret.find("Content-Length: ");
    if (pos != std::string::npos) {
        // getting Content-Length
        std::string        l;
        std::istringstream iss(buffer.substr(pos + 16));
        iss >> l;

        // converting to int
        int               len;
        std::stringstream ss(l);
        ss >> len;

        // adding body to ret
        if (len > 0) { ret += buffer.substr(endHeaders + 4, len); }
    }

    buffer = buffer.substr(ret.size());
    return (ret);
}

} // namespace http
