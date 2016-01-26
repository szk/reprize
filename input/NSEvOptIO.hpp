#ifndef NSEV_OPT_IO_HPP_
#define NSEV_OPT_IO_HPP_

#include "misc/NSDepInfo.hpp"
#include "OptIO.hpp"

namespace reprize
{
namespace inp
{
class NSEvOptIO : public OptIO
{
public:
    NSEvOptIO(NSDepInfo* dep_info_) : OptIO(dep_info_), ns_info(dep_info_)
        {}
    virtual ~NSEvOptIO(void) {}

    virtual const bool init(void) { return false; }
    virtual void restore(void) {}
    virtual void check(void) {}
    virtual void reset(void) {}

    virtual Int32 get_stick(void) { return 0; }
    virtual Int32 get_trigger(void) { return 0; }

protected:
    NSDepInfo* ns_info;
};
}
}
#endif
