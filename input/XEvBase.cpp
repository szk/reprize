#include "Common.hpp"

#include "misc/X11DepInfo.hpp"

#include "XEvKeyboard.hpp"
#include "XEvMouse.hpp"
#include "XEvOptIO.hpp"

#include "XEvBase.hpp"

using namespace reprize;
using namespace inp;

XEvBase::XEvBase(X11DepInfo* dep_info_)
    : InputBase(dep_info_), x11_info(dep_info_)
{
}

XEvBase::~XEvBase(void)
{
}

const bool XEvBase::init(void)
{
    InputBase::init();
    return true;
}

void XEvBase::restore(void)
{
}

void XEvBase::check(Keyboard* keyboard_, Mouse* mouse_, OptIO* opt_io_)
{
    /*
      XPending uses signals, if you use in signals in
      other place then it was broken.
      if you have problem, please use following code as substitute.

      int arg = 0;
      while (XEventsQueued (display, QueuedAlready) ||
      ioctl (ConnectionNumber (display), FIONREAD, &arg) == 0 && arg)
      {
          blah blah
      }
    */

    XFlush(x11_info->get_dpy());
//    XSync(x11_info->get_dpy(), false);

    while (XPending(x11_info->get_dpy()))
    {
	XNextEvent(x11_info->get_dpy(), x11_info->get_event());
	switch (x11_info->get_event()->type)
	{
	case KeyPress:
        case KeyRelease:
	    keyboard_->check(pressed, released);
	    break;
	case MotionNotify:
	case ButtonPress:
        case ButtonRelease:
	    mouse_->check(pressed, released);
	    break;
	case ConfigureNotify:
	    break;
	case Expose:
	    break;
	case MappingNotify:
	    g_focus = true;
	    break;
	case ClientMessage:
	    if (static_cast<Atom>(x11_info->get_event()->xclient.data.l[0])
                == *(x11_info->get_del_atom()))
	    { g_finish = true; }
	    break;
	}
    }
}
