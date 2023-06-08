#include "http/methods.hpp"

#include "config/Options.hpp"
#include "http/Handler.hpp"
#include "http/MimeType.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
#include "utils/smt.hpp"

#include <dirent.h>
#include <sstream>
#include <unistd.h>

namespace http {
namespace methods {

smt::shared_ptr<http::Response>
    generateAutoIndexResponse(smt::shared_ptr<http::Request> const req) {

    smt::shared_ptr<http::Response> resp;

    // Get the list of files in the directory
    std::string routedPath = req->routeRequest();
    std::string path = req->getPath();

    std::stringstream ss;
    ss << "<html><head><title>Index of " << path << "</title></head><body>"
       << "<h1>Index of " << path << "</h1>"
       << "<ul>";

    dirent* entry;
    DIR*    dir = opendir(routedPath.c_str());
    while ((entry = readdir(dir)) != NULL) {
        std::string fileName = entry->d_name;
        if (fileName != "." && fileName != "..") {
            ss << "<li><a href=\"" << std::string(ft::directory(path))
               << fileName << "\">" << fileName << "</a></li>";
        }
    }
    closedir(dir);

    ss << "</ul>";
    ss << "</body></html>";

    // creating body
    std::string body = ss.str();

    // creating headers
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "text/html";

    resp = smt::make_shared(new http::Response(200, headers, body));
    return (resp);
}

smt::shared_ptr<http::Response>
    GET(smt::shared_ptr<http::Request> const request,
        smt::shared_ptr<config::Opts> const  opts) {

    smt::shared_ptr<http::Response> resp;

    // check if file/directory is valid
    std::string file = request->routeRequest();
    if (!ft::file(file).isValid() && !ft::directory(file).isValid()) {
        return (generateErrorResponse(404, opts));
    }

    // checking if its a directory
    if (ft::file(file).isDirectory()) {
        // check for index and autoindex
        if (!std::string(opts->m_index).empty()) {
            file = opts->m_index;
            if (!ft::file(file).isValid()) {
                // TODO: check if status code is correct
                return (generateErrorResponse(404, opts));
            }
        }
        else if (opts->m_autoindex) {
            return (generateAutoIndexResponse(request));
        }
        else {
            // TODO: check if status code is correct
            return (generateErrorResponse(403, opts));
        }
    }

    // TODO: check if its a cgi file
    // checking if its a regular file
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = http::MimeType(file);
    std::string body = ft::file(file).read();

    // return response
    resp = smt::make_shared(new http::Response(200, headers, body));
    return (resp);
}

smt::shared_ptr<http::Response>
    POST(smt::shared_ptr<http::Request> const request,
         smt::shared_ptr<config::Opts> const  opts) {

    smt::shared_ptr<http::Response> resp;

    std::string file = request->routeRequest();

    // checking if directory of file is valid
    ft::directory fileDir = ft::file(file).getDirectory();
    if (!fileDir.isValid()) { return (generateErrorResponse(404, opts)); }

    // checking if file exists
    if (ft::file(request->getPath()).isCrawler()) {
        return (generateErrorResponse(404, opts));
    }

    // checking if file exists
    if (!ft::file(file).exists()) {
        std::ofstream f(file.c_str(), std::ios_base::app);
        if (!f.good()) { return (generateErrorResponse(403, opts)); }
        f << request->getBody();
        f.close();
    }
    else {
        std::ofstream f(file.c_str(), std::ofstream::out | std::ofstream::app);
        if (!f.good()) { return (generateErrorResponse(403, opts)); }
        f << request->getBody();
        f.close();
    }

    std::map<std::string, std::string> headers;
    resp = smt::make_shared(new http::Response(201, headers));

    return (resp);
}

smt::shared_ptr<http::Response>
    DELETE(smt::shared_ptr<http::Request> const request,
           smt::shared_ptr<config::Opts> const  opts) {
    smt::shared_ptr<http::Response> resp;

    // checking if file is valid
    std::string file = request->routeRequest();
    if (ft::file(file).isDirectory()) {
        return (generateErrorResponse(403, opts));
    }
    if (!ft::file(file).isValid()) {
        return (generateErrorResponse(404, opts));
    }

    // Delete the file
    if (unlink(file.c_str()) == -1) { generateErrorResponse(403, opts); }

    // return response
    std::map<std::string, std::string> headers;
    resp = smt::make_shared(new http::Response(204, headers));
    return (resp);
}

} // namespace methods

MethodType convertMethod(std::string const& methodStr) {
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
