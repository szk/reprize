#ifndef DI_BASE_HPP_
#define DI_BASE_HPP_

#include "InputBase.hpp"

namespace reprize
{
class DIBase : public InputBase
{
public:
    DIBase(Win32DepInfo* dep_info);
    virtual ~DIBase(void);

    virtual void init(void);
    virtual void restore(void);
    virtual void check(void);

protected:
	Win32DepInfo* win32_info;
};
}
#endif
