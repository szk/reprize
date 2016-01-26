//
//  NSGesture.h
//  reprize
//
//  Created by szk on 5/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface NSGesture : NSObject <UIGestureRecognizerDelegate>
{
    int touchCount;
    CGPoint startTouchPosition;
}

- (void)addGestureRecognizersToPiece:(UIView*)piece;

@end
