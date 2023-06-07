#include "server/Middleware.hpp"

#include "config/Options.hpp"
#include "http/Handler.hpp"
#include "http/Request.hpp"
#include "http/RequestBuffer.hpp"
#include "http/Response.hpp"
#include "sockets/ServerSocket.hpp"

namespace webserv {

int Middleware::handleRecv(smt::shared_ptr<net::ServerSocket> sock, int fd) {
    smt::shared_ptr<config::Opts> opts(new config::Opts());

    // receiving request string
    std::string reqStr = sock->recv(fd);

    int status = 0;
    reqStr = http::RequestBuffer::getNextRequest(fd, reqStr);
    while (!reqStr.empty()) {

        smt::shared_ptr<http::Request> request;
        try {
            request = smt::make_shared(new http::Request(reqStr));
        }
        catch (http::Request::MalformedRequestException const&) {
            LOG_E("Malformed request");
            status = 404;
        }

        try {
            opts = getOptions(sock, request);
        }
        catch (config::Options::NoSuchOptionsException const&) {
            LOG_E("No config found for this request");
            status = 500;
        }

        // handle request
        smt::shared_ptr<http::Response> response =
            http::processRequest(status, request, opts);

        // send response
        sock->send(fd, response->toString());

        // checking status
        if (status) { break; }

        // getting next request
        reqStr = http::RequestBuffer::getNextRequest(fd);
    }
    return (status);
}

smt::shared_ptr<config::Opts>
    Middleware::getOptions(smt::shared_ptr<net::ServerSocket> sock,
                           smt::shared_ptr<http::Request>     request) {

    std::stringstream ss;
    ss << sock->getPort();
    return (config::Options::getOptions(ss.str(), sock->getHost(),
                                        request->getPath(),
                                        request->getHeader("Host")));
}

} // namespace webserv