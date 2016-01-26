#ifndef SOUND_HPP_
#define SOUND_HPP_

#include "Node.hpp"

namespace reprize
{
namespace res
{
class Sound : public Node
{
public:
    Sound(const Str name_, const uInt32 buffer_id_, Matter* mtt_ = NULL)
        : Node(name_, NULL, true), id(buffer_id_)
        {}
    virtual ~Sound(void) {}

protected:
    const uInt32 id;
};
}
}

#endif
