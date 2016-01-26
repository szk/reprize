#ifndef SKELETON_HPP_
#define SKELETON_HPP_

#include "Node.hpp"

namespace reprize
{
namespace res
{
class Skeleton : public Node
{
public:
    Skeleton(const Str name_);
    virtual ~Skeleton(void);

    inline const bool add_bone(class Bone* bone_);

protected:
};
}
}

#endif
