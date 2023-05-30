#include "config/blocks/route.hpp"

namespace config {

route::route(void)
    : m_target("/"), m_max_body_size(0), m_autoindex(false), m_closed(false) {}

route::route(std::string target)
    : m_target(target), m_max_body_size(0), m_autoindex(false),
      m_closed(false) {}

route::route(route const& src) { *this = src; }

route::~route(void) {}

route& route::operator=(route const& rhs) {
    if (this != &rhs) {
        m_target = rhs.m_target;
        m_root = rhs.m_root;
        m_error_pages = rhs.m_error_pages;
        m_max_body_size = rhs.m_max_body_size;
        m_allowed_methods = rhs.m_allowed_methods;
        m_autoindex = rhs.m_autoindex;
        m_index = rhs.m_index;
        m_cgi_extension = rhs.m_cgi_extension;
        m_closed = rhs.m_closed;
    }
    return *this;
}

bool route::operator==(route const& rhs) const {
    return (m_target == rhs.m_target && m_root == rhs.m_root &&
            m_error_pages == rhs.m_error_pages &&
            m_max_body_size == rhs.m_max_body_size &&
            m_allowed_methods == rhs.m_allowed_methods &&
            m_autoindex == rhs.m_autoindex && m_index == rhs.m_index &&
            m_cgi_extension == rhs.m_cgi_extension && m_closed == rhs.m_closed);
}

} // namespace config