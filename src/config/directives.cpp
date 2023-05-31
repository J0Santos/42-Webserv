#include "config/directives.hpp"

namespace config {

DirectiveTypeTraitsBase::DirectiveTypeTraitsBase(std::string const& name)
    : m_name(name) {}

DirectiveTypeTraitsBase::~DirectiveTypeTraitsBase(void) {}

bool DirectiveTypeTraitsBase::operator==(
    DirectiveTypeTraitsBase const& rhs) const {
    LOG_D("Here");
    LOG_D("name: " << m_name << " rhs: " << rhs.m_name);
    return (m_name == rhs.m_name);
}

std::string const DirectiveTypeTraitsBase::getName(void) { return (m_name); }

bool Opts::operator==(Opts const& rhs) const {
    return (m_target == rhs.m_target && m_host == rhs.m_host &&
            m_port == rhs.m_port && m_root == rhs.m_root &&
            m_server_name == rhs.m_server_name &&
            m_error_pages == rhs.m_error_pages &&
            m_max_body_size == rhs.m_max_body_size &&
            m_allowed_methods == rhs.m_allowed_methods &&
            m_index == rhs.m_index && m_autoindex == rhs.m_autoindex &&
            m_cgi_extension == rhs.m_cgi_extension);
}

} // namespace config