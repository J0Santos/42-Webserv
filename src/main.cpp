#include "config/Parser.hpp"
#include "utils/Logger.hpp"

int main(int argc, char const* argv[]) {
    (void)argc;
    (void)argv;

    LOG_D("Webserv " << 2.0);
    std::string filename("config/webserv.conf");
    if (argc > 1) { filename = argv[1]; }
    webserv::webserv(filename);
    return (0);
}