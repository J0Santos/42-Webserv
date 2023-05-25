#include "utils/FileSystem.hpp"

namespace ft {

/* directory */
directory::directory(void) : std::string() {}

directory::directory(char const* s) : std::string(s) { addTrailingSlash(); }

directory::directory(std::string const& s) : std::string(s) {
    addTrailingSlash();
}

directory& directory::operator=(std::string const& s) {
    std::string::operator=(s);
    addTrailingSlash();
    return (*this);
}

directory& directory::operator=(char const* s) {
    std::string::operator=(s);
    addTrailingSlash();
    return (*this);
}

bool directory::exists(void) const {
    struct stat s;
    if (stat((*this).c_str(), &s) == 0) {
        if (s.st_mode & S_IFDIR) { return (true); }
    }
    return false;
}

bool directory::isFile(void) const {
    std::string tmp = *this;
    if (!(*this).empty() && (*this)[size() - 1] == '/') {
        tmp = tmp.substr(0, size() - 1);
    }

    if (!exists()) {
        struct stat s;
        if (stat(tmp.c_str(), &s) == 0) {
            if (s.st_mode & S_IFREG) { return (true); }
        }
    }
    return (false);
}

bool directory::isCrawler(void) const {
    std::vector<string> dirs = string(*this).split("/");

    int count = 0;
    for (std::vector<string>::iterator it = dirs.begin(); it != dirs.end();
         ++it) {
        if (*it == "..") { count--; }
        else if (*it != ".") { count++; }
        if (count < 0) { return (true); }
    }
    return (false);
}

void directory::addTrailingSlash() {
    if (!(*this).empty() && (*this)[size() - 1] != '/') { *this += '/'; }
}

void directory::removeTrailingSlash() {
    if (!empty() && (*this)[size() - 1] == '/') {
        assign(substr(0, size() - 1));
    }
}

/* file */
file::file(void) : std::string() {}

file::file(char const* s) : std::string(s) {}

file::file(std::string const& s) : std::string(s) {}

bool file::exists(void) const {
    struct stat s;
    if (stat((*this).c_str(), &s) == 0) {
        if (s.st_mode & S_IFREG) { return (true); }
    }
    return (false);
}

bool file::isDirectory(void) const {
    struct stat s;
    if (stat((*this).c_str(), &s) == 0) {
        if (s.st_mode & S_IFDIR) { return (true); }
    }
    return (false);
}

std::string file::getExtension(void) const {
    std::string         ext;
    std::vector<string> subs = string(*this).split(".");

    if (subs.size() > 1) { ext = subs[subs.size() - 1]; }
    return (ext);
}

directory file::getDirectory(void) const {
    if (isDirectory()) { return (*this); }
    else {
        size_t      pos = find_last_of('/');
        std::string dir = substr(0, pos);
        if (dir.empty() || pos == npos) { dir = "./"; }
        return (directory(dir));
    }
}

} // namespace ft
