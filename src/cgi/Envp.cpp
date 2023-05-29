#include "cgi/Envp.hpp"

#include "http/Request.hpp"

namespace cgi {

void Envp::fillEnvp(std::string name, std::string val) {
    if (!name.empty() && !val.empty()) { m_envp[name] = val; }
}

Envp::Envp(http::Request const& request, http::Route const& route) {

    fillEnvp("CONTENT_LENGTH", request.getHeader("Content-Length"));
    fillEnvp("CONTENT_TYPE", request.getHeader("Content-Type"));
    fillEnvp("REQUEST_METHOD", request.getMethod());
    fillEnvp("SERVER_PROTOCOL", request.getVersion());
    fillEnvp("QUERY_STRING", request.getQuery());
    fillEnvp("USER_AGENT", request.getHeader("User-Agent"));
    fillEnvp("SERVER_PORT", request.getPort());

    // TODO: check this tommorrow
    fillEnvp("PATH_TRANSLATED", route.route(request.getPath()));
    fillEnvp("DOCUMENT_ROOT", ft::file(route.getRoot()).getDirectory());

    // TODO: check this tommorrow
    std::vector<std::string> subs = splitInfoFromPath(request.getPath());
    fillEnvp("PATH_INFO", subs.at(1));
    fillEnvp("SCRIPT_NAME", subs.at(0));
}

Envp::~Envp(void) {}

std::string Envp::get(std::string const& key) const { return (m_envp.at(key)); }

std::vector<std::string>
    Envp::splitInfoFromPath(std::string const& path) const {
    std::vector<std::string> subs;
    return (subs);
}

} // namespace cgi
