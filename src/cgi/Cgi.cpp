#include "cgi/Cgi.hpp"

#include "utils/Logger.hpp"

namespace cgi {

std::vector<std::string> splitInfoFromPath(std::string const& path) {

    std::vector<std::string> subs(2);

    size_t pos = path.find(".");
    while (pos != std::string::npos) {
        std::string ext = path.substr(pos);
        ext = (ext.find("/") != std::string::npos)
                  ? ext.substr(0, ext.find("/"))
                  : ext;
        if (cgi::convertCgiExtension(ext) != cgi::Unknown) {
            subs[0] = path.substr(0, pos + ext.size());
            subs[1] = path.substr(pos + ext.size());
            break;
        }
        pos = path.find(".", pos + 1);
    }

    return (subs);
}

CgiType convertCgiExtension(std::string const& cgiExtension) {
    if (cgiExtension == ".py") { return (Py); }
    if (cgiExtension == ".php") { return (Php); }
    if (cgiExtension == ".cgi") { return (Cgi); }
    return (Unknown);
}

} // namespace cgi
