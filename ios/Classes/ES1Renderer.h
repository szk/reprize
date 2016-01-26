//
//  ES1Renderer.h
//  reprize
//
//  Created by szk on 3/21/11.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#import "ESRenderer.h"

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

@interface ES1Renderer : NSObject <ESRenderer>
{
@private
    EAGLContext *context;

    // The pixel dimensions of the CAEAGLLayer
    GLint backingWidth;
    GLint backingHeight;

    // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view
    GLuint defaultFramebuffer, colorRenderbuffer;
    float transY;
}

- (void)render;
- (void)render_orig;
- (void)update;
- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer;

@end
