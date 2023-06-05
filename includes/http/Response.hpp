#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "http/ErrorCode.hpp"

#include <map>
#include <string>

namespace http {

#define SP   " "
#define CRLF "\r\n"

class Response {

    public:

        Response(void);
        Response(Response const& src);
        Response(int code, std::map<std::string, std::string> headers,
                 std::string body = "", std::string version = "HTTP/1.1");

        ~Response(void);

        Response& operator=(Response const& rhs);

        int                                getCode(void) const;
        std::map<std::string, std::string> getHeaders(void) const;
        std::string getHeader(std::string const& key) const;
        std::string getBody(void) const;
        std::string getVersion(void) const;
        std::string getReason(void) const;

        std::string const toString(void) const;

    private:

        ErrorCode                          m_code;
        std::map<std::string, std::string> m_header;
        std::string                        m_body;
        std::string                        m_version;
};

} // namespace http

#endif /* HTTP_RESPONSE_HPP */