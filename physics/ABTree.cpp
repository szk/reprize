#include "Common.hpp"

#include "resource/Actor.hpp"
#include "ABTree.hpp"

using namespace reprize;
using namespace res;
using namespace res;
using namespace std;

ABTree::ABTree(const Str& name_, const res::VFS* vfs_)
    : Node(name_), vfs(vfs)
{
}

ABTree::~ABTree(void)
{
}
