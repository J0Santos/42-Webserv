#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "utils/Logger.hpp"
#include "utils/string.hpp"

#include <string>
#include <sys/stat.h>

namespace ft {

class directory : public std::string {
    public:

        directory(void);
        directory(char const* s);
        directory(std::string const& s);

        directory& operator=(std::string const& s);
        directory& operator=(char const* s);

        bool exists(void) const;
        bool isCrawler(void) const;
        bool isFile(void) const;

    private:

        void addTrailingSlash(void);
        void removeTrailingSlash(void);
};

class file : public std::string {
    public:

        file(void);
        file(char const* s);
        file(std::string const& s);

        bool        exists(void) const;
        bool        isDirectory(void) const;
        std::string getExtension(void) const;
        directory   getDirectory(void) const;
};

} // namespace ft

#endif /* FILESYSTEM_HPP */