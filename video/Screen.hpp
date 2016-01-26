#ifndef SCREEN_HPP_
#define SCREEN_HPP_

class EnvDepInfo;

namespace reprize
{
namespace vid
{
class Screen
{
public:
    Screen(EnvDepInfo* depinfo_) {}
    virtual ~Screen(void) {}

    virtual void init(void) {}
    virtual void appear(void) {}

    virtual void begin_paint(void) {}
    virtual void finish_paint(void) {}
    virtual const bool flip(void) { return false; }

    virtual void release(void) {}
};
}
}

#endif
