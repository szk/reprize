#ifndef UNIX_TIMER_HPP_
#define UNIX_TIMER_HPP_

#include <unistd.h>
#include <time.h>
#include <sys/time.h>

// Singleton
namespace reprize
{
namespace tmr
{
class UNIXTimer : public Timer
{
protected:
    UNIXTimer(void) { reset(); }
public:
    virtual ~UNIXTimer(void) {}

    static UNIXTimer* Instance(void)
        {
            if (instance == 0) { instance = new UNIXTimer; }
            return instance;
        }
    virtual const bool init(void) { return true; }
    virtual void reset(void) { gettimeofday(&init_itv, NULL); }
    virtual const bool update(void)
        {
            gettimeofday(&now_itv, NULL);

            if (now_itv.tv_sec >= init_itv.tv_sec)
            {
                passed_usec = (now_itv.tv_sec - init_itv.tv_sec) * 1000000 +
                    (now_itv.tv_usec - init_itv.tv_usec);

                if (passed_usec > 0) { return true; }
                else return false;
            }
            else // wraparound
            {
                passed_usec = (now_itv.tv_sec - init_itv.tv_sec) * 1000000 +
                    (now_itv.tv_usec - init_itv.tv_usec);
                return false;
            }
        }
    virtual Int32 get_passed_usec(void) { return passed_usec; }
    virtual uInt32 get_epoc_sec(void) { time_t t; return ::time(&t); }
    virtual void sleep_sec(const uInt32 s_) { sleep(s_); }
    virtual void sleep_msec(const uInt32 ms_) { usleep(ms_ * 1000); }
    virtual void sleep_usec(const uInt32 us_) { usleep(us_); }

protected:
    static UNIXTimer* instance;
    struct timeval init_itv, now_itv;
    Int32 passed_usec;
};

UNIXTimer* UNIXTimer::instance = 0;
}
}
#endif
