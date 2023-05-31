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

Parser::~Parser(void) {

    // closing file
    m_file.close();

    // deleting all directives
    for (std::vector<DirectiveTypeTraitsBase*>::iterator it =
             m_directives.begin();
         it != m_directives.end(); ++it) {
        delete *it;
    }
}

bool Parser::nextLine(void) {
    if (m_file.eof()) { return (false); }
    std::getline(m_file, m_line);
    m_pos++;
    return (true);
}

std::string Parser::getLine(void) const { return (m_line); }

size_t Parser::getPosition(void) const { return (m_pos); }

std::vector< DirectiveTypeTraitsBase* > const&
    Parser::getParsedDirectives(void) const {
    return (m_directives);
}

void Parser::error(void) const {
    LOG_E("config: syntax error: " << m_line << " (:" << m_pos << ")");
    throw InvalidSyntaxException();
}

template<LineType T>
void Parser::parseLine(std::vector<std::string> const& args) {

    DirectiveTypeTraits<T>* directive = new DirectiveTypeTraits<T>(args);
    m_directives.push_back(static_cast<DirectiveTypeTraitsBase*>(directive));

    if (!directive->isValid()) { error(); }
}

char const* Parser::InvalidSyntaxException::what(void) const throw() {
    return ("config::parser: invalid syntax.");
}

void parse(ft::file const& filename) {
    Parser parser(filename);
    while (parser.nextLine()) {
        Line line(parser.getLine());
        switch (line.getType()) {
            case (LineEmpty): break;

            case (LineBlock): parser.parseLine<LineBlock>(line.format()); break;

            case (LineRoute): parser.parseLine<LineRoute>(line.format()); break;

            case (LineEnd): parser.parseLine<LineEnd>(line.format()); break;

            case (LineListen):
                parser.parseLine<LineListen>(line.format());
                break;

            case (LineServerName):
                parser.parseLine<LineServerName>(line.format());
                break;

            case (LineRoot): parser.parseLine<LineRoot>(line.format()); break;

            case (LineErrorPage):
                parser.parseLine<LineErrorPage>(line.format());
                break;

            case (LineMaxBodySize):
                parser.parseLine<LineMaxBodySize>(line.format());
                break;

            case (LineAllowMethods):
                parser.parseLine<LineAllowMethods>(line.format());
                break;

            case (LineIndex): parser.parseLine<LineIndex>(line.format()); break;

            case (LineAutoIndex):
                parser.parseLine<LineAutoIndex>(line.format());
                break;

            case (LineCgiExtension):
                parser.parseLine<LineCgiExtension>(line.format());
                break;

            case (LineUnknown):
            default: parser.error();
        }
    }
    // std::vector< smt::shared_ptr<Opts> > opts;
    // opts = extract(parser.getParsedDirectives());
    // return (opts);
}

} // namespace config
