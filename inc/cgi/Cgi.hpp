#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <vector>

namespace cgi {

enum CgiType { Py, Php, Cgi, Unknown };

CgiType convertCgiExtension(std::string const& cgiExtension);

std::vector<std::string> splitInfoFromPath(std::string const& path);

} // namespace cgi

#endif /* CGI_HPP */