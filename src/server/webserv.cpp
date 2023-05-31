#include "webserv.hpp"

namespace webserv {

static void stop(int) {}

void webserv(std::string const& filename) {
    signal(SIGINT, stop);

    LOG_I("Reading config file: " + filename << "...");
    try {
        std::vector< smt::shared_ptr<config::Opts> > opts =
            config::parse(filename);
        config::Options::getInstance(opts);
    }
    catch (std::exception& e) {
        LOG_E("Error: " + std::string(e.what()));
        return;
    }
}

} // namespace webserv
