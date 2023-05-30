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
    if (m_status == InNone &&
        (directive.isRouteDirective() || directive.isBlockDirective())) {
        error();
    }
    if (m_status == InBlock && !directive.isBlockDirective()) { error(); }
    if (m_status == InRoute && !directive.isRouteDirective()) { error(); }

    // parse directive
    directive.parse(args);
    if (!directive.isValid()) { error(); }
    directive.extract(m_blocks);

    // update status
    if (T == End) {
        if (m_status == InRoute) { m_status = InBlock; }
        else if (m_status == InBlock) { m_status = InNone; }
    }
    if (T == Block) { m_status = InBlock; }
    if (T == Route) { m_status = InRoute; }
}

char const* Parser::InvalidSyntaxException::what(void) const throw() {
    return ("config::parser: invalid syntax.");
}

void parse(ft::file const& filename) {
    Parser parser(filename);

    while (parser.nextLine()) {
        Line line(parser.getLine());
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

            case (ErrorPage): parser.parseLine<ErrorPage>(line.format()); break;

            case (MaxBodySize):
                parser.parseLine<MaxBodySize>(line.format());
                break;

            case (AllowMethods):
                parser.parseLine<AllowMethods>(line.format());
                break;

            case (Index): parser.parseLine<Index>(line.format()); break;

            case (AutoIndex): parser.parseLine<AutoIndex>(line.format()); break;

            case (CgiExtension):
                parser.parseLine<CgiExtension>(line.format());
                break;

            case (Unknown):
            default: parser.error();
        }
    }
    // TODO: check if it ended with a close bracket
    // check if directives are valid, if theres no missing things
    // create default location block
}

} // namespace config
