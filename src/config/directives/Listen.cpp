#include "config/directives/Listen.hpp"

#include "utils/ft_string.hpp"

namespace directive {

Listen::Listen(std::vector<std::string> args)
    : m_host("localhost"), m_port("8080"), m_valid(false) {

    if (args.size() != 2 || args[0] != "listen") { return; }
    size_t pos = args[1].find(":");
    if (pos != std::string::npos) {
        if (!pos || pos == args[1].size() - 1) { return; }
        m_port = args[1].substr(0, pos);
        m_host = args[1].substr(pos + 1);
        if (!ft::string::isnumeric(m_port)) { return; }
    }
    else if (ft::string::isnumeric(args[1])) { m_port = args[1]; }
    else { m_host = args[1]; }
    m_valid = true;
}

Listen::~Listen(void) {}

bool Listen::isValid(void) const { return (m_valid); }

void Listen::extract(void* dest) { (void)dest; }

} // namespace directive
