#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "config/blocks/route.hpp"
#include "http/methods.hpp"
#include "utils/ft_filesystem.hpp"

#include <map>
#include <string>

namespace config {

struct block {

        block(void);
        block(block const& src);

        ~block(void);

        block& operator=(block const& rhs);

        bool operator==(block const& rhs) const;

        std::string                   m_port;
        std::string                   m_host;
        std::string                   m_server_name;
        ft::directory                 m_root;
        std::map<int, ft::file>       m_error_pages;
        unsigned                      m_max_body_size;
        std::vector<http::MethodType> m_allowed_methods;
        std::map<std::string, route > m_location;
};

} // namespace config

#endif /* BLOCK_HPP */