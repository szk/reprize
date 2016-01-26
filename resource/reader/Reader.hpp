#ifndef READER_H_
#define READER_H_

namespace reprize
{
namespace res
{
class VFS;
class Loader;

class Reader
{
public:
    virtual ~Reader(void) {}

    const bool add_loader(Loader* loader_)
        {
            loader_l.push_back(loader_);
            return true;
        }
    virtual const bool check(VFS* vfs_,  const Str& location_) { return false; }

protected:
    Reader(void) {}
    std::list<Loader*> loader_l;
};
}
}
#endif
