#include "webserv.hpp"

// TODO: test with valgrind
// TODO: make mimetypes in utils and mabe others
// TODO: make sure all cgi scripts have python 3 to them
// TODO: check for all stringstreams that could be replaced for stoi
// TODO: here maybe make all the alone functions inside classes, as statics
int main(int argc, char const* argv[]) {
    (void)argc;
    (void)argv;

    std::string filename("config/webserv.conf");
    if (argc > 1) { filename = argv[1]; }
    webserv::webserv(filename);
    return (0);
}