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

        typedef std::map< std::string, smt::shared_ptr<Opts> > OptsMap;

        static smt::shared_ptr<Options>
            getInstance(std::vector< smt::shared_ptr<Opts> > opts =
                            std::vector< smt::shared_ptr<Opts> >());

        static smt::shared_ptr<Opts> getOpts(std::string port, std::string host,
                                             std::string path,
                                             std::string header = "");

        static smt::shared_ptr<Opts> getOpts(std::string key);

        ~Options(void);

        struct NoMatchingBlockException : public std::exception {
                virtual char const* what() const throw();
        };

    private:

        OptsMap getAllOpts(void) const;

        static OptsMap getBlocksWithPortAndHost(std::string const& specs,
                                                OptsMap const&     blocks);

        static OptsMap getBlocksFromServerName(std::string const& specs,
                                               OptsMap const&     blocks);

        Options(void);
        Options(std::vector< smt::shared_ptr<Opts> > const& opts);
        Options(Options const& rhs);

        Options& operator=(Options const& rhs);

        OptsMap m_opts;
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