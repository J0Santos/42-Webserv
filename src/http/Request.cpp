#include "http/Request.hpp"

#include "utils/Logger.hpp"

namespace http {

Request::Request(std::string const& reqStr) { (void)reqStr; }

Request::~Request(void) {}

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
