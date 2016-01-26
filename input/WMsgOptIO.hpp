#ifndef WMSG_OPT_IO_HPP_
#define WMSG_OPT_IO_HPP_

#include "OptIO.hpp"

namespace reprize
{
namespace inp
{
class WMsgOptIO : public OptIO
{
public:
	WMsgOptIO(reprize::Win32DepInfo* dep_info_);
    virtual ~WMsgOptIO(void);

    virtual const bool init(void);
    virtual void restore(void);

    virtual Int32 get_stick(void);
    virtual Int32 get_trigger(void);

    virtual void check(void);

private:
	reprize::Win32DepInfo* win32_info;
};
}
}
#endif
