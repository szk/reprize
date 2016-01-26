#ifndef PROP_PROP_HPP_
#define PROP_PROP_HPP_

// Command pattern
namespace reprize
{
namespace res
{
template <class Receiver>
class PropProp : public Property
{
public:
    typedef const Str& (Receiver::* Action)(Property*);
    PropProp(Receiver* receiver_, Action action_) :
        receiver(receiver_), action(action_) {}
    virtual ~PropProp(void) {}
    const Str& Apply(Property* arg_)
        {
            argument = arg_;
            if (receiver) { return (receiver->*action)(arg_); }
            return RE_FAILURE;
        }
    const PropertyType get_type(void) { return PROPERTY_PROP; }
    const Property* get_last_argument(void) { return argument; }
    virtual inline operator const Property*() const { return argument; }

protected:
    Receiver* receiver;
    Action action;
    Property* argument;
};
}
}

#endif
