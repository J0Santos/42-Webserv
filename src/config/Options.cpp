#include "config/Options.hpp"

namespace config {

Options::Options(std::vector<ServerOpts> const& options) : m_options(options) {}

Options::~Options(void) {}

smt::shared_ptr<Options>
    Options::getInstance(std::vector<ServerOpts> const& options) {
    static smt::shared_ptr<Options> ist(new Options(options));
    return (ist);
}

} // namespace config
