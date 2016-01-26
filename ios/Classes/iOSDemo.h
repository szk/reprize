#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include "reprize.hpp"

namespace reprize
{
    extern bool g_finish;
    extern NSDevice* ns_device;
};

#pragma mark -
#pragma mark Initialization

void engine_init(int width, int height);
void engine_loop(void);

void set_pos(Float32 x_, Float32 y_);
void set_click(const bool clicked_);
void set_scale(const Float32 scale_);
