#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "http/methods.hpp"
#include "http/Uri.hpp"
#include "utils/smt.hpp"

#include <map>
#include <string>

namespace http {

#define SP   " "
#define CRLF "\r\n"

class Request {
    public:

        Request(std::string const& reqStr);
        Request(Request const& src);

        ~Request(void);

        Request& operator=(Request const& rhs);

        MethodType const&      getMethod(void) const;
        Version const&     getVersion(void) const;
        std::string const& getHeader(std::string const& key) const;
        std::string const& getBody(void) const;

        std::string getScheme(void) const;
        std::string getHost(void) const;
        std::string getPort(void) const;
        ft::file    getPath(void) const;
        std::string getQuery(void) const;

        std::string const& toString(void) const;

        struct MalformedRequestException : public std::exception {
                char const* what(void) const throw();
        };

    private:

        MethodType                             m_method;
        Version                            m_version;
        std::map<std::string, std::string> m_headers;
        std::string                        m_body;
        smt::shared_ptr<Uri>               m_uri;

        std::string m_reqStr;
};

std::string getRequest(std::string const& reqStr = "");

} // namespace http

#endif /* HTTP_REQUEST_HPP */