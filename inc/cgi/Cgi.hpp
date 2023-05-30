#ifndef CGI_HPP
#define CGI_HPP

#include <string>

namespace cgi {

enum CgiType { Py, Php, Cgi, Unknown };

CgiType convertCgiExtension(std::string const& cgiExtension);

} // namespace cgi

#endif /* CGI_HPP */