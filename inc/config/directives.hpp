#ifndef CONFIG_DIRECTIVES_HPP
#define CONFIG_DIRECTIVES_HPP

#include "config/blocks/block.hpp"
#include "utils/ft_string.hpp"
#include "utils/Logger.hpp"

#include <string>
#include <typeinfo>
#include <vector>

namespace config {

enum LineType {
    Empty,
    Block,
    Route,
    End,
    Listen,
    ServerName,
    /* Root,
    ErrorPage,
    MaxBodySize,
    AllowMethods,
    Index,
    AutoIndex,
    CgiExtension, */
    Unknown
};

template<LineType Directive>
struct DirectiveTypeTraits;

template<>
struct DirectiveTypeTraits<Block> {

        DirectiveTypeTraits(void) : m_valid(false) {}

        ~DirectiveTypeTraits(void) {}

        void parse(std::vector<std::string> const& args) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != "server") {
                LOG_W(getName() << ": invalid directive.");
                return;
            }
            if (args[1] != "{") {
                LOG_W(getName() << ": invalid end.");
                return;
            }
            m_valid = true;
        }

        std::string const getName(void) const { return ("Block"); }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (false); }

        bool isRouteDirective(void) const { return (false); }

        void extract(std::vector<block>& blocks) { blocks.push_back(block()); }

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<Route> {

        DirectiveTypeTraits(void) : m_valid(false) {}

        ~DirectiveTypeTraits(void) {}

        void parse(std::vector<std::string> const& args) {
            if (args.size() != 3) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != "location") {
                LOG_W(getName() << ": invalid directive.");
                return;
            }
            if (args[2] != "{") {
                LOG_W(getName() << ": invalid end.");
                return;
            }
            m_target = (args[1][0] != '/') ? ("/" + args[1]) : args[1];
            m_valid = true;
        }

        std::string const getName(void) const { return ("Route"); }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (false); }

        void extract(std::vector<block>& blocks) {
            blocks.back().m_routes.insert(
                std::make_pair(m_target, route(m_target)));
        }

        bool        m_valid;
        std::string m_target;
};

template<>
struct DirectiveTypeTraits<End> {

        DirectiveTypeTraits(void) : m_valid(false) {}

        ~DirectiveTypeTraits(void) {}

        void parse(std::vector<std::string> const& args) {
            if (args.size() != 1) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != "}") {
                LOG_W(getName() << ": invalid directive.");
                return;
            }
            m_valid = true;
        }

        std::string const getName(void) const { return ("End"); }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (true); }

        void extract(std::vector<block>& blocks) { (void)blocks; }

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<Listen> {

        DirectiveTypeTraits(void)
            : m_valid(false), m_host("localhost"), m_port("8080") {}

        ~DirectiveTypeTraits(void) {}

        void parse(std::vector<std::string> const& args) {
            if (args.size() != 2 || args[0] != "listen") {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            size_t pos = args[1].find(":");
            if (pos != std::string::npos) {
                if (!pos || pos == args[1].size() - 1) {
                    LOG_W(getName() << ": invalid host:port.");
                    return;
                }
                m_port = args[1].substr(0, pos);
                m_host = args[1].substr(pos + 1);
                if (!ft::string::isnumeric(m_port)) {
                    LOG_W(getName() << ": invalid host:port.");
                    return;
                }
            }
            else if (ft::string::isnumeric(args[1])) { m_port = args[1]; }
            else { m_host = args[1]; }
            m_valid = true;
        }

        std::string const getName(void) const { return ("Listen"); }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (false); }

        void extract(std::vector<block>& blocks) {
            blocks.back().m_host = m_host;
            blocks.back().m_port = m_port;
        }

        bool m_valid;

        std::string m_host;
        std::string m_port;
};

template<>
struct DirectiveTypeTraits<ServerName> {

        DirectiveTypeTraits(void) : m_valid(false) {}

        ~DirectiveTypeTraits(void) {}

        void parse(std::vector<std::string> const& args) {
            if (args.size() != 2 || args[0] != "server_name") {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            m_valid = true;
        }

        std::string const getName(void) const { return ("ServerName"); }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (false); }

        void extract(std::vector<block>& blocks) {
            blocks.back().m_server_name = m_server_name;
        }

        bool m_valid;

        std::string m_server_name;
};

} // namespace config

#endif /* CONFIG_DIRECTIVES_HPP */