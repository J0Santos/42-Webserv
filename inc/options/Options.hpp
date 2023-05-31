#ifndef CONFIG_OPTIONS_HPP
#define CONFIG_OPTIONS_HPP

#include "options/ServerOpts.hpp"
#include "utils/smt.hpp"

#include <vector>

namespace config {

class Options {
    private:

        Options(void);
        Options(Options const& src);

        ~Options(void);

        Options& operator=(Options const& rhs);

        std::vector< smt::shared_ptr<ServerOpts> > m_options;
};

} // namespace config

#endif /* CONFIG_OPTIONS_HPP */