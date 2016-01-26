#ifndef POSIX_FS_READER_HPP_
#define POSIX_FS_READER_HPP_

#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

#include "FSReader.hpp"

namespace reprize
{
namespace res
{
class VisLoader;
class Loader;
class VarLoader;
class VFS;

class POSIXFSReader : public FSReader
{
public:
    POSIXFSReader(void);
    virtual ~POSIXFSReader(void);

    const bool check(VFS* vfs_,  const Str& dir_);

protected:
    void crawl(Node* cwn_);
    Node* load(const Str filename_, const Str fullpath_, Node* cwn_);

    VFS* vfs;
    Node* fs_root;
    Char fullpath[PATH_MAX+1];
};
}
}
#endif
