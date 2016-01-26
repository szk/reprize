#ifndef VAR_LOADER_HPP_
#define VAR_LOADER_HPP_

#include "Loader.hpp"

namespace reprize
{
namespace res
{
class XML;

class VarLoader : public Loader
{
public:
    VarLoader(VFS* vfs_, Node* init_def_);
    virtual ~VarLoader(void);

    const bool load(const Str& filename_, const Str& path_);
    const bool read(const Char* content_, VFS* vfs_, Node* parent_);
    const bool config(const Char* content_, VFS* vfs_, Node* parent_);
    const bool assemble(Node* define_);

protected:
    void crawl(XML* xml_, Node* def_node_, Node* parent_node_);

    VFS* vfs;
    Node* init_def;
    std::list<std::pair<XML*, Node*> >docs;
    Props* tmp_props;
};
}
}
#endif
