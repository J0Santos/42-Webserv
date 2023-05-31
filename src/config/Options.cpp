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
    if (serverIt == options.end()) { throw std::exception(); }

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

    if (locationIt == (*serverIt).m_locations.end()) { throw std::exception(); }

    // Convert to Opts
    return (smt::make_shared(new Opts(*serverIt, *locationIt)));
}

int Options::getCountOfDirs(std::vector<std::string> const& cmd,
                            std::vector<std::string> const& target) {
    int count = 0;
    for (size_t i = 0; i < cmd.size() && i < target.size(); i++) {
        if (cmd[i] == target[i]) { count++; }
        else { break; }
    }
    return (count);
}

/* Opts */
Opts::Opts(ServerOpts const& srv, LocationOpts const& loc) {
    // Extracting the options from the server block and location block
    m_target = loc.m_target;
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
    if (!m_max_body_size) { m_max_body_size = 1000000; }
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

} // namespace config
