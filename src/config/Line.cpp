#include "config/Line.hpp"

namespace config {

Line::Line(Line const& src) : m_line(src.m_line), m_type(src.m_type) {}

Line::Line(std::string const& line) : m_line(ft::string::trim(line)) {
    m_type = convertLineType(m_line);
}

Line::~Line(void) {}

Line& Line::operator=(Line const& rhs) {
    if (this != &rhs) {
        m_line = rhs.m_line;
        m_type = rhs.m_type;
    }
    return (*this);
}

std::string Line::getLine(void) const { return (m_line); }

LineType Line::getType(void) const { return (m_type); }

LineType convertLineType(std::string const& line) {
    std::vector<std::string> subs = ft::string::split(line, " \t");
    if (subs.empty() || subs[0] == LINE_COMMENT) { return (Empty); }
    if (subs[0] == BLOCK_SERVER) { return (Block); }
    if (subs[0] == BLOCK_LOCATION) { return (Route); }
    if (subs[0] == BLOCK_END) { return (End); }
    if (subs[0] == DIRECTIVE_LISTEN) { return (Listen); }
    if (subs[0] == DIRECTIVE_SERVER_NAME) { return (ServerName); }
    if (subs[0] == DIRECTIVE_ROOT) { return (Root); }
    if (subs[0] == DIRECTIVE_ERROR_PAGE) { return (ErrorPage); }
    if (subs[0] == DIRECTIVE_MAX_BODY_SIZE) { return (MaxBodySize); }
    if (subs[0] == DIRECTIVE_ALLOW_METHODS) { return (AllowMethods); }
    if (subs[0] == DIRECTIVE_INDEX) { return (Index); }
    if (subs[0] == DIRECTIVE_AUTOINDEX) { return (AutoIndex); }
    if (subs[0] == DIRECTIVE_FASTCGI) { return (CgiExtension); }
    return (Unknown);
}

std::vector<std::string> Line::format(void) const {
    std::vector<std::string> subs;
    if (m_type == Empty || m_type == Unknown) { return (subs); }

    size_t      pos = m_line.find('#');
    std::string str =
        (pos == std::string::npos) ? m_line : m_line.substr(0, pos);
    str = ft::string::trim(str);

    if ((m_type == Block || m_type == Route) &&
        str.find('{') != str.size() - 1) {
        return (subs);
    }
    else if (m_type == End && str != "}") { return (subs); }
    else if (m_type > End && str.find(';') != str.size() - 1) { return (subs); }

    str = (str.find(';') != std::string::npos) ? (str.substr(0, str.size() - 1))
                                               : str;

    subs = ft::string::split(str, " \t");
    return (subs);
}

} // namespace config
