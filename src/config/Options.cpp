#include "config/Options.hpp"

namespace config {

std::vector<DirectiveTypeTraitsBase*>
    getNextBlock(std::vector<DirectiveTypeTraitsBase*> const& dirs) {

    static std::vector<DirectiveTypeTraitsBase*> directives = dirs;

    if (!dirs.empty()) { directives = dirs; }

    int level = 0;

    std::vector<DirectiveTypeTraitsBase*>                 block;
    std::vector<DirectiveTypeTraitsBase*>::const_iterator it;
    for (it = directives.begin(); it != directives.end(); ++it) {
        if ((*it)->getName() == BLOCK_SERVER) { ++level; }
        if ((*it)->getName() == BLOCK_LOCATION) { ++level; }
        else if ((*it)->getName() == BLOCK_END) { --level; }

        block.push_back(*it);
        if (level == 0) { break; }
    }
    if (level != 0) { throw std::exception(); }
    else {
        directives.erase(directives.begin(), directives.begin() + block.size());
    }
    return (block);
}

std::vector< smt::shared_ptr<Opts> >
    extract(std::vector<DirectiveTypeTraitsBase*> const& directives) {
    std::vector< smt::shared_ptr<Opts> > opts;
    if (directives.empty()) { return (opts); }
    std::vector<DirectiveTypeTraitsBase*> blocks = getNextBlock(directives);
    while (!blocks.empty()) {
        LOG_D("blocks.size(): " << blocks.size());
        blocks = getNextBlock();
    }
    return (opts);
}

} // namespace config
