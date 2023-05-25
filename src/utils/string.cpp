#include "utils/string.hpp"

namespace ft {

string::string(void) : std::string() {}

string::string(char const* s) : std::string(s) {}

string::string(std::string const& s) : std::string(s) {}

std::vector<string> string::split(string const& delimiter) const {
    std::vector<string> substrings;

    char* word = strtok(const_cast<char*>(this->c_str()), delimiter.c_str());
    while (word) {
        substrings.push_back(word);
        word = strtok(NULL, delimiter.c_str());
    }
    return (substrings);
}

string& string::trim(void) {
    size_type start = find_first_not_of(" \t\n\r\f\v");
    if (start != npos) { *this = substr(start); }
    else {
        clear();
        return (*this);
    }

    size_type end = find_last_not_of(" \t\n\r\f\v");
    if (end != npos) { *this = substr(0, end + 1); }

    return (*this);
}
} // namespace ft