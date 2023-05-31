#include "config/Options.hpp"

namespace config {

Options::Options(std::vector< smt::shared_ptr<Opts> > opts) : m_opts(opts) {}

Options::~Options(void) {}

smt::shared_ptr<Options>
    Options::getInstance(std::vector< smt::shared_ptr<Opts> > opts) {
    static smt::shared_ptr<Options> ist(new Options(opts));
    return (ist);
}

static std::vector<smt::shared_ptr<Opts> >
    extractOpts(std::map< std::string, smt::shared_ptr<Opts> > const& opts) {
    std::vector<smt::shared_ptr<Opts> > ret;

    std::map< std::string, smt::shared_ptr<Opts> >::const_iterator it;
    for (it = opts.begin(); it != opts.end(); ++it) {
        ret.push_back(it->second);
    }
    return (ret);
}

std::vector<DirectiveTypeTraitsBase*>
    getNextBlock(std::vector<DirectiveTypeTraitsBase*> const& dirs) {

    static std::vector<DirectiveTypeTraitsBase*> directives = dirs;

    if (!dirs.empty()) { directives = dirs; }

    int level = 0;

    std::vector<DirectiveTypeTraitsBase*>                 block;
    std::vector<DirectiveTypeTraitsBase*>::const_iterator it;
    for (it = directives.begin(); it != directives.end(); ++it) {
        if ((*it)->getName() == BLOCK_SERVER) { ++level; }
        else if ((*it)->getName() == BLOCK_LOCATION) { ++level; }
        else if ((*it)->getName() == BLOCK_END) { --level; }

        block.push_back(*it);
        if (level == 0) { break; }
    }
    if (level != 0) { throw config::Parser::InvalidSyntaxException(); }
    else {
        directives.erase(directives.begin(), directives.begin() + block.size());
    }
    return (block);
}

static void setDefaults(std::vector< smt::shared_ptr<Opts> >& opts) {
    std::vector< smt::shared_ptr<Opts> >::iterator it;
    for (it = opts.begin(); it != opts.end(); ++it) {
        // checking if mandatory directives are present
        if ((*it)->m_port.empty() && (*it)->m_host.empty()) {
            LOG_E("listen directive not found in server block");
            throw config::Parser::InvalidSyntaxException();
        }
        if (std::string((*it)->m_root).empty()) {
            LOG_E("root directive not found in server block");
            throw config::Parser::InvalidSyntaxException();
        }

        // setting default values
        if ((*it)->m_port.empty()) { (*it)->m_port = "8080"; }
        if ((*it)->m_host.empty()) { (*it)->m_host = "localhost"; }
        if (!(*it)->m_max_body_size) { (*it)->m_max_body_size = 1000000; }
        if ((*it)->m_allowed_methods.empty()) {
            (*it)->m_allowed_methods.push_back("GET");
            (*it)->m_allowed_methods.push_back("POST");
            (*it)->m_allowed_methods.push_back("DELETE");
        }
    }
}

std::vector< smt::shared_ptr<Opts> >
    extract(std::vector<DirectiveTypeTraitsBase*> const& directives) {

    std::vector< smt::shared_ptr<Opts> > ret;
    if (directives.empty()) { return (ret); }

    std::vector<DirectiveTypeTraitsBase*> blocks = getNextBlock(directives);
    while (!blocks.empty()) {
        std::map< std::string, smt::shared_ptr<Opts> > opts;
        extractBlock(blocks, opts);
        std::vector< smt::shared_ptr<Opts> > tmp = extractOpts(opts);
        ret.insert(ret.end(), tmp.begin(), tmp.end());
        opts.clear();
        blocks = getNextBlock();
    }
    setDefaults(ret);
    return (ret);
}

static void createOpts(std::vector<DirectiveTypeTraitsBase*> const& directives,
                       std::map< std::string, smt::shared_ptr<Opts> >& opts) {
    std::vector<DirectiveTypeTraitsBase*>::const_iterator it;
    for (it = directives.begin(); it != directives.end(); ++it) {
        if ((*it)->getName() == BLOCK_LOCATION) {
            smt::shared_ptr<Opts> opt(new Opts);
            opt->m_target =
                dynamic_cast< DirectiveTypeTraits<Route>* >(*it)->m_target;
            if (opts.find(opt->m_target) != opts.end()) {
                LOG_E("location with target " << opt->m_target
                                              << " already exists");
                throw config::Parser::InvalidSyntaxException();
            }
            opts.insert(std::make_pair(opt->m_target, opt));
        }
    }
    if (opts.find("/") == opts.end()) {
        smt::shared_ptr<Opts> opt(new Opts);
        opt->m_target = "/";
        opts.insert(std::make_pair(opt->m_target, opt));
    }
}

static void setDefaults(std::vector<DirectiveTypeTraitsBase*> const& directives,
                        std::map< std::string, smt::shared_ptr<Opts> >& opts) {
    int level = 0;

    std::vector<DirectiveTypeTraitsBase*>::const_iterator it;
    for (it = directives.begin(); it != directives.end(); ++it) {
        // LOG_D("directive: " << (*it)->getName());
        if ((*it)->getName() == BLOCK_SERVER) { ++level; }
        else if ((*it)->getName() == BLOCK_LOCATION) { ++level; }
        else if ((*it)->getName() == BLOCK_END) { --level; }
        else if (level == 1) {
            // validate directive level
            if (!(*it)->isBlockDirective()) {
                LOG_E("directive " << (*it)->getName()
                                   << " is not a block directive");
                throw config::Parser::InvalidSyntaxException();
            }
            std::map< std::string, smt::shared_ptr<Opts> >::iterator itt;
            for (itt = opts.begin(); itt != opts.end(); ++itt) {
                (*it)->set(itt->second);
            }
        }
    }
}

void extractBlock(std::vector<DirectiveTypeTraitsBase*> const&    directives,
                  std::map< std::string, smt::shared_ptr<Opts> >& opts) {
    // look for all locations and create opts with location target
    createOpts(directives, opts);
    // extract all server only options and set them to all opts
    setDefaults(directives, opts);

    // get each location and set its options
    int         level = 0;
    std::string currentTarget = "";

    std::vector<DirectiveTypeTraitsBase*>::const_iterator it;
    for (it = directives.begin(); it != directives.end(); ++it) {
        if ((*it)->getName() == BLOCK_SERVER) { ++level; }
        else if ((*it)->getName() == BLOCK_LOCATION) {
            currentTarget =
                dynamic_cast< DirectiveTypeTraits<Route>* >(*it)->m_target;
            ++level;
        }
        else if ((*it)->getName() == BLOCK_END) { --level; }
        else if (level == 2) {
            // validate directive level
            if (!(*it)->isRouteDirective()) {
                LOG_E("directive " << (*it)->getName()
                                   << " is not a location directive");
                throw config::Parser::InvalidSyntaxException();
            }
            (*it)->set(opts[currentTarget]);
        }
    }
}

std::ostream& operator<<(std::ostream& os, Opts const& rhs) {
    os << "target: " << rhs.m_target << std::endl
       << " host: " << rhs.m_host << std::endl
       << " port: " << rhs.m_port << std::endl
       << " root: " << rhs.m_root << std::endl
       << " server_name: " << rhs.m_server_name << std::endl
       << " error_pages: ";

    for (std::map<int, ft::file>::const_iterator it = rhs.m_error_pages.begin();
         it != rhs.m_error_pages.end(); ++it) {
        os << "(" << it->first << " " << it->second << ") ";
    }

    os << std::endl
       << " max_body_size: " << rhs.m_max_body_size << std::endl
       << " allowed_methods: ";

    for (std::vector<std::string>::const_iterator it =
             rhs.m_allowed_methods.begin();
         it != rhs.m_allowed_methods.end(); ++it) {
        os << *it << " ";
    }

    os << std::endl
       << " index: " << rhs.m_index << std::endl
       << " autoindex: " << rhs.m_autoindex << std::endl
       << " cgi: " << rhs.m_cgi_extension << std::endl;
    // << " error_pages: " << rhs.m_error_pages << std::endl;
    return (os);
}

} // namespace config
