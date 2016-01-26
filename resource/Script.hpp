#ifndef SCRIPT_HPP_
#define SCRIPT_HPP_

#include "Node.hpp"
#include "Keyframe.hpp"

namespace reprize
{
namespace res
{
class Script : public Node
{
public:
    Script(Str name_, const std::list<Keyframe>& kf_l_)
        : Node(name_), kf_l(kf_l_)
        {
            // kf_l = kf_l_;
        }
    Script(Str name_)
        : Node(name_)
        {
        }

    virtual ~Script(void)
        {}

    inline const Keyframe& CurrentKf(void) const
        {
            return *kf_itr;
        }
    inline void FirstKf(void) { kf_itr = kf_l.begin(); }
    inline const bool IsDoneKf(void) { return kf_itr == kf_l.end(); }
    inline void NextKf(void) { ++kf_itr; }

    virtual void apply(Props* props_, Node* target_) {}

protected:
    std::list<Keyframe> kf_l;
    std::list<Keyframe>::const_iterator kf_itr;

    Node* parent;
};
}
}
#endif
