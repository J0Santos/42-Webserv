#include "config/blocks/block.hpp"

namespace config {

block::block(void) : m_max_body_size(0) {}

block::block(block const& src) { *this = src; }

block::~block(void) {}

block& block::operator=(block const& rhs) {
    if (this != &rhs) {
        m_port = rhs.m_port;
        m_host = rhs.m_host;
        m_server_name = rhs.m_server_name;
        m_root = rhs.m_root;
        m_error_pages = rhs.m_error_pages;
        m_max_body_size = rhs.m_max_body_size;
        m_allowed_methods = rhs.m_allowed_methods;
        m_routes = rhs.m_routes;
    }
    return *this;
}

bool block::operator==(block const& rhs) const {
    return (m_port == rhs.m_port && m_host == rhs.m_host &&
            m_server_name == rhs.m_server_name && m_root == rhs.m_root &&
            m_error_pages == rhs.m_error_pages &&
            m_max_body_size == rhs.m_max_body_size &&
            m_allowed_methods == rhs.m_allowed_methods &&
            m_routes == rhs.m_routes);
}

} // namespace config