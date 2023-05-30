#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "config/blocks/block.hpp"
#include "config/directives.hpp"
#include "utils/ft_filesystem.hpp"

#include <typeinfo>
#include <vector>

namespace config {

class Parser {
    public:

        Parser(ft::file filename);
        ~Parser(void);

        bool nextLine(void);

        std::string getLine(void) const;
        size_t      getPosition(void) const;

        void error(void) const;

        template<LineType T>
        void parseLine(std::vector<std::string> const& args);

        struct InvalidSyntaxException : public std::exception {
                char const* what(void) const throw();
        };

        enum StatusType {
            InBlock = 0,
            InRoute = 1,
            InNone = 2,
        };

    private:

        std::string m_line;

        size_t        m_pos;
        std::ifstream m_file;

        StatusType m_status;

        std::vector<block> m_blocks;
};

void parse(ft::file const& filename);

} // namespace config

#endif /* CONFIG_PARSER_HPP */