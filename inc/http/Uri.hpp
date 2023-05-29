#ifndef HTTP_URI_HPP
#define HTTP_URI_HPP

#include "utils/ft_filesystem.hpp"
#include "utils/ft_string.hpp"

#include <string>

namespace http {

class Uri {

    public:

        Uri(std::string const& uri);
        ~Uri(void);

        // std::string m_target;
        std::string m_scheme;
        std::string m_host;
        std::string m_port;
        ft::file    m_path;
        std::string m_query;
};

} // namespace http

#endif /* HTTP_URI_HPP */