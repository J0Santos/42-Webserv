#include <cstring>
#include <string>
#include <vector>

namespace ft {

class string : public std::string {
    public:

        string(void);
        string(char const* s);
        string(std::string const& s);

        std::vector<string> split(string const& delimiter) const;
        string&             trim(void);
};

} // namespace ft