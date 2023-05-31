#ifndef CONFIG_OPTIONS_HPP
#define CONFIG_OPTIONS_HPP

#include "config/directives.hpp"
#include "config/ServerOpts.hpp"
#include "utils/smt.hpp"

#include <vector>

namespace config {

class Opts;

class Options {
    public:

        ~Options(void);

        static smt::shared_ptr<Options> getInstance(
            std::vector<ServerOpts> const& options = std::vector<ServerOpts>());

        static smt::shared_ptr<Opts> getOptions(std::string const& port,
                                                std::string const& host,
                                                std::string const& path,
                                                std::string const& header = "");

    private:

        Options(void);
        Options(std::vector<ServerOpts> const& options);
        Options(Options const& src);

        Options& operator=(Options const& rhs);

        static int getCountOfDirs(std::vector<std::string> const& cmd,
                                  std::vector<std::string> const& target);

        std::vector<ServerOpts> m_options;
};

class Opts {

    public:

        ~Opts(void);
        Opts(void);
        Opts(ServerOpts const& serverOpts, LocationOpts const& locationOpts);

        bool operator==(Opts const& rhs) const;

        std::string              m_target;
        std::string              m_host;
        std::string              m_port;
        ft::directory            m_root;
        std::string              m_server_name;
        std::map<int, ft::file>  m_error_pages;
        unsigned long            m_max_body_size;
        std::vector<std::string> m_allowed_methods;
        ft::file                 m_index;
        bool                     m_autoindex;
        std::string              m_cgi_extension;
};

} // namespace config

#endif /* CONFIG_OPTIONS_HPP */