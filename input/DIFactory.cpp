#include "Common.hpp"
#include "Win32DepInfo.hpp"

#include "DIFactory.hpp"

#include "DIBase.hpp"
#include "DIKeyboard.hpp"
#include "DIMouse.hpp"
#include "DIOptIO.hpp"

using namespace reprize;

DIFactory::DIFactory(Win32DepInfo* dep_info)
: InputFactory(dep_info), win32_info(dep_info)
{
}

DIFactory::~DIFactory(void)
{
}

InputBase* DIFactory::makeBase(void) const
{
    return new DIBase(win32_info);
}

Keyboard* DIFactory::makeKeyboard(void) const
{
    return new DIKeyboard(win32_info);
}

Mouse* DIFactory::makeMouse(void) const
{
    return new DIMouse(win32_info);
}

OptIO* DIFactory::makeOptIO(void) const
{
    return new DIOptIO(win32_info);
}
