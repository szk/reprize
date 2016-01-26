//
//  NSGesture.m
//  reprize
//
//  Created by szk on 5/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#include "iOSDemo.h"
#import "NSGesture.h"

@implementation NSGesture

// adds a set of gesture recognizers to one of our piece subviews
- (void)addGestureRecognizersToPiece:(UIView *)piece
{
    UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc]
                                             initWithTarget:self
                                                     action:@selector(tapPiece:)];
    tapGesture.numberOfTapsRequired = 2;
    tapGesture.numberOfTouchesRequired = 1;
    [piece addGestureRecognizer:tapGesture];
    [tapGesture release];

    UIRotationGestureRecognizer *rotationGesture = [[UIRotationGestureRecognizer alloc]
                                                       initWithTarget:self
                                                               action:@selector(rotatePiece:)];
    [piece addGestureRecognizer:rotationGesture];
    [rotationGesture release];

    UIPinchGestureRecognizer *pinchGesture = [[UIPinchGestureRecognizer alloc]
                                                 initWithTarget:self
                                                         action:@selector(scalePiece:)];
    [pinchGesture setDelegate:self];
    [piece addGestureRecognizer:pinchGesture];
    [pinchGesture release];

    UIPanGestureRecognizer *panGesture = [[UIPanGestureRecognizer alloc]
                                             initWithTarget:self
                                                     action:@selector(panPiece:)];
    [panGesture setMaximumNumberOfTouches:2];
    [panGesture setDelegate:self];
    [piece addGestureRecognizer:panGesture];
    [panGesture release];

    UILongPressGestureRecognizer *longPressGesture = [[UILongPressGestureRecognizer alloc]
                                                         initWithTarget:self
                                                                 action:@selector(pressPiece:)];
    [piece addGestureRecognizer:longPressGesture];
    [longPressGesture release];
}

-(void)dealloc
{
    [super dealloc];
}

/*
// scale and rotation transforms are applied relative to the layer's anchor point
// this method moves a gesture recognizer's view's anchor point between the user's fingers
- (void)adjustAnchorPointForGestureRecognizer:(UIGestureRecognizer *)gestureRecognizer
{
    if (gestureRecognizer.state == UIGestureRecognizerStateBegan)
    {
        UIView *piece = gestureRecognizer.view;
        CGPoint locationInView = [gestureRecognizer locationInView:piece];
        CGPoint locationInSuperview = [gestureRecognizer locationInView:piece.superview];

        piece.layer.anchorPoint = CGPointMake(locationInView.x / piece.bounds.size.width, locationInView.y / piece.bounds.size.height);
        piece.center = locationInSuperview;
    }
}
*/
// UIMenuController requires that we can become first responder or it won't display
- (BOOL)canBecomeFirstResponder
{
    return YES;
}

- (void)tapPiece:(UITapGestureRecognizer*)gestureRecognizer
{
    UIView *piece = [gestureRecognizer view];

//    [self adjustAnchorPointForGestureRecognizer:gestureRecognizer];

    if ([gestureRecognizer state] == UIGestureRecognizerStateBegan
        || [gestureRecognizer state] == UIGestureRecognizerStateChanged)
    {
        // CGPoint translation = [gestureRecognizer translationInView:[piece superview]];
        /*
        [piece setCenter:CGPointMake([piece center].x + translation.x, [piece center].y + translation.y)];
        */
        set_click(true);
        // [gestureRecognizer setTranslation:CGPointZero inView:[piece superview]];
    }
}

// shift the piece's center by the pan amount
// reset the gesture recognizer's translation to {0, 0} after applying so the next callback is a delta from the current position
- (void)panPiece:(UIPanGestureRecognizer*)gestureRecognizer
{
    UIView *piece = [gestureRecognizer view];

//    [self adjustAnchorPointForGestureRecognizer:gestureRecognizer];

    if ([gestureRecognizer state] == UIGestureRecognizerStateBegan
        || [gestureRecognizer state] == UIGestureRecognizerStateChanged)
    {
        CGPoint translation = [gestureRecognizer translationInView:[piece superview]];
        /*
        [piece setCenter:CGPointMake([piece center].x + translation.x, [piece center].y + translation.y)];
        */
        // set_pos([piece center].x + translation.x, [piece center].y + translation.y);
        set_pos(translation.x, translation.y);
        [gestureRecognizer setTranslation:CGPointZero inView:[piece superview]];
    }
}

// rotate the piece by the current rotation
// reset the gesture recognizer's rotation to 0 after applying so the next callback is a delta from the current rotation
- (void)rotatePiece:(UIRotationGestureRecognizer*)gestureRecognizer
{
//    [self adjustAnchorPointForGestureRecognizer:gestureRecognizer];

    if ([gestureRecognizer state] == UIGestureRecognizerStateBegan
        || [gestureRecognizer state] == UIGestureRecognizerStateChanged)
    {
        /*
        [gestureRecognizer view].transform = CGAffineTransformRotate([[gestureRecognizer view] transform],
                                                                     [gestureRecognizer rotation]);
                                                                     [gestureRecognizer setRotation:0];
        */
    }
}

// scale the piece by the current scale
// reset the gesture recognizer's rotation to 0 after applying so the next callback is a delta from the current scale
- (void)scalePiece:(UIPinchGestureRecognizer*)gestureRecognizer
{
//    [self adjustAnchorPointForGestureRecognizer:gestureRecognizer];

    if ([gestureRecognizer state] == UIGestureRecognizerStateBegan
        || [gestureRecognizer state] == UIGestureRecognizerStateChanged)
    {
        /*
        [gestureRecognizer view].transform = CGAffineTransformScale([[gestureRecognizer view] transform],
                                                                    [gestureRecognizer scale],
                                                                    [gestureRecognizer scale]);
                                                                    [gestureRecognizer setScale:1];
        */
        set_scale([gestureRecognizer scale]);
    }
}

- (void)pressPiece:(UITapGestureRecognizer*)gestureRecognizer
{
    UIView *piece = [gestureRecognizer view];

//    [self adjustAnchorPointForGestureRecognizer:gestureRecognizer];

    if ([gestureRecognizer state] == UIGestureRecognizerStateBegan
        || [gestureRecognizer state] == UIGestureRecognizerStateChanged)
    {
        // CGPoint translation = [gestureRecognizer translationInView:[piece superview]];
        /*
        [piece setCenter:CGPointMake([piece center].x + translation.x, [piece center].y + translation.y)];
        */
        set_click(true);
        // [gestureRecognizer setTranslation:CGPointZero inView:[piece superview]];
    }
}

// ensure that the pinch, pan and rotate gesture recognizers on a particular view can all recognize simultaneously
// prevent other gesture recognizers from recognizing simultaneously
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
    // if the gesture recognizers's view isn't one of our pieces, don't allow simultaneous recognition
//    if (gestureRecognizer.view != firstPieceView && gestureRecognizer.view != secondPieceView && gestureRecognizer.view != thirdPieceView)
//        return NO;

    // if the gesture recognizers are on different views, don't allow simultaneous recognition
    if (gestureRecognizer.view != otherGestureRecognizer.view)
    { return NO; }

    // if either of the gesture recognizers is the long press, don't allow simultaneous recognition
    if ([gestureRecognizer isKindOfClass:[UILongPressGestureRecognizer class]]
        || [otherGestureRecognizer isKindOfClass:[UILongPressGestureRecognizer class]])
    { return NO; }

    return YES;
}

@end
