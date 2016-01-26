//
//  reprizeAppDelegate.m
//  reprize
//
//  Created by szk on 3/21/11.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#import "reprizeAppDelegate.h"
#import "EAGLView.h"

@implementation reprizeAppDelegate

@synthesize window;
@synthesize glView;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [glView startAnimation];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [glView stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [glView startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [glView stopAnimation];
}

- (void)dealloc
{
    [window release];
    [glView release];

    [super dealloc];
}

@end
