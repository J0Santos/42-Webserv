#include "config/Parser.hpp"
#include "utils/Logger.hpp"

int main(int argc, char const* argv[]) {
    (void)argc;
    (void)argv;

    std::string filename("config/webserv.conf");
    if (argc > 1) { filename = argv[1]; }
    try {
        config::parse(filename);
    }
    catch (std::exception const& e) {
        LOG_E(e.what());
    }

    LOG_D("Webserv " << 2.0);
    return (0);
}