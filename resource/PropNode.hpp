#ifndef PROP_NODE_HPP_
#define PROP_NODE_HPP_

// Command pattern
namespace reprize
{
namespace res
{
template <class Receiver>
class PropNode : public Property
{
public:
    typedef const Str& (Receiver::* Action)(Node*);
    PropNode(Receiver* receiver_, Action action_) :
        receiver(receiver_), action(action_) {}
    virtual ~PropNode(void) {}
    const Str& Apply(Node* arg_)
        {
            argument = arg_;
            if (receiver) { return (receiver->*action)(arg_); }
            return RE_FAILURE;
        }
    const PropertyType get_type(void) { return PROPERTY_NODE; }
    virtual inline const Str& get_str(void) { return VFS_NONAME; }
    virtual inline const Node* get_node(void) { return argument; }

protected:
    Receiver* receiver;
    Action action;
    Node* argument;
};
}
}

#endif
