#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include "cgi/Envp.hpp"
#include "http/methods.hpp"
#include "http/Request.hpp"
#include "utils/Logger.hpp"

#include <cstdlib>
#include <iostream>
#include <map>
#include <sys/wait.h>

namespace cgi {
class CgiHandler {
    public:

        CgiHandler(http::Request const& request, http::Route const& route);
        ~CgiHandler(void);

        std::pair<std::string, int> run(void);
        std::pair<std::string, int> runAsChildProcess(void);

        char*  getPath(void) const;
        char** getEnvp(void) const;
        char** getArgv(void) const;

    private:

        void setPath(std::string const& path);
        void setEnvp(Envp const& envp);
        void setArgv(std::string const& path, std::string const& script);

        http::Request m_request;
        char**        m_envp;
        char**        m_argv;
        char*         m_path;

        // Envp          m_envp;
};
} // namespace cgi

std::ostream& operator<<(std::ostream& os, cgi::Envp const& ctx);

#endif // CGI_HANDLER_HPP