#ifndef LOADER_HPP_
#define LOADER_HPP_

#include "../default_res.hpp"

namespace reprize
{
namespace res
{
class Loader : public Node
{
public:
    virtual ~Loader(void) {}

    virtual Node* create_sys_node(void) { return NULL; }
    virtual const bool load(const Str& filename_, const Str& path_)
        { return false; }
    Node* get(void) { return loaded_node; }
    const Str& get_type(void) { return type; }

protected:
    Loader(const Str& type_) : Node(type_), loaded_node(NULL), type(type_) {}
    const bool compare_extension(const Str& filename_, const Str& extension_)
        {
            if (extension_.size() > filename_.size()) { return false; }
            Str ext = extension_;
            Str src_ext = filename_.substr(filename_.size() - ext.size(),
                                           ext.size());
            transform(src_ext.begin(), src_ext.end(), src_ext.begin(),
                      (Int32(*)(Int32))tolower);
            transform(ext.begin(), ext.end(), ext.begin(),
                      (Int32(*)(Int32))tolower);

            return ext == src_ext;
        }

    Node* loaded_node;
    const Str type;
};
}
}
#endif
