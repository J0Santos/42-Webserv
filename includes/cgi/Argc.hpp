#ifndef CGI_ARGC_HPP
#define CGI_ARGC_HPP

#include "utils/ft_array.hpp"

namespace cgi {

class Argc {

    public:

        Argc(void);
        Argc(std::vector<std::string> const& argc);
        Argc(Argc const& src);

        ~Argc(void);

        Argc& operator=(Argc const& rhs);

        operator char**(void) const;

    private:

        std::vector<std::string> m_argc;
};

} // namespace cgi

#endif /* CGI_ARGC_HPP */