#ifndef PROPERTY_HPP_
#define PROPERTY_HPP_

#include "Node.hpp"

namespace reprize
{
namespace res
{
enum PropertyType
{
    PROPERTY_STR,
    PROPERTY_NODE,
    PROPERTY_PROP,
    PROPERTY_URL,  // XXX not implemented
    PROPERTY_BYTE, // XXX not implemented
    PROPERTY_NUM
};

class Property
{
public:
    virtual ~Property(void) {}
    virtual const Str& Apply(const Str& arg_) { return RE_FAILURE; }
    virtual const Str& Apply(Node* arg_) { return RE_FAILURE; }
    virtual const Str& Apply(Property* arg_) { return RE_FAILURE; }
    virtual const PropertyType get_type(void) { return PROPERTY_NUM; }
    virtual inline const Str& get_str(void) { return VFS_NONAME; }
    virtual inline const Node* get_node(void) { return NULL; }

protected:
    Property(void) {}
};
}
}

#endif
