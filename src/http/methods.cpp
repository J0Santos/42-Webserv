#include "http/methods.hpp"

namespace http {

Method convertMethod(std::string const& methodStr) {
    if (methodStr == "GET") { return (GET); }
    if (methodStr == "HEAD") { return (HEAD); }
    if (methodStr == "POST") { return (POST); }
    if (methodStr == "PUT") { return (PUT); }
    if (methodStr == "DELETE") { return (DELETE); }
    if (methodStr == "CONNECT") { return (CONNECT); }
    if (methodStr == "OPTIONS") { return (OPTIONS); }
    if (methodStr == "TRACE") { return (TRACE); }
    if (methodStr == "PATCH") { return (PATCH); }
    if (methodStr == "INVALID") { return (INVALID); }
    return (UNKNOWN_METHOD);
}

Version convertVersion(std::string const& versionStr) {
    if (versionStr == "HTTP/1.1") { return (HTTP_1_1); }
    return (UNKNOWN_VERSION);
}

} // namespace http
