#ifndef PROP_STR_HPP_
#define PROP_STR_HPP_

#include "Property.hpp"

// Command pattern
namespace reprize
{
namespace res
{
template <class Receiver>
class PropStr : public Property
{
public:
    typedef const Str& (Receiver::* Action)(const Str&);
    PropStr(Receiver* receiver_, Action action_) :
        receiver(receiver_), action(action_) {}
    virtual ~PropStr(void) {}
    const Str& Apply(const Str& arg_)
        {
            argument = arg_;
            if (receiver) { return (receiver->*action)(arg_); }
            return RE_FAILURE;
        }

    const PropertyType get_type(void) { return PROPERTY_STR; }
    virtual inline const Str& get_str(void) { return argument; }
    virtual inline const Node* get_node(void) { return NULL; }

protected:
    Receiver* receiver;
    Action action;
    Str argument;
};
}
}

#endif
