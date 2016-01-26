//
//  ESRenderer.h
//  reprize
//
//  Created by szk on 3/21/11.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>

@protocol ESRenderer <NSObject>

- (void)render;
- (void)update;
- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer;

@end
