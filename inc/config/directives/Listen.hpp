#ifndef DIRECTIVE_LISTEN_HPP
#define DIRECTIVE_LISTEN_HPP

#include <string>
#include <vector>

namespace directive {

class Listen {
    public:

        Listen(std::vector<std::string> args);
        ~Listen(void);

        bool isValid(void) const;
        void extract(void* dest);

    private:

        bool m_valid;

        std::string m_host;
        std::string m_port;
};

} // namespace directive

#endif /* DIRECTIVE_LISTEN_HPP */