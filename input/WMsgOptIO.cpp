#include "Common.hpp"

#include "WMsgOptIO.hpp"
#include "OptIO.hpp"
using namespace reprize;
using namespace inp;

WMsgOptIO::WMsgOptIO(Win32DepInfo* dep_info)
    : OptIO(dep_info), win32_info(dep_info)
{
}

WMsgOptIO::~WMsgOptIO(void)
{
}

const bool WMsgOptIO::init(void)
{
    return true;
}

void WMsgOptIO::restore(void)
{
}

Int32 WMsgOptIO::get_stick(void)
{
    return 0;
}

Int32 WMsgOptIO::get_trigger(void)
{
    return 0;
}

void WMsgOptIO::check(void)
{
}
