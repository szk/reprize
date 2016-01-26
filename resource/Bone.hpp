#ifndef BONE_HPP_
#define BONE_HPP_

#include "Node.hpp"

namespace reprize
{
namespace res
{
class Bone : public Node
{
public:
    Bone(const Str name_);
    virtual ~Bone(void);

    inline const bool add_bone(class Bone* bone_);

protected:
};
}
}

#endif
