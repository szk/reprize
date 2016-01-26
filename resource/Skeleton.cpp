#include "Common.hpp"

#include "Bone.hpp"
#include "Skeleton.hpp"

using namespace reprize;
using namespace res;
using namespace std;

Skeleton::Skeleton(const Str name_)
    : Node(name_)
{
}

Skeleton::~Skeleton(void)
{
}

const bool Skeleton::add_bone(Bone* bone_)
{
    return false;
}
