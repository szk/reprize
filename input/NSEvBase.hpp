#ifndef NSEV_BASE_HPP_
#define NSEV_BASE_HPP_

#include "InputBase.hpp"
#include "misc/NSDepInfo.hpp"

namespace reprize
{
namespace inp
{
class NSEvBase : public InputBase
{
public:
    NSEvBase(NSDepInfo* dep_info_) : InputBase(dep_info_), ns_info(dep_info_) {}
    virtual ~NSEvBase(void) {}

    virtual const bool init(void)
        {
            InputBase::init();
            return true;
        }

    virtual void restore(void) { }
    virtual void check(Keyboard* keyboard_, Mouse* mouse_, OptIO* opt_)
        {
	    mouse_->check(pressed, released);
        }

protected:
    NSDepInfo* ns_info;
};
}
}
#endif
