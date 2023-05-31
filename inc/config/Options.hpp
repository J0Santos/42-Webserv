#ifndef CONFIG_OPTIONS_HPP
#define CONFIG_OPTIONS_HPP

#include "config/directives.hpp"

namespace config {

struct Opts {
        virtual ~Opts(void) {}
};

std::vector< smt::shared_ptr<Opts> >
    extract(std::vector<DirectiveTypeTraitsBase*> const& directives);

std::vector<DirectiveTypeTraitsBase*>
    getNextBlock(std::vector<DirectiveTypeTraitsBase*> const& dirs =
                     std::vector<DirectiveTypeTraitsBase*>());

} // namespace config

#endif /* CONFIG_OPTIONS_HPP */