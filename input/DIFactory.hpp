#ifndef DI_FACTORY_HPP_
#define DI_FACTORY_HPP_

#include "DIOptIO.hpp"

namespace reprize
{
class DIFactory : public InputFactory
{
public:
	DIFactory(Win32DepInfo* dep_info);
	virtual ~DIFactory(void);

	virtual InputBase* makeBase(void) const;

	virtual Keyboard* makeKeyboard(void) const;
	virtual Mouse* makeMouse(void) const;
	virtual OptIO* makeOptIO(void) const;

protected:
	Win32DepInfo* win32_info;
};
}
#endif
