#include "http/Handler.hpp"

#include "http/MimeType.hpp"
#include "utils/Logger.hpp"

namespace http {

smt::shared_ptr<Response>
    processRequest(int status, smt::shared_ptr<Request> const request,
                   smt::shared_ptr<config::Opts> const opts) {
    (void)request;
    if (status) { return (generateErrorResponse(status, opts)); }
    // check if method is allowed
    // TODO: change m_allowed_methods to a set
    // if (opts->m_allowed_methods.find(request->getMethod()) ==
    //     opts->m_allowed_methods.end()) {
    //     return (generateErrorResponse(405, opts));
    // }

    // TODO: check if request is cgi request

    // handle request
    smt::shared_ptr<Response> response;
    return (response);
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