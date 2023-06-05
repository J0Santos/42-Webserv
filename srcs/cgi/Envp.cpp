#include "cgi/Envp.hpp"

#include "http/Request.hpp"
#include "utils/ft_array.hpp"

namespace cgi {

Envp::Envp(void) {}

Envp::Envp(http::Request const& request) {
    fillEnvp("REQUEST_METHOD", request.getMethod());
    fillEnvp("SCRIPT_NAME", request.getPath());
    fillEnvp("QUERY_STRING", request.getQuery());
    fillEnvp("CONTENT_LENGTH", request.getHeader("Content-Length"));
    fillEnvp("CONTENT_TYPE", request.getHeader("Content-Type"));
}

Envp::Envp(Envp const& src) { *this = src; }

Envp::~Envp(void) {}

Envp& Envp::operator=(Envp const& rhs) {
    if (this != &rhs) { m_argc = rhs.m_argc; }
    return (*this);
}

Envp::operator char**(void) const { return (ft::array(m_argc)); }

void Envp::fillEnvp(std::string const& key, std::string const& value) {
    if (!value.empty()) { m_argc.push_back(key + "=" + value); }
}

} // namespace cgi
