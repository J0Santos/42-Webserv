#ifndef CONFIG_DIRECTIVES_HPP
#define CONFIG_DIRECTIVES_HPP

#include "cgi/CgiHandler.hpp"
#include "config/blocks/block.hpp"
#include "config/Line.hpp"
#include "utils/ft_string.hpp"
#include "utils/Logger.hpp"

#include <string>
#include <typeinfo>
#include <vector>

namespace config {

class DirectiveTypeTraitsBase {
    public:

        // DirectiveTypeTraitsBase(std::string const& name) : m_name(name) {}
        virtual ~DirectiveTypeTraitsBase(void) {}

        virtual void parse(std::vector<std::string> const& args) = 0;

        // std::string const getName(void) const { return (m_name); };
        virtual std::string const getName(void) const = 0;

        virtual bool isValid(void) const = 0;
        virtual bool isBlockDirective(void) const = 0;
        virtual bool isRouteDirective(void) const = 0;
};

template<LineType Directive>
struct DirectiveTypeTraits;

template<>
struct DirectiveTypeTraits<Block> : public DirectiveTypeTraitsBase {

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

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<Route> : public DirectiveTypeTraitsBase {

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

        bool        m_valid;
        std::string m_target;
};

template<>
struct DirectiveTypeTraits<End> : public DirectiveTypeTraitsBase {

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

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<Listen> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(void)
            : m_valid(false), m_host("localhost"), m_port("8080") {}

        ~DirectiveTypeTraits(void) {}

        void parse(std::vector<std::string> const& args) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
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

        std::string const getName(void) const { return ("listen"); }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (false); }

        bool m_valid;

        std::string m_host;
        std::string m_port;
};

template<>
struct DirectiveTypeTraits<ServerName> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(void) : m_valid(false) {}

        ~DirectiveTypeTraits(void) {}

        void parse(std::vector<std::string> const& args) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            m_server_name = args[1];
            m_valid = true;
        }

        std::string const getName(void) const { return ("server_name"); }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (false); }

        bool m_valid;

        std::string m_server_name;
};

template<>
struct DirectiveTypeTraits<Root> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(void) : m_valid(false) {}

        ~DirectiveTypeTraits(void) {}

        void parse(std::vector<std::string> const& args) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            m_root = args[1];
            if (!m_root.isValid()) {
                LOG_W(getName() << ": invalid root.");
                return;
            }
            m_valid = true;
        }

        std::string const getName(void) const { return ("root"); }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (true); }

        ft::directory m_root;

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<ErrorPage> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(void) : m_valid(false) {}

        ~DirectiveTypeTraits(void) {}

        void parse(std::vector<std::string> const& args) {
            if (args.size() < 3) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            ft::file file = args.back();
            if (!file.isValid()) {
                LOG_W(getName() << ": invalid file.");
                return;
            }

            std::vector<std::string> error_codes =
                std::vector<std::string>(args.begin() + 1, args.end() - 1);

            for (std::vector<std::string>::iterator it = error_codes.begin();
                 it != error_codes.end(); ++it) {
                if (!ft::string::isnumeric(*it)) {
                    LOG_W(getName() << ": invalid error code.");
                    return;
                }
                m_error_pages[ft::string::stoi(*it)] = file;
            }

            m_valid = true;
        }

        std::string const getName(void) const { return ("error_page"); }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (true); }

        std::map<int, ft::file> m_error_pages;

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<MaxBodySize> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(void) : m_valid(false) {}

        ~DirectiveTypeTraits(void) {}

        void parse(std::vector<std::string> const& args) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            if (!ft::string::isnumeric(args[1])) {
                LOG_W(getName() << ": invalid max body size.");
                return;
            }
            m_max_body_size = ft::string::stoul(args[1]);
            m_valid = true;
        }

        std::string const getName(void) const {
            return ("client_max_body_size");
        }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (true); }

        unsigned long m_max_body_size;

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<AllowMethods> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(void) : m_valid(false) {}

        ~DirectiveTypeTraits(void) {}

        void parse(std::vector<std::string> const& args) {
            if (args.size() < 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            for (std::vector<std::string>::const_iterator it = args.begin() + 1;
                 it != args.end(); ++it) {
                if (http::convertMethod(*it) == http::UNKNOWN_METHOD) {
                    LOG_W(getName() << ": invalid method.");
                    return;
                }
                m_allowed_methods.push_back(*it);
            }
            m_valid = true;
        }

        std::string const getName(void) const { return ("allow_methods"); }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (true); }

        std::vector<std::string> m_allowed_methods;

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<Index> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(void) : m_valid(false) {}

        ~DirectiveTypeTraits(void) {}

        void parse(std::vector<std::string> const& args) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            m_index = args[1];
            if (!m_index.isValid()) {
                LOG_W(getName() << ": invalid index file.");
                return;
            }
            m_valid = true;
        }

        std::string const getName(void) const { return ("index"); }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (false); }

        bool isRouteDirective(void) const { return (true); }

        ft::file m_index;

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<AutoIndex> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(void) : m_valid(false) {}

        ~DirectiveTypeTraits(void) {}

        void parse(std::vector<std::string> const& args) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            if (args[1] == "on") { m_autoindex = true; }
            else if (args[1] == "off") { m_autoindex = false; }
            else {
                LOG_W(getName() << ": invalid autoindex value.");
                return;
            }
            m_valid = true;
        }

        std::string const getName(void) const { return ("autoindex"); }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (false); }

        bool isRouteDirective(void) const { return (true); }

        bool m_autoindex;

        bool m_valid;
};

template<>
struct DirectiveTypeTraits<CgiExtension> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(void) : m_valid(false) {}

        ~DirectiveTypeTraits(void) {}

        void parse(std::vector<std::string> const& args) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            if (cgi::convertCgiExtension(args[1]) == cgi::Unknown) {
                LOG_W(getName() << ": invalid cgi extension.");
                return;
            }
            m_cgi_extension = args[1];
            m_valid = true;
        }

        std::string const getName(void) const { return ("fastcgi"); }

        bool isValid(void) const { return (m_valid); }

        bool isBlockDirective(void) const { return (false); }

        bool isRouteDirective(void) const { return (true); }

        std::string m_cgi_extension;

        bool m_valid;
};

} // namespace config

#endif /* CONFIG_DIRECTIVES_HPP */