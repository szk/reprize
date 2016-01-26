#ifndef GLX11_SCREEN_HPP_
#define GLX11_SCREEN_HPP_

#include "Screen.hpp"

// if you don't use OpenGL extensions, please comment out here.
// #define GL_GLEXT_PROTOTYPES

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

namespace reprize
{
class NSDepInfo;

namespace vid
{
class GLNSScreen : public Screen
{
public:
    GLNSScreen(NSDepInfo* dep_info);
    virtual ~GLNSScreen(void);

    virtual void init(void);
    virtual void appear(void);

    virtual void begin_paint(void);
    virtual void finish_paint(void);
    virtual const bool flip(void);

    virtual void release(void);

protected:
    const bool alloc_extension(void);
    void kill_cursor(void);

    NSDepInfo* ns_info;
/*
    XVisualInfo* vi;
    XSizeHints* sz_hints;
    XWMHints* wm_hints;
*/
    bool doublebuffer;
};
}
}
#endif
