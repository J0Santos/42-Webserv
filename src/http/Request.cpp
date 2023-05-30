#include "http/Request.hpp"

#include "utils/Logger.hpp"

namespace http {

Request::Request(std::string const& reqStr) : m_reqStr(reqStr) {
    std::vector<std::string> subs;

    size_t endPos = reqStr.find("\r\n\r\n");
    if (endPos == std::string::npos) { throw(MalformedRequestException()); }

    std::istringstream iss(reqStr);
    std::string        startLine;
    getline(iss, startLine); // getting first line

    // handle headers
    std::string line;
    while (getline(iss, line) && line != "\r") {
        subs = ft::string::split(line, ":\r");
        if (subs.size() != 2) { throw(MalformedRequestException()); }
        subs[1] = (subs[1][0] == ' ') ? subs[1].substr(1) : subs[1];
        m_headers[subs[0]] = subs[1];
    }

    // handle start line
    if (startLine.empty()) { throw(MalformedRequestException()); }
    // getting startLine without \r
    if (startLine.find("\r") != startLine.size() - 1) {
        throw(MalformedRequestException());
    }
    startLine = startLine.substr(0, startLine.size() - 1);

    subs = ft::string::split(startLine, " ");
    if (subs.size() != 3) { throw(MalformedRequestException()); }
    if (convertMethod(subs[0]) == UNKNOWN_METHOD) {
        throw(MalformedRequestException());
    }
    m_method = subs[0];
    m_uri = smt::make_shared(new Uri(subs[1]));
    if (convertVersion(subs[2]) == UNKNOWN_VERSION) {
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
        if (body.size() != len) { throw(MalformedRequestException()); }
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
