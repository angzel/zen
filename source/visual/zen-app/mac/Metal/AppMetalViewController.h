//
//  GameViewController.h
//  macMetalGame
//
//  Created by MeherTj on 2020/12/19.
//

#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

// Our macOS view controller.
@interface AppMetalViewController : NSViewController

@property MTKView * mtkView;

-(void)setFPS:(int)fps;
-(int)getFPS;

@end

extern AppMetalViewController * gViewController;
