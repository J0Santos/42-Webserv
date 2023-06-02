#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include "config/Options.hpp"
#include "http/MimeType.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
#include "sockets/ServerSocket.hpp"
#include "utils/Logger.hpp"

#include <string>

namespace config {
class Opts;
}

namespace net {
class ServerSocket;
}

namespace http {

void handle(smt::shared_ptr<net::ServerSocket>, int connectFd);

smt::shared_ptr<Response>
    generateErrorResponse(int code, smt::shared_ptr<config::Opts> const opts);

} // namespace http

#endif /* HTTP_HANDLER_HPP */