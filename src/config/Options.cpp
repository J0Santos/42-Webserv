#include "config/Options.hpp"

#include "utils/ft_string.hpp"

namespace config {

/* Options */
Options::Options(std::vector<ServerOpts> const& options) : m_options(options) {
    // setupDefaultRoutes();
}

Options::~Options(void) {}

smt::shared_ptr<Options>
    Options::getInstance(std::vector<ServerOpts> const& options) {
    static smt::shared_ptr<Options> ist(new Options(options));
    if (options.size() > 0) { ist = smt::make_shared(new Options(options)); }
    return (ist);
}

smt::shared_ptr<Opts> Options::getOptions(std::string const& port,
                                          std::string const& host,
                                          std::string const& path,
                                          std::string const& header) {
    std::vector<ServerOpts> options = getInstance()->m_options;
    smt::shared_ptr<Opts>   opts;

    // Get The Server Block with the Port And Host And Header
    std::vector<ServerOpts>::const_iterator serverIt = options.end();
    for (std::vector<ServerOpts>::const_iterator it = options.begin();
         it != options.end(); ++it) {

        if ((*it).m_port == port && (*it).m_host == host) {
            if (serverIt == options.end()) { serverIt = it; }
            else if ((*it).m_server_name == header) { serverIt = it; }
        }
    }
    if (serverIt == options.end()) {
        LOG_F("Failed to find options for " << port << ":" << host);
        throw NoSuchOptionsException();
    }

    // Find Location Block With Target that best Matches request path
    std::vector<std::string> pathVector = ft::string::split(path, "/");
    int                      oldCount = -1;

    std::vector<LocationOpts>::const_iterator locationIt =
        (*serverIt).m_locations.end();
    for (std::vector<LocationOpts>::const_iterator it =
             (*serverIt).m_locations.begin();
         it != (*serverIt).m_locations.end(); ++it) {

        std::vector<std::string> target =
            ft::string::split((*it).m_target, "/");

        int count = getCountOfDirs(pathVector, target);
        if (count > oldCount) {
            locationIt = it;
            oldCount = count;
        }
    }

    if (locationIt == (*serverIt).m_locations.end()) {
        return (smt::make_shared(new Opts(*serverIt)));
    }
    return (smt::make_shared(new Opts(*serverIt, *locationIt)));
}

std::set< std::pair<int, std::string> > Options::getSocketOptions(void) {
    std::vector<ServerOpts> options = getInstance()->m_options;

    static std::set< std::pair<int, std::string> > sockOpts;
    for (std::vector<ServerOpts>::const_iterator it = options.begin();
         it != options.end(); ++it) {

        std::stringstream ss((*it).m_port);
        int               port;
        ss >> port;

        std::pair<int, std::string> opt(port, (*it).m_host);
        if (sockOpts.find(opt) == sockOpts.end()) { sockOpts.insert(opt); }
    }
    return (sockOpts);
}

int Options::getCountOfDirs(std::vector<std::string> const& path,
                            std::vector<std::string> const& target) {
    int count = 0;
    for (size_t i = 0; i < path.size() && i < target.size(); i++) {
        if (path[i] == target[i]) { count++; }
        else { break; }
    }
    return (count);
}

char const* Options::NoSuchOptionsException::what(void) const throw() {
    return ("config::Options: no such options");
}

/* Opts */
Opts::Opts(void) : m_max_body_size(0), m_autoindex(false) {}

Opts::Opts(ServerOpts const& srv, LocationOpts const& loc) {
    // Extracting the options from the server block and location block
    m_target = (loc.m_target.empty() ? "/" : loc.m_target);
    m_host = srv.m_host;
    m_port = srv.m_port;
    m_root = (std::string(loc.m_root).empty() ? srv.m_root : loc.m_root);
    m_server_name = srv.m_server_name;
    m_error_pages =
        (loc.m_error_pages.empty() ? srv.m_error_pages : loc.m_error_pages);
    m_max_body_size =
        (!loc.m_max_body_size ? srv.m_max_body_size : loc.m_max_body_size);
    m_allowed_methods = (loc.m_allowed_methods.empty() ? srv.m_allowed_methods
                                                       : loc.m_allowed_methods);
    m_index = loc.m_index;
    m_autoindex = loc.m_autoindex;
    m_cgi_extension = loc.m_cgi_extension;

    // Setting up the default values
    if (!m_max_body_size) { m_max_body_size = DEFAULT_BODY_SIZE; }
    if (m_allowed_methods.empty()) {
        std::string arr[3] = {"GET", "POST", "DELETE"};
        m_allowed_methods = std::vector<std::string>(arr, arr + 3);
    }
}

Opts::Opts(ServerOpts const& srv) {
    // Extracting the options from the server block
    m_target = "/";
    m_host = srv.m_host;
    m_port = srv.m_port;
    m_root = srv.m_root;
    m_server_name = srv.m_server_name;
    m_error_pages = srv.m_error_pages;
    m_max_body_size = srv.m_max_body_size;
    m_allowed_methods = srv.m_allowed_methods;

    // Setting up the default values
    if (!m_max_body_size) { m_max_body_size = DEFAULT_BODY_SIZE; }
    if (m_allowed_methods.empty()) {
        std::string arr[3] = {"GET", "POST", "DELETE"};
        m_allowed_methods = std::vector<std::string>(arr, arr + 3);
    }
}

Opts::~Opts(void) {}

bool Opts::operator==(Opts const& rhs) const {
    return (m_target == rhs.m_target && m_host == rhs.m_host &&
            m_port == rhs.m_port && m_root == rhs.m_root &&
            m_server_name == rhs.m_server_name &&
            m_error_pages == rhs.m_error_pages &&
            m_max_body_size == rhs.m_max_body_size &&
            m_allowed_methods == rhs.m_allowed_methods &&
            m_index == rhs.m_index && m_autoindex == rhs.m_autoindex &&
            m_cgi_extension == rhs.m_cgi_extension);
}

std::ostream& operator<<(std::ostream& os, Opts const& rhs) {
    os << "Opts: " << std::endl
       << "target: " << rhs.m_target << std::endl
       << "host: " << rhs.m_host << std::endl
       << "port: " << rhs.m_port << std::endl
       << "root: " << rhs.m_root << std::endl
       << "server_name: " << rhs.m_server_name << std::endl
       << "max_body_size: " << rhs.m_max_body_size << std::endl
       << "index: " << rhs.m_index << std::endl
       << "autoindex: " << rhs.m_autoindex << std::endl
       << "cgi_extension: " << rhs.m_cgi_extension << std::endl
       << "error_pages:";
    for (std::map<int, ft::file>::const_iterator it = rhs.m_error_pages.begin();
         it != rhs.m_error_pages.end(); ++it) {
        os << it->first << " (" << std::string(it->second) << ")";
    }
    os << std::endl << "allowed_methods:";
    for (std::vector<std::string>::const_iterator it =
             rhs.m_allowed_methods.begin();
         it != rhs.m_allowed_methods.end(); ++it) {
        os << " " << *it;
    }
    os << std::endl;
    return (os);
}

} // namespace config
