#include "config/directives.hpp"

namespace config {

DirectiveTypeTraitsBase::DirectiveTypeTraitsBase(std::string const& name)
    : m_name(name) {}

DirectiveTypeTraitsBase::~DirectiveTypeTraitsBase(void) {}

std::string const DirectiveTypeTraitsBase::getName(void) { return (m_name); }

} // namespace config