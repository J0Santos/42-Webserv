#ifndef HTTP_METHODS_HPP
#define HTTP_METHODS_HPP

#include <string>

namespace http {

typedef enum {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
    INVALID,
    UNKNOWN_METHOD,
} Method;

typedef enum {
    HTTP_1_1,
    UNKNOWN_VERSION,
} Version;

Method convertMethod(std::string const& methodStr);

Version convertVersion(std::string const& versionStr);

} // namespace http

#endif /* HTTP_METHODS_HPP */