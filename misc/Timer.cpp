#include "Common.hpp"

using namespace reprize;
using namespace tmr;

Timer::Timer(void)
: node(NULL)
{
    node = new res::Node("Timer");
}

Timer::~Timer(void)
{
}

Timer* Timer::Instance(void)
{
    return NULL;
}

res::Node* Timer::get_node(void)
{
    return node;
}

const bool Timer::init(void)
{
    return false;
}

void Timer::reset(void)
{
}

const bool Timer::update(void)
{
    return false;
}

Int32 Timer::get_passed_usec(void)
{
    return 0;
}

uInt32 Timer::get_epoc_sec(void)
{
    return 0;
}

void Timer::sleep_sec(const uInt32 ms)
{
}

void Timer::sleep_msec(const uInt32 ms)
{
}

void Timer::sleep_usec(const uInt32 us)
{
}
