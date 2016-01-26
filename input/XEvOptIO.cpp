#include "Common.hpp"

#include "misc/X11DepInfo.hpp"
#include "OptIO.hpp"
#include "XEvOptIO.hpp"

using namespace reprize;
using namespace inp;

XEvOptIO::XEvOptIO(X11DepInfo* dep_info_)
    : OptIO(dep_info_), x11_info(dep_info_)
{
}

XEvOptIO::~XEvOptIO(void)
{
}

const bool XEvOptIO::init(void)
{
    return true;
}

void XEvOptIO::restore(void)
{
}

Int32 XEvOptIO::get_stick(void)
{
    return 0;
}

Int32 XEvOptIO::get_trigger(void)
{
    return 0;
}

void XEvOptIO::check(void)
{
}
