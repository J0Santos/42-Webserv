#include "config/Parser.hpp"

#include "config/directives.hpp"
#include "config/Line.hpp"
#include "utils/ft_exceptions.hpp"
#include "utils/Logger.hpp"

#include <iostream>

namespace config {

Parser::Parser(ft::file filename) : m_line(), m_pos(0), m_status(InNone) {
    if (!filename.isValid()) { throw ft::InvalidFileException(); }
    m_file.open(std::string(filename).c_str());
    if (!m_file.is_open()) { throw ft::FailedToOpenFileException(); }
}

Parser::~Parser(void) {
    m_file.close();
    if (!nextLine() && m_status != InNone) { error(); }
}

bool Parser::nextLine(void) {
    if (m_file.eof()) {
        // m_line.clear();
        return (false);
    }
    std::getline(m_file, m_line);
    m_pos++;
    return (true);
}

std::string Parser::getLine(void) const { return (m_line); }

size_t Parser::getPosition(void) const { return (m_pos); }

void Parser::error(void) const {
    LOG_E("config: syntax error: " << m_line << " (:" << m_pos << ")");
    throw InvalidSyntaxException();
}

template<LineType T>
void Parser::parseLine(std::vector<std::string> const& args) {
    DirectiveTypeTraits<T> directive;

    // validate directive placement
    LOG_I("directive: " << directive.getName());
    if (m_status == InNone &&
        (directive.isRouteDirective() || directive.isBlockDirective())) {
        error();
    }
    if (m_status == InBlock && !directive.isBlockDirective()) { error(); }
    if (m_status == InRoute && !directive.isRouteDirective()) { error(); }
    LOG_I("passed placement control");

    // parse directive
    directive.parse(args);
    LOG_I("passed parsing");
    if (!directive.isValid()) { error(); }
    LOG_I("passed validation");
    directive.extract(m_blocks);
    LOG_I("passed extraction");

    // update status
    if (T == End) {
        if (m_status == InRoute) { m_status = InBlock; }
        else if (m_status == InBlock) { m_status = InNone; }
    }
    if (T == Block) { m_status = InBlock; }
    if (T == Route) { m_status = InRoute; }
    LOG_I("updated status");
}

char const* Parser::InvalidSyntaxException::what(void) const throw() {
    return ("config::parser: invalid syntax.");
}

void parse(ft::file const& filename) {
    Parser parser(filename);

    while (parser.nextLine()) {
        Line line(parser.getLine());
        std::cout << "line: " << line.getLine() << std::endl;
        switch (line.getType()) {
            case (Empty): break;

            case (Block): parser.parseLine<Block>(line.format()); break;

            case (Route): parser.parseLine<Route>(line.format()); break;

            case (End): parser.parseLine<End>(line.format()); break;

            case (Listen): parser.parseLine<Listen>(line.format()); break;

            case (ServerName):
                parser.parseLine<ServerName>(line.format());
                break;

            case (Root): parser.parseLine<Root>(line.format()); break;

            case (Unknown):
            default: parser.error();
        }
    }
    // TODO: check if it ended with a close bracket
    // check if directives are valid, if theres no missing things
    // create default location block
}

} // namespace config
