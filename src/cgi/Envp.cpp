#include "cgi/Envp.hpp"

#include "cgi/Cgi.hpp"
#include "http/Request.hpp"

namespace cgi {

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
    fillEnvp("SCRIPT_NAME", subs.at(0));
    fillEnvp("PATH_INFO", subs.at(1));
}

Envp::Envp(Envp const& src) { *this = src; }

Envp::~Envp(void) {}

Envp& Envp::operator=(Envp const& rhs) {
    if (this != &rhs) { m_envp = rhs.m_envp; }
    return (*this);
}

bool Envp::operator==(Envp const& rhs) const { return (m_envp == rhs.m_envp); }

void Envp::fillEnvp(std::string name, std::string val) {
    if (!name.empty() && !val.empty()) { m_envp[name] = val; }
}

std::string Envp::get(std::string const& key) const {
    if (m_envp.find(key) == m_envp.end()) { return (""); }
    return (m_envp.at(key));
}

std::map<std::string, std::string> Envp::getMapEnvp(void) const {
    return (m_envp);
}

} // namespace cgi
