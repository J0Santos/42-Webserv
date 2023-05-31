#include "webserv.hpp"

namespace webserv {

void webserv(std::string const& filename) {
    config::Parser::getInstance(filename);
}

} // namespace webserv
