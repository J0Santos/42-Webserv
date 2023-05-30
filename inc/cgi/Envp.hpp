#ifndef CGI_ENVP_HPP
#define CGI_ENVP_HPP

#include "http/Request.hpp"
#include "utils/Route.hpp"

#include <iostream>
#include <map>
#include <vector>

namespace cgi {
class Envp {
    public:

        Envp(http::Request const& request, http::Route const& route);
        ~Envp(void);

        void fillEnvp(std::string name, std::string val);
        std::map<std::string, std::string> getMapEnvp(void) const;

        std::string get(std::string const& key) const;

    private:

        std::map<std::string, std::string> m_envp;
};

} // namespace cgi

std::ostream& operator<<(std::ostream& os, cgi::Envp const& ctx);

#endif /* CGI_ENVP_HPP */