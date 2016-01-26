#ifndef SCRPT_LOADER_HPP_
#define SCRPT_LOADER_HPP_

#include "Loader.hpp"
#include "util/scm/mscm.hpp"

namespace reprize
{
namespace res
{
class Script;

class ScrptLoader : public Loader
{
public:
    ScrptLoader(VFS* vfs_);
    virtual ~ScrptLoader(void);

    Node* create_sys_node(void);

    const bool load(const Str& filename_, const Str& path_);
    const bool add_proc(Str& name_, void* func_);

protected:
    Script* build(const Str& name_);

    scm::Scm<int, Str>* mscm;
    VFS* vfs;
    Script* def_scrpt;
};
}
}
#endif
