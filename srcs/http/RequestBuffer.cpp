#include "http/RequestBuffer.hpp"

#include "utils/ft_string.hpp"
#include "utils/Logger.hpp"

namespace http {

RequestBuffer::RequestBuffer(void) {}

RequestBuffer::RequestBuffer(RequestBuffer const& src) { *this = src; }

RequestBuffer::~RequestBuffer(void) {}

RequestBuffer& RequestBuffer::operator=(RequestBuffer const& rhs) {
    if (this != &rhs) { m_buffers = rhs.m_buffers; }
    return (*this);
}

RequestBuffer& RequestBuffer::getInstance(void) {
    static RequestBuffer ist;
    return (ist);
}

std::string RequestBuffer::getNextRequest(int fd, std::string reqStr) {
    // getting buffer for this fd
    std::string buff = reqStr;

    RequestBuffer& rb = getInstance();
    if (rb.m_buffers.find(fd) != rb.m_buffers.end()) {
        buff = rb.m_buffers[fd];
        buff += reqStr;
        if (buff.empty()) {
            rb.m_buffers.erase(fd);
            return ("");
        }
    }
    else { rb.m_buffers.insert(std::make_pair(fd, buff)); }

    std::string ret;

    size_t endHeaders = buff.find("\r\n\r\n");
    // Request is incomplete
    if (endHeaders == std::string::npos) { return (""); }

    // getting request until the end of headers
    ret = buff.substr(0, endHeaders + 4);

    // getting Content-Length
    size_t startPos = ret.find("Content-Length: ");
    if (startPos != std::string::npos) {
        size_t endPos = ret.find("\r\n", startPos);
        // getting Content-Length
        int len = ft::string::stoi(buff.substr(startPos + 16, endPos));

        // checking if body is complete
        if (buff.size() < endHeaders + 4 + len) { return (""); }

        // adding body to ret
        if (len) { ret += buff.substr(endHeaders + 4, len); }
    }

    buff = buff.substr(ret.size());

    // updating buffer for this fd
    rb.m_buffers[fd] = buff;

    return (ret);
}

bool RequestBuffer::hasRequest(int fd) {
    return (getInstance().m_buffers.find(fd) != getInstance().m_buffers.end());
}

} // namespace http
