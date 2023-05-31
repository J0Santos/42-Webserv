#include "config/Parser.hpp"

#include "config/directives.hpp"
#include "config/Line.hpp"
#include "utils/ft_exceptions.hpp"
#include "utils/Logger.hpp"

#include <iostream>

namespace config {

Parser::Parser(ft::file filename) : m_line(), m_pos(0) {
    if (!filename.isValid()) { throw ft::InvalidFileException(); }
    m_file.open(std::string(filename).c_str());
    if (!m_file.is_open()) { throw ft::FailedToOpenFileException(); }
}

Parser::~Parser(void) { m_file.close(); }

bool Parser::nextLine(void) {
    if (m_file.eof()) { return (false); }
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
    DirectiveTypeTraits<T>* directive = new DirectiveTypeTraits<T>();

    directive->parse(args);
    if (!directive->isValid()) { error(); }

    m_directives.push_back(static_cast<DirectiveTypeTraitsBase*>(directive));
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
    try {
        // Options::getInstance(parser.getDirectives());
    }
    catch (std::exception&) {
        throw Parser::InvalidSyntaxException();
    }
}

} // namespace config
