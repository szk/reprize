#ifndef OPT_IO_HPP_
#define OPT_IO_HPP_

namespace reprize
{
namespace inp
{
class OptIO
{
public:
    OptIO(EnvDepInfo* dep_info) {}
    virtual ~OptIO(void) {}

    virtual const bool init(void) { return false; }
    virtual void restore(void) {}
    virtual void check(void) {}
    virtual void reset(void) {}
    
    virtual Int32 get_stick(void) { return 0; }
    virtual Int32 get_trigger(void) { return 0; }

protected:
};
}
}
#endif
