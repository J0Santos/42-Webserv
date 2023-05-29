#include "http/Uri.hpp"

#include "utils/Logger.hpp"

namespace http {

Uri::Uri(std::string const& uri) {

    std::string str = ft::string::trim(uri);

    size_t scheme_pos = std::string(str).find("://");
    if (scheme_pos != std::string::npos) {
        m_scheme = str.substr(0, scheme_pos);
        str = str.substr(scheme_pos + 3);
        scheme_pos += 3;
    }
    else { scheme_pos = 0; }

    std::vector<std::string> subs = ft::string::split(str, "/");
    if (subs.size() > 1) {
        std::vector<std::string> tmp = ft::string::split(subs[0], ":");
        m_host = tmp[0];
        if (tmp.size() > 1) { m_port = tmp[1]; }
        size_t host_pos = str.find("/");
        if (host_pos != std::string::npos) { str = str.substr(host_pos); }
        else { str = str.substr(scheme_pos); }
    }

    size_t path_pos = str.find("?");
    if (path_pos != std::string::npos) {

        m_path = str.substr(0, path_pos);
        m_query = str.substr(++path_pos);
    }
    else { m_path = str; }
}

Uri::~Uri(void) {}

} // namespace http
