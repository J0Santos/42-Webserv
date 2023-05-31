#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "config/blocks/block.hpp"
#include "config/directives.hpp"
#include "config/Options.hpp"
#include "utils/ft_filesystem.hpp"

#include <typeinfo>
#include <vector>

namespace config {

class Parser {
    public:

        Parser(ft::file filename);
        ~Parser(void);

        bool nextLine(void);

        std::string               getLine(void) const;
        size_t                    getPosition(void) const;
        std::vector<block> const& getBlocks(void) const;

        void error(void) const;

        template<LineType T>
        void parseLine(std::vector<std::string> const& args);

        struct InvalidSyntaxException : public std::exception {
                char const* what(void) const throw();
        };

        std::vector< DirectiveTypeTraitsBase* > m_directives;

    private:

        std::string m_line;

        size_t        m_pos;
        std::ifstream m_file;
};

void parse(ft::file const& filename);

} // namespace config

#endif /* CONFIG_PARSER_HPP */