#ifndef VFS_HPP_
#define VFS_HPP_

#include "Node.hpp"
#include "Root.hpp"

namespace reprize
{
namespace res
{
class XML;

class VFS
{
public:
    VFS(uInt32 alloc_mem_ = 64); // megabyte
    virtual ~VFS(void);

    void init(void);

    // add system node or resources
    const bool add_to_root(Node* src_);
    const bool add_to_sys(Node* src_);

    // read xml tree
    const bool read(XML* xml_, Node* parent_);

    // get memory object
    Node* find(const Str& name_);

    // change memory object
    void apply_props(Props* pr_, const StrMap& atts_);

protected:
    Property* get_prop(const Str& prop_str_);
    Props* get_props(const Str& prop_str_);
    void debug(Node* node_ = NULL);
    Root root;
    Node sys;
    uInt32 empty_node;
};
};
}

#endif
