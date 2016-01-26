#ifndef WIN32_FS_READER_HPP_
#define WIN32_FS_READER_HPP_

#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

#include "FSReader.hpp"

namespace reprize
{
namespace res
{
class TexLoader;
class GeomLoader;
class ResLoader;
class VarLoader;
class VFS;

class Win32FSReader : public FSReader
{
public:
    Win32FSReader(void);
    virtual ~Win32FSReader(void);

    const bool check(VFS* vfs_, const Str& dir_);

private:
    void crawl(Node* cwn_);
    Node* load(const Str filename_, const Str fullpath_, Node* cwn_);
    const Str fix_path(const Str& src_);

    VFS* vfs;
    Node* fs_root;
    TCHAR fullpath[MAX_PATH + 1];
};
}
}
#endif
