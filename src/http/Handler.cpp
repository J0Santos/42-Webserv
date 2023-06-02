#include "http/Handler.hpp"

namespace http {

void handle(smt::shared_ptr<net::ServerSocket> sock, int connectFd) {
    smt::shared_ptr<config::Opts> opts;

    // receiving request string
    std::string reqStr = sock->recv(connectFd);
    // LOG_D("Received request: " << reqStr);

    // TODO: Handle GetNextRequest
    smt::shared_ptr<Request> request;
    try {
        request = smt::make_shared(new Request(reqStr));
        LOG_D(std::endl << request->toString());
    }
    catch (Request::MalformedRequestException const&) {
        LOG_E("Malformed request");
        generateErrorResponse(400, opts);
    }

    opts = config::getOptions(sock, request);
    if (!opts) {
        LOG_E("No config found for this request");
        smt::shared_ptr<Response> response = generateErrorResponse(500, opts);
        sock->send(connectFd, response->toString());
        return;
    }
    std::string respStr = generateErrorResponse(405, opts)->toString();
    LOG_D("Sending response: " << std::endl << respStr);
    sock->send(connectFd, respStr);
}

smt::shared_ptr<Response>
    generateErrorResponse(int code, smt::shared_ptr<config::Opts> const opts) {

    // TODO: protect this when opts is null
    std::string                        body;
    std::map<std::string, std::string> header;
    std::stringstream                  ss;

    // getting custom body
    bool default_body = false;
    if (opts->m_error_pages.find(code) != opts->m_error_pages.end()) {
        std::string   filename = std::string(opts->m_error_pages[code]);
        std::ifstream file(filename.c_str());
        if (!file.good()) {
            LOG_I("Fail to open " << filename);
            default_body = true;
        }
        else {
            body = std::string((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
            header["Content-Type"] = http::MimeType(filename);
        }
        file.close();
    }
    else { default_body = true; }

    // getting default body
    if (default_body) {

        // converting code to str
        ss << code;
        std::string error_code = ss.str();

        // generating message
        std::string msg =
            error_code + " - " + std::string(http::ErrorCode(code));
        body = "<!DOCTYPE html>"
               "<html>"
               "  <head>"
               "    <title>" +
               msg +
               "</title>"
               "  </head>"
               "  <body>"
               "    <h1>" +
               msg +
               "</h1>"
               "  </body>"
               "</html>";
        header["Content-Type"] = "text/html";
    }

    return (smt::shared_ptr<Response>(new Response(code, header, body)));
}

} // namespace http