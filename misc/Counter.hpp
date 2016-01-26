#ifndef COUNTER_HPP_
#define COUNTER_HPP_

// Singleton
namespace reprize
{
namespace res
{
class Counter
{
public:
    Counter(void) : counter(0) { reset(); }
    virtual ~Counter(void) {}

    const bool init(void) { return true; }
    void reset(void) { counter = 0; }
    inline uInt32 get_num(void) { return counter++; }

protected:
    uInt32 counter;
};

}
}
#endif
