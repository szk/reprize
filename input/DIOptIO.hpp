#ifndef DI_OPT_IO_HPP_
#define DI_OPT_IO_HPP_

#include "OptIO.hpp"

namespace reprize
{
class DIOptIO : public OptIO
{
public:
	DIOptIO(Win32DepInfo* dep_info);
	virtual ~DIOptIO(void);

	virtual void init(void);
	virtual void restore(void);

	virtual Int32 get_stick(void);
	virtual Int32 get_trigger(void);

private:
	Win32DepInfo* win32_info;
	LPDIRECTINPUTDEVICE8* dinp_stick;
};
}
#endif
