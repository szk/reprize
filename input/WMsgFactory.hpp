#ifndef WMSG_FACTORY_HPP_
#define WMSG_FACTORY_HPP_
#include "WMsgOptIO.hpp"
#include "InputFactory.hpp"

#include "WMsgBase.hpp"
#include "WMsgKeyboard.hpp"
#include "WMsgMouse.hpp"
#include "WMsgOptIO.hpp"

namespace reprize
{
namespace inp
{
class WMsgFactory : public InputFactory
{
public:
    WMsgFactory(void) {}
    virtual ~WMsgFactory(void) {}

	virtual InputBase* makeBase(reprize::Win32DepInfo* depinfo_) const { return new WMsgBase(depinfo_); }

	virtual Keyboard* makeKeyboard(reprize::Win32DepInfo* depinfo_) const { return new WMsgKeyboard(depinfo_); }
	virtual Mouse* makeMouse(reprize::Win32DepInfo* depinfo_) const { return new WMsgMouse(depinfo_); }
	virtual OptIO* makeOptIO(reprize::Win32DepInfo* depinfo_) const { return new WMsgOptIO(depinfo_); }
};
}
}
#endif
