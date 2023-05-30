#include "cgi/Argc.hpp"

namespace cgi {

Argc::Argc(void) {}

Argc::Argc(std::vector<std::string> const& argc) : m_argc(argc) {}

Argc::Argc(Argc const& src) { *this = src; }

Argc::~Argc(void) {}

Argc& Argc::operator=(Argc const& rhs) {
    if (this != &rhs) { m_argc = rhs.m_argc; }
    return (*this);
}

Argc::operator char**(void) const { return (ft::array(m_argc)); }

} // namespace cgi
