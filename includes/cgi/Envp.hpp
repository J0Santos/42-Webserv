#ifndef CGI_ENVP_HPP
#define CGI_ENVP_HPP

#include "utils/ft_array.hpp"

namespace http {
class Request;
}

namespace cgi {

class Envp {

    public:

        Envp(void);
        Envp(http::Request const& request);
        Envp(Envp const& src);

        ~Envp(void);

        Envp& operator=(Envp const& rhs);

        operator char**(void) const;

    private:

        void fillEnvp(std::string const& key, std::string const& value);

        std::vector<std::string> m_argc;
};

} // namespace cgi

#endif /* CGI_ENVP_HPP */