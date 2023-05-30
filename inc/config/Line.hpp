#ifndef CONFIG_LINE_HPP
#define CONFIG_LINE_HPP

#include "config/directives.hpp"

#include <vector>

namespace config {

class Line {
    public:

        Line(void);
        Line(Line const& src);
        Line(std::string const& line);

        ~Line(void);

        Line& operator=(Line const& rhs);

        std::string getLine(void) const;
        LineType    getType(void) const;

        std::vector<std::string> format(void) const;

    private:

        LineType getDirectiveType(void) const;

        std::string m_line;
        LineType    m_type;
};

} // namespace config

#endif /* CONFIG_LINE_HPP */