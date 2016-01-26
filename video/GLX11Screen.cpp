#include "Common.hpp"

#include "misc/X11DepInfo.hpp"

#include "GLX11Screen.hpp"

using namespace reprize;
using namespace vid;

Int32 sb_config[] = { GLX_DOUBLEBUFFER, GLX_RGBA, GLX_DEPTH_SIZE, 16,
		      GLX_RED_SIZE, 0, GLX_BLUE_SIZE, 0, GLX_GREEN_SIZE, 0,
                      None };
Int32 db_config[] = { GLX_DOUBLEBUFFER, GLX_RGBA, GLX_DEPTH_SIZE, 16,
		      GLX_RED_SIZE, 0, GLX_BLUE_SIZE, 0, GLX_GREEN_SIZE, 0,
                      GLX_DOUBLEBUFFER, None };

PFNGLATTACHSHADERPROC glAttachShader;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLDRAWBUFFERSPROC glDrawBuffers;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERSOURCEPROC glGetShaderSource;
PFNGLGETUNIFORMFVPROC glGetUniformfv;
PFNGLGETUNIFORMIVPROC glGetUniformiv;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv;
PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv;
PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv;
PFNGLISPROGRAMPROC glIsProgram;
PFNGLISSHADERPROC glIsShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate;
PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate;
PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM1FVPROC glUniform1fv;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLUNIFORM1IVPROC glUniform1iv;
PFNGLUNIFORM2FPROC glUniform2f;
PFNGLUNIFORM2FVPROC glUniform2fv;
PFNGLUNIFORM2IPROC glUniform2i;
PFNGLUNIFORM2IVPROC glUniform2iv;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLUNIFORM3IPROC glUniform3i;
PFNGLUNIFORM3IVPROC glUniform3iv;
PFNGLUNIFORM4FPROC glUniform4f;
PFNGLUNIFORM4FVPROC glUniform4fv;
PFNGLUNIFORM4IPROC glUniform4i;
PFNGLUNIFORM4IVPROC glUniform4iv;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLVALIDATEPROGRAMPROC glValidateProgram;
PFNGLVERTEXATTRIB1DPROC glVertexAttrib1d;
PFNGLVERTEXATTRIB1DVPROC glVertexAttrib1dv;
PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f;
PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv;
PFNGLVERTEXATTRIB1SPROC glVertexAttrib1s;
PFNGLVERTEXATTRIB1SVPROC glVertexAttrib1sv;
PFNGLVERTEXATTRIB2DPROC glVertexAttrib2d;
PFNGLVERTEXATTRIB2DVPROC glVertexAttrib2dv;
PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f;
PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv;
PFNGLVERTEXATTRIB2SPROC glVertexAttrib2s;
PFNGLVERTEXATTRIB2SVPROC glVertexAttrib2sv;
PFNGLVERTEXATTRIB3DPROC glVertexAttrib3d;
PFNGLVERTEXATTRIB3DVPROC glVertexAttrib3dv;
PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f;
PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv;
PFNGLVERTEXATTRIB3SPROC glVertexAttrib3s;
PFNGLVERTEXATTRIB3SVPROC glVertexAttrib3sv;
PFNGLVERTEXATTRIB4BVPROC glVertexAttrib4bv;
PFNGLVERTEXATTRIB4DPROC glVertexAttrib4d;
PFNGLVERTEXATTRIB4DVPROC glVertexAttrib4dv;
PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f;
PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
PFNGLVERTEXATTRIB4IVPROC glVertexAttrib4iv;
PFNGLVERTEXATTRIB4NBVPROC glVertexAttrib4Nbv;
PFNGLVERTEXATTRIB4NIVPROC glVertexAttrib4Niv;
PFNGLVERTEXATTRIB4NSVPROC glVertexAttrib4Nsv;
PFNGLVERTEXATTRIB4NUBPROC glVertexAttrib4Nub;
PFNGLVERTEXATTRIB4NUBVPROC glVertexAttrib4Nubv;
PFNGLVERTEXATTRIB4NUIVPROC glVertexAttrib4Nuiv;
PFNGLVERTEXATTRIB4NUSVPROC glVertexAttrib4Nusv;
PFNGLVERTEXATTRIB4SPROC glVertexAttrib4s;
PFNGLVERTEXATTRIB4SVPROC glVertexAttrib4sv;
PFNGLVERTEXATTRIB4UBVPROC glVertexAttrib4ubv;
PFNGLVERTEXATTRIB4UIVPROC glVertexAttrib4uiv;
PFNGLVERTEXATTRIB4USVPROC glVertexAttrib4usv;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

GLX11Screen::GLX11Screen(X11DepInfo* depinfo_)
    : Screen(depinfo_), x11_info(depinfo_), vi(NULL), sz_hints(NULL),
      wm_hints(NULL), doublebuffer(false)
{
    if (!(sz_hints = XAllocSizeHints()))
    { g_log->printf("Could not alloc XSizeHints"); }
    if (!(wm_hints = XAllocWMHints()))
    { g_log->printf("Could not alloc XWMHints"); }
}

GLX11Screen::~GLX11Screen(void)
{
    XFree(vi);
    XFree(sz_hints);
    XFree(wm_hints);
}

void GLX11Screen::init(void)
{
    Int32 dummy;
    if (!glXQueryExtension(x11_info->get_dpy(), &dummy, &dummy))
    {
	g_log->printf("Can't get query extension");
	g_log->printf("X server doesn't have glx extension");
	return;
    }

    if ((vi = glXChooseVisual(x11_info->get_dpy(), x11_info->get_scr(),
                              db_config)))
    { doublebuffer = true; }
    else if ((vi = glXChooseVisual(x11_info->get_dpy(), x11_info->get_scr(),
                                   sb_config)))
    { doublebuffer = false; }
    else
    { g_log->printf("No RGB visual with depth buffer"); return; }

    GLXContext cx;
//     if (!(cx = glXCreateContext(x11_info->get_dpy(), vi, None, GL_FALSE)))
//     { g_log->printf("Could not create rendering context"); }

    if (!(cx = glXCreateContext(x11_info->get_dpy(), vi, None, GL_TRUE)))
    { g_log->printf("Could not create rendering context"); }

    Colormap cmap;
    cmap = XCreateColormap(x11_info->get_dpy(),
                           RootWindow(x11_info->get_dpy(), vi->screen),
                           vi->visual, AllocNone);

    XSetWindowAttributes win_att;
    win_att.colormap = cmap;
    win_att.border_pixel = 0;
    win_att.event_mask = ExposureMask | ButtonPressMask | StructureNotifyMask;

    sz_hints->flags = USSize | USPosition | PMaxSize | PMinSize | PBaseSize;
    sz_hints->base_width = SCREEN_WIDTH;
    sz_hints->base_height = SCREEN_HEIGHT;
    sz_hints->min_width = SCREEN_WIDTH;
    sz_hints->min_height = SCREEN_HEIGHT;
    sz_hints->max_width = SCREEN_WIDTH;
    sz_hints->max_height = SCREEN_HEIGHT;

    x11_info->set_win(XCreateWindow(x11_info->get_dpy(),
                                    RootWindow(x11_info->get_dpy(), vi->screen),
                                    0, 0,
                                    sz_hints->base_width, sz_hints->base_height,
                                    0, vi->depth, InputOutput, vi->visual,
                                    CWBorderPixel | CWColormap | CWEventMask,
                                    &win_att));

    XSetStandardProperties(x11_info->get_dpy(), x11_info->get_win(),
                           TITLE, TITLE, None, x11_info->get_argv(),
                           x11_info->get_argc(), sz_hints);

    wm_hints->initial_state = NormalState;
    wm_hints->flags = StateHint;
    XSetWMHints(x11_info->get_dpy(), x11_info->get_win(), wm_hints);

    XSelectInput(x11_info->get_dpy(), x11_info->get_win(),
		 (ExposureMask | ButtonPressMask | ButtonReleaseMask
                  | PropertyChangeMask | PointerMotionMask
                  | KeyPressMask | KeyReleaseMask | FocusChangeMask
                  | VisibilityChangeMask | StructureNotifyMask));

    x11_info->set_del_atom(XInternAtom(x11_info->get_dpy(),
                                       "WM_DELETE_WINDOW", False));
    XSetWMProtocols(x11_info->get_dpy(), x11_info->get_win(),
                    x11_info->get_del_atom(), 1);
    glXMakeCurrent(x11_info->get_dpy(), x11_info->get_win(), cx);

    kill_cursor();

    alloc_extension();

    Screen::init();
}

void GLX11Screen::appear(void)
{
    //show the window
    XMapWindow(x11_info->get_dpy(), x11_info->get_win());
}

void GLX11Screen::begin_paint(void)
{
}

void GLX11Screen::finish_paint(void)
{
}

const bool GLX11Screen::flip(void)
{
    if (doublebuffer)
    { glXSwapBuffers(x11_info->get_dpy(), x11_info->get_win()); }
    else { glFlush(); }

    return true;
}

void GLX11Screen::release(void)
{
}

#define PADDR(functype, funcname) \
    ((funcname = (functype) glXGetProcAddress( #funcname )) == 0)

const bool GLX11Screen::alloc_extension(void)
{
    // XXX
    Str ext_str(reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)));
    // we need pbo or fbo or vbo, and shaders
//     std::cerr << ext_str << std::endl;

    int err = 0;
// glXGetProcAddressARB
#ifdef UNDEF
    err |= PADDR(PFNGLATTACHSHADERPROC, glAttachShader);
    err |= PADDR(PFNGLBINDATTRIBLOCATIONPROC, glBindAttribLocation);
    err |= PADDR(PFNGLBLENDEQUATIONSEPARATEPROC, glBlendEquationSeparate);
    err |= PADDR(PFNGLCOMPILESHADERPROC, glCompileShader);
    err |= PADDR(PFNGLCREATEPROGRAMPROC, glCreateProgram);
    err |= PADDR(PFNGLCREATESHADERPROC, glCreateShader);
    err |= PADDR(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
    err |= PADDR(PFNGLDELETESHADERPROC, glDeleteShader);
    err |= PADDR(PFNGLDETACHSHADERPROC, glDetachShader);
    err |= PADDR(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
    err |= PADDR(PFNGLDRAWBUFFERSPROC, glDrawBuffers);
    err |= PADDR(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
    err |= PADDR(PFNGLGETACTIVEATTRIBPROC, glGetActiveAttrib);
    err |= PADDR(PFNGLGETACTIVEUNIFORMPROC, glGetActiveUniform);
    err |= PADDR(PFNGLGETATTACHEDSHADERSPROC, glGetAttachedShaders);
    err |= PADDR(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation);
    err |= PADDR(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
    err |= PADDR(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
    err |= PADDR(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
    err |= PADDR(PFNGLGETSHADERIVPROC, glGetShaderiv);
    err |= PADDR(PFNGLGETSHADERSOURCEPROC, glGetShaderSource);
    err |= PADDR(PFNGLGETUNIFORMFVPROC, glGetUniformfv);
    err |= PADDR(PFNGLGETUNIFORMIVPROC, glGetUniformiv);
    err |= PADDR(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
    err |= PADDR(PFNGLGETVERTEXATTRIBDVPROC, glGetVertexAttribdv);
    err |= PADDR(PFNGLGETVERTEXATTRIBFVPROC, glGetVertexAttribfv);
    err |= PADDR(PFNGLGETVERTEXATTRIBIVPROC, glGetVertexAttribiv);
    err |= PADDR(PFNGLGETVERTEXATTRIBPOINTERVPROC, glGetVertexAttribPointerv);
    err |= PADDR(PFNGLISPROGRAMPROC, glIsProgram);
    err |= PADDR(PFNGLISSHADERPROC, glIsShader);
    err |= PADDR(PFNGLLINKPROGRAMPROC, glLinkProgram);
    err |= PADDR(PFNGLSHADERSOURCEPROC, glShaderSource);
    err |= PADDR(PFNGLSTENCILFUNCSEPARATEPROC, glStencilFuncSeparate);
    err |= PADDR(PFNGLSTENCILMASKSEPARATEPROC, glStencilMaskSeparate);
    err |= PADDR(PFNGLSTENCILOPSEPARATEPROC, glStencilOpSeparate);
    err |= PADDR(PFNGLUNIFORM1FPROC, glUniform1f);
    err |= PADDR(PFNGLUNIFORM1FVPROC, glUniform1fv);
    err |= PADDR(PFNGLUNIFORM1IPROC, glUniform1i);
    err |= PADDR(PFNGLUNIFORM1IVPROC, glUniform1iv);
    err |= PADDR(PFNGLUNIFORM2FPROC, glUniform2f);
    err |= PADDR(PFNGLUNIFORM2FVPROC, glUniform2fv);
    err |= PADDR(PFNGLUNIFORM2IPROC, glUniform2i);
    err |= PADDR(PFNGLUNIFORM2IVPROC, glUniform2iv);
    err |= PADDR(PFNGLUNIFORM3FPROC, glUniform3f);
    err |= PADDR(PFNGLUNIFORM3FVPROC, glUniform3fv);
    err |= PADDR(PFNGLUNIFORM3IPROC, glUniform3i);
    err |= PADDR(PFNGLUNIFORM3IVPROC, glUniform3iv);
    err |= PADDR(PFNGLUNIFORM4FPROC, glUniform4f);
    err |= PADDR(PFNGLUNIFORM4FVPROC, glUniform4fv);
    err |= PADDR(PFNGLUNIFORM4IPROC, glUniform4i);
    err |= PADDR(PFNGLUNIFORM4IVPROC, glUniform4iv);
    err |= PADDR(PFNGLUNIFORMMATRIX2FVPROC, glUniformMatrix2fv);
    err |= PADDR(PFNGLUNIFORMMATRIX3FVPROC, glUniformMatrix3fv);
    err |= PADDR(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
    err |= PADDR(PFNGLUSEPROGRAMPROC, glUseProgram);
    err |= PADDR(PFNGLVALIDATEPROGRAMPROC, glValidateProgram);
    err |= PADDR(PFNGLVERTEXATTRIB1DPROC, glVertexAttrib1d);
    err |= PADDR(PFNGLVERTEXATTRIB1DVPROC, glVertexAttrib1dv);
    err |= PADDR(PFNGLVERTEXATTRIB1FPROC, glVertexAttrib1f);
    err |= PADDR(PFNGLVERTEXATTRIB1FVPROC, glVertexAttrib1fv);
    err |= PADDR(PFNGLVERTEXATTRIB1SPROC, glVertexAttrib1s);
    err |= PADDR(PFNGLVERTEXATTRIB1SVPROC, glVertexAttrib1sv);
    err |= PADDR(PFNGLVERTEXATTRIB2DPROC, glVertexAttrib2d);
    err |= PADDR(PFNGLVERTEXATTRIB2DVPROC, glVertexAttrib2dv);
    err |= PADDR(PFNGLVERTEXATTRIB2FPROC, glVertexAttrib2f);
    err |= PADDR(PFNGLVERTEXATTRIB2FVPROC, glVertexAttrib2fv);
    err |= PADDR(PFNGLVERTEXATTRIB2SPROC, glVertexAttrib2s);
    err |= PADDR(PFNGLVERTEXATTRIB2SVPROC, glVertexAttrib2sv);
    err |= PADDR(PFNGLVERTEXATTRIB3DPROC, glVertexAttrib3d);
    err |= PADDR(PFNGLVERTEXATTRIB3DVPROC, glVertexAttrib3dv);
    err |= PADDR(PFNGLVERTEXATTRIB3FPROC, glVertexAttrib3f);
    err |= PADDR(PFNGLVERTEXATTRIB3FVPROC, glVertexAttrib3fv);
    err |= PADDR(PFNGLVERTEXATTRIB3SPROC, glVertexAttrib3s);
    err |= PADDR(PFNGLVERTEXATTRIB3SVPROC, glVertexAttrib3sv);
    err |= PADDR(PFNGLVERTEXATTRIB4BVPROC, glVertexAttrib4bv);
    err |= PADDR(PFNGLVERTEXATTRIB4DPROC, glVertexAttrib4d);
    err |= PADDR(PFNGLVERTEXATTRIB4DVPROC, glVertexAttrib4dv);
    err |= PADDR(PFNGLVERTEXATTRIB4FPROC, glVertexAttrib4f);
    err |= PADDR(PFNGLVERTEXATTRIB4FVPROC, glVertexAttrib4fv);
    err |= PADDR(PFNGLVERTEXATTRIB4IVPROC, glVertexAttrib4iv);
    err |= PADDR(PFNGLVERTEXATTRIB4NBVPROC, glVertexAttrib4Nbv);
    err |= PADDR(PFNGLVERTEXATTRIB4NIVPROC, glVertexAttrib4Niv);
    err |= PADDR(PFNGLVERTEXATTRIB4NSVPROC, glVertexAttrib4Nsv);
    err |= PADDR(PFNGLVERTEXATTRIB4NUBPROC, glVertexAttrib4Nub);
    err |= PADDR(PFNGLVERTEXATTRIB4NUBVPROC, glVertexAttrib4Nubv);
    err |= PADDR(PFNGLVERTEXATTRIB4NUIVPROC, glVertexAttrib4Nuiv);
    err |= PADDR(PFNGLVERTEXATTRIB4NUSVPROC, glVertexAttrib4Nusv);
    err |= PADDR(PFNGLVERTEXATTRIB4SPROC, glVertexAttrib4s);
    err |= PADDR(PFNGLVERTEXATTRIB4SVPROC, glVertexAttrib4sv);
    err |= PADDR(PFNGLVERTEXATTRIB4UBVPROC, glVertexAttrib4ubv);
    err |= PADDR(PFNGLVERTEXATTRIB4UIVPROC, glVertexAttrib4uiv);
    err |= PADDR(PFNGLVERTEXATTRIB4USVPROC, glVertexAttrib4usv);
    err |= PADDR(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);

#endif
    return false;
}

void GLX11Screen::kill_cursor(void)
{
    static Cursor empty_cursor = static_cast<Cursor>(NULL);
    if (empty_cursor == static_cast<Cursor>(NULL))
    {
        static char bits[] = { 0, 0, 0, 0, 0, 0, 0, 0, };
        Pixmap pm = XCreateBitmapFromData(x11_info->get_dpy(),
                                          x11_info->get_win(), bits, 8, 8);
        XColor color;
        color.pixel = color.red = color.green = color.blue = 0;
        color.flags = DoGreen | DoRed | DoBlue;
        empty_cursor = XCreatePixmapCursor(x11_info->get_dpy(), pm, pm,
                                           &color, &color, 0, 0);
    }
    XDefineCursor(x11_info->get_dpy(), x11_info->get_win(), empty_cursor);
}
