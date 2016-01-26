#ifndef XEV_BASE_HPP_
#define XEV_BASE_HPP_

#include "InputBase.hpp"

namespace reprize
{
class X11DepInfo;

namespace inp
{
class XEvBase : public InputBase
{
public:
    XEvBase(X11DepInfo* dep_info_);
    virtual ~XEvBase(void);

    const bool init(void);
    void restore(void);
    void check(Keyboard* keyboard_, Mouse* mouse_, OptIO* opt_io_);

protected:
    X11DepInfo* x11_info;
};
}
}
#endif
