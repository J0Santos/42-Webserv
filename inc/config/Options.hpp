#ifndef CONFIG_OPTIONS_HPP
#define CONFIG_OPTIONS_HPP

#include "config/directives.hpp"
#include "config/Parser.hpp"
#include "utils/smt.hpp"

namespace config {

class DirectiveTypeTraitsBase;

template<LineType Directive>
struct DirectiveTypeTraits;

struct Opts;

// struct Opts {
//         virtual ~Opts(void) {}

// std::string              m_target;
// std::string              m_host;
// std::string              m_port;
// ft::directory            m_root;
// std::string              m_server_name;
// std::map<int, ft::file>  m_error_pages;
// unsigned long            m_max_body_size;
// std::vector<std::string> m_allowed_methods;
// ft::file                 m_index;
// bool                     m_autoindex;
// std::string              m_cgi_extension;
// };

std::ostream& operator<<(std::ostream& os, Opts const& rhs);

std::vector< smt::shared_ptr<Opts> >
    extract(std::vector<DirectiveTypeTraitsBase*> const& directives);

std::vector<DirectiveTypeTraitsBase*>
    getNextBlock(std::vector<DirectiveTypeTraitsBase*> const& dirs =
                     std::vector<DirectiveTypeTraitsBase*>());

void extractBlock(std::vector<DirectiveTypeTraitsBase*> const&    directives,
                  std::map< std::string, smt::shared_ptr<Opts> >& opts);

} // namespace config

#endif /* CONFIG_OPTIONS_HPP */