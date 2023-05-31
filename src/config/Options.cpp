#include "config/Options.hpp"

namespace config {

Options::Options(std::vector< smt::shared_ptr<Opts> > const& opts) {

    std::vector< smt::shared_ptr<Opts> >::const_iterator it;
    for (it = opts.begin(); it != opts.end(); ++it) {

        // creating key
        std::string key = (*it)->m_port + ":" + (*it)->m_host + ":" +
                          (*it)->m_server_name + ":" + (*it)->m_target;

        // check if key already exists
        if (m_opts.find(key) != m_opts.end()) {
            LOG_E("config::Options: Duplicate block found");
            throw Parser::InvalidSyntaxException();
        }

        // insert block
        m_opts.insert(std::make_pair(key, (*it)));
    }
}

Options::~Options(void) {}

smt::shared_ptr<Options>
    Options::getInstance(std::vector< smt::shared_ptr<Opts> > opts) {
    static smt::shared_ptr<Options> ist(new Options(opts));
    return (ist);
}

Options::OptsMap Options::getAllOpts(void) const { return (m_opts); }

smt::shared_ptr<Opts> Options::getOpts(std::string port, std::string host,
                                       std::string path, std::string header) {
    return (getOpts(port + ":" + host + ":" + header + ":" + path));
}

smt::shared_ptr<Opts> Options::getOpts(std::string key) {
    smt::shared_ptr<Opts> opts;

    OptsMap blocks = Options::getInstance()->getAllOpts();

    // get all blocks from port and host
    blocks = getBlocksWithPortAndHost(key, blocks);

    // for (OptsMap::const_iterator it = blocks.begin(); it != blocks.end();
    //      ++it) {
    //     LOG_F("config::Options: Checking block: " << it->first);
    // }
    // LOG_F(std::endl);

    // get all blocks from server anme
    blocks = getBlocksFromServerName(key, blocks);

    for (OptsMap::const_iterator it = blocks.begin(); it != blocks.end();
         ++it) {
        LOG_D("config::Options: Checking block: " << it->first);
    }
    LOG_D(std::endl);

    // get blocks that best corresponds with target
    // opts = getBlockFromTarget(key, blocks);
    return (opts);
}

Options::OptsMap
    Options::getBlocksWithPortAndHost(std::string const&      specs,
                                      Options::OptsMap const& blocks) {

    Options::OptsMap subs;

    // find position of the second ':' in key
    std::string::size_type pos = specs.find(':', specs.find(':') + 1);
    if (pos == std::string::npos) {
        LOG_E("config::Options: Invalid key");
        throw Parser::InvalidSyntaxException();
    }
    std::string key = specs.substr(0, pos + 1);

    OptsMap::const_iterator it;
    for (it = blocks.begin(); it != blocks.end(); ++it) {
        if (!it->first.find(key)) { subs.insert(*it); }
    }
    return (subs);
}

Options::OptsMap Options::getBlocksFromServerName(std::string const& specs,
                                                  OptsMap const&     blocks) {
    Options::OptsMap subs;

    LOG_D("config::Options: Searching for server name: " << specs);

    // find position of the third ':' in key
    std::string::size_type pos = specs.find_last_of(':');
    if (pos == std::string::npos) { throw Parser::InvalidSyntaxException(); }

    std::string key = specs.substr(0, pos + 1);

    OptsMap::const_iterator it;
    for (it = blocks.begin(); it != blocks.end(); ++it) {
        if (it->first.find(key) != std::string::npos) { subs.insert(*it); }
    }

    if (subs.empty()) {
        // find position of the second ':' in key
        pos = blocks[0].first.find(':', blocks[0].first.find(':') + 1);
        if (pos == std::string::npos) {
            throw Parser::InvalidSyntaxException();
        }
        key = blocks[0].first.substr(0, pos + 1) + ":";
        LOG_D("config::Options: Searching for server name: " << key);

        OptsMap::const_iterator it;
        for (it = blocks.begin(); it != blocks.end(); ++it) {
            if (it->first.find(key) != std::string::npos) { subs.insert(*it); }
        }
    }
    return (subs);
}

char const* Options::NoMatchingBlockException::what() const throw() {
    return ("config::Options: No matching block found");
}

/* Helpers */
static std::vector<smt::shared_ptr<Opts> >
    extractOpts(Options::OptsMap const& opts) {
    std::vector<smt::shared_ptr<Opts> > ret;

    Options::OptsMap::const_iterator it;
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
        Options::OptsMap opts;
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
                       Options::OptsMap&                            opts) {
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
                        Options::OptsMap&                            opts) {
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
            Options::OptsMap::iterator itt;
            for (itt = opts.begin(); itt != opts.end(); ++itt) {
                (*it)->set(itt->second);
            }
        }
    }
}

void extractBlock(std::vector<DirectiveTypeTraitsBase*> const& directives,
                  Options::OptsMap&                            opts) {
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
