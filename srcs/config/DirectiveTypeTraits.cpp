#include "config/DirectiveTypeTraits.hpp"

namespace config {

DirectiveTypeTraitsBase::DirectiveTypeTraitsBase(std::string const& name)
    : m_name(name) {}

DirectiveTypeTraitsBase::~DirectiveTypeTraitsBase(void) {}

bool DirectiveTypeTraitsBase::operator==(
    DirectiveTypeTraitsBase const& rhs) const {
    return (m_name == rhs.m_name);
}

std::string const DirectiveTypeTraitsBase::getName(void) { return (m_name); }

} // namespace config