#ifndef ABTREE_HPP_
#define ABTREE_HPP_

namespace  reprize
{
namespace res { class VFS; }

namespace res
{
class ABTree : public res::Node
{
public:
    ABTree(const Str& name_, const res::VFS* vfs_);
    virtual ~ABTree(void);

protected:
    res::VFS* vfs;
};
}
}

#endif
