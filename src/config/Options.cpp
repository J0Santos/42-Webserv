#include "config/Options.hpp"

namespace config {

std::vector<DirectiveTypeTraitsBase*>
    getNextBlock(std::vector<DirectiveTypeTraitsBase*> const& dirs) {

    std::vector<DirectiveTypeTraitsBase*> directives(dirs);

    int level = 0;

    std::vector<DirectiveTypeTraitsBase*>                 block;
    std::vector<DirectiveTypeTraitsBase*>::const_iterator it;
    if (directives.front()->getName() != BLOCK_SERVER) { return (block); }
    for (it = directives.begin(); it != directives.end(); ++it) {
        if ((*it)->getName() == BLOCK_SERVER) { ++level; }
        if ((*it)->getName() == BLOCK_LOCATION) { ++level; }
        else if ((*it)->getName() == BLOCK_END) { --level; }

        block.push_back(*it);
        if (level == 0) { break; }
    }
    return (block);
}

std::vector< smt::shared_ptr<Opts> >
    extract(std::vector<DirectiveTypeTraitsBase*> const& directives) {
    std::vector< smt::shared_ptr<Opts> > opts;
    if (directives.empty() || directives.back()->getName() != BLOCK_END) {
        return (opts);
    }
    // get the first block
    // extract the first block
    // get the next block extract
    return (opts);
}

} // namespace config
