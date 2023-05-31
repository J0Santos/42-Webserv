#ifndef CONFIG_OPTIONS_HPP
#define CONFIG_OPTIONS_HPP

#include "config/directives.hpp"

namespace config {

class Options {
    public:

        static Options& getInstance(
            std::vector<DirectiveTypeTraitsBase*> const& directives =
                std::vector<DirectiveTypeTraitsBase*>());

        static std::vector<DirectiveTypeTraitsBase*> const& getDirectives(void);

    private:

        Options(void);
        Options(std::vector<DirectiveTypeTraitsBase*> const& directives);
        Options(Options const& src);

        ~Options(void);

        Options& operator=(Options const& rhs);

        std::vector<DirectiveTypeTraitsBase*> m_directives;
};

} // namespace config

#endif /* CONFIG_OPTIONS_HPP */