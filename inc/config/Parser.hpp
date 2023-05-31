#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "config/blocks/block.hpp"
#include "config/directives.hpp"
#include "config/Options.hpp"
#include "utils/ft_filesystem.hpp"
#include "utils/smt.hpp"

#include <typeinfo>
#include <vector>

namespace config {

class DirectiveTypeTraitsBase;
struct Opts;

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

        std::vector< DirectiveTypeTraitsBase* > const&
            getParsedDirectives(void) const;

    private:

        std::string   m_line;
        size_t        m_pos;
        std::ifstream m_file;

        std::vector< DirectiveTypeTraitsBase* > m_directives;
};

std::vector< smt::shared_ptr<config::Opts> > parse(ft::file const& filename);

} // namespace config

#endif /* CONFIG_PARSER_HPP */