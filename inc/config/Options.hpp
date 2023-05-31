#ifndef CONFIG_OPTIONS_HPP
#define CONFIG_OPTIONS_HPP

#include "config/directives.hpp"
#include "config/Parser.hpp"
#include "utils/smt.hpp"

namespace config {

class DirectiveTypeTraitsBase;
struct Opts;
template<LineType Directive>
struct DirectiveTypeTraits;

class Options {

    public:

        static smt::shared_ptr<Options>
            getInstance(std::vector< smt::shared_ptr<Opts> > opts =
                            std::vector< smt::shared_ptr<Opts> >());

    private:

        Options(void);
        Options(std::vector< smt::shared_ptr<Opts> > opts);
        Options(Options const& rhs);
        ~Options(void);

        Options& operator=(Options const& rhs);

        std::vector< smt::shared_ptr<Opts> > m_opts;
};

std::ostream& operator<<(std::ostream& os, Opts const& rhs);

std::vector< smt::shared_ptr<Opts> >
    extract(std::vector<DirectiveTypeTraitsBase*> const& directives);

std::vector<DirectiveTypeTraitsBase*>
    getNextBlock(std::vector<DirectiveTypeTraitsBase*> const& dirs =
                     std::vector<DirectiveTypeTraitsBase*>());

void extractBlock(std::vector<DirectiveTypeTraitsBase*> const&    directives,
                  std::map< std::string, smt::shared_ptr<Opts> >& opts);

} // namespace config

#endif /* CONFIG_OPTIONS_HPP */