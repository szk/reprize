#ifndef WIN32_TIMER_HPP_
#define WIN32_TIMER_HPP_

#include <Windef.h>
#include <mmsystem.h>

typedef unsigned int UINT; // XXX
typedef unsigned long DWORD; // XXX
static unsigned int period;

// we can use PerformanceCounter, but should examine multicore environment.
static void CALLBACK TimerFunc(UINT tid, UINT msg, DWORD arg,
                               DWORD dw1, DWORD dw2)
{
    ++period;
}

namespace reprize
{
namespace tmr
{
class Win32Timer : public Timer
{
protected:
    Win32Timer(void) : passed_usec(0) { reset(); }
public:
    virtual ~Win32Timer(void)
	{
	    if (timer_id)
            {
                timeKillEvent(timer_id);
                timeGetDevCaps(&caps, sizeof(caps));

                if (timeEndPeriod(caps.wPeriodMin) != TIMERR_NOERROR)
                {}
                else {}
	    }
            else
	    {
            }
	}

    static Win32Timer* Instance(void)
	{
		if (instance == 0) { instance = new Win32Timer; }
		return instance;
	}

    virtual const bool init(void)
	{
	    passed_usec = 0;

            timeGetDevCaps(&caps, sizeof(TIMECAPS));
            MMRESULT result = timeBeginPeriod(caps.wPeriodMin);

            if (result != TIMERR_NOERROR) { return false; }
            if (caps.wPeriodMin > 1) { return false; }

	    timer_id = timeSetEvent(1, caps.wPeriodMin,
                                    (LPTIMECALLBACK)TimerFunc, 0,
                                    (UINT)TIME_PERIODIC);
	    if (timer_id == NULL) { return false; }

            return true;
	}

    virtual void reset(void) { period = 0; }
    virtual const bool update(void)
        {
            passed_usec = period * 1000;

//                 global_log->printf("%d", passed_ms);
            return false;
        }

    virtual Int32 get_passed_usec(void) { return passed_usec; }
    virtual uInt32 get_epoc_sec(void) { return 0; }

    virtual void sleep_sec(const uInt32 s_) { Sleep((DWORD) s_ * 1000); }
    virtual void sleep_msec(const uInt32 ms_) { Sleep((DWORD) ms_); }
    virtual void sleep_usec(const uInt32 us_) { Sleep((DWORD) us_ / 1000); }

private:
    static Win32Timer* instance;
    Int32 passed_usec;

    TIMECAPS caps;
    MMRESULT timer_id;
};

Win32Timer* Win32Timer::instance = 0;
}
}
#endif
