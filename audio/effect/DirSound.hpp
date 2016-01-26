#ifndef DIR_SOUND_HPP_
#define DIR_SOUND_HPP_

#include "Sound.hpp"

namespace reprize
{
namespace res
{
class DirSound : public Sound
{
public:
    DirSound(const Str name_, const uInt32 buffer_id_, Matter* mtt_ = NULL)
        : Sound(name_, buffer_id_, mtt_)
        {}
    virtual ~Sound(void) {}

protected:
    const uInt32 id;
};
}
}

#endif
