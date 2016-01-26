#ifndef WMSG_BASE_HPP_
#define WMSG_BASE_HPP_

#include "InputBase.hpp"

namespace reprize
{
namespace inp
{
class WMsgBase : public InputBase
{
public:
    WMsgBase(reprize::Win32DepInfo* dep_info);
    virtual ~WMsgBase(void);

    virtual const bool init(void);
    virtual void restore(void);
    virtual void check(Keyboard* keyboard_, Mouse* mouse_, OptIO* opt_io_);

protected:
    reprize::Win32DepInfo* win32_info;
};
}
}
#endif
