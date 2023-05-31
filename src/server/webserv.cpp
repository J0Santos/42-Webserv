#include "webserv.hpp"

namespace webserv {

static void stop(int);

void webserv(std::string const& filename) {
    signal(SIGINT, stop);

    LOG_I("Reading config file: " << filename << "...");
    try {
        config::parse(filename);
    }
    catch (std::exception& e) {
        LOG_F("Failed parsing file " << filename << ": "
                                     << std::string(e.what()));
        return;
    }
}

static void stop(int) {}

} // namespace webserv
