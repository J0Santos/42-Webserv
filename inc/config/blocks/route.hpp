#ifndef ROUTE_HPP
#define ROUTE_HPP

#include "http/methods.hpp"
#include "utils/ft_filesystem.hpp"

#include <map>
#include <string>
#include <vector>

namespace config {

struct route {

        route(void);
        route(std::string target);
        route(route const& src);

        ~route(void);

        route& operator=(route const& rhs);

        bool operator==(route const& rhs) const;

        std::string                   m_target;
        ft::directory                 m_root;
        std::map<int, ft::file>       m_error_pages;
        unsigned                      m_max_body_size;
        std::vector<http::MethodType> m_allowed_methods;
        bool                          m_autoindex;
        ft::file                      m_index;
        std::string                   m_cgi_extension;
};

} // namespace config

#endif /* ROUTE_HPP */