//
//  Renderer.h
//  macMetalGame
//
//  Created by MeherTj on 2020/12/19.
//

#import <MetalKit/MetalKit.h>

// Our platform independent renderer class.   Implements the MTKViewDelegate protocol which
//   allows it to accept per-frame update and drawable resize callbacks.
@interface AppMetalRenderer : NSObject <MTKViewDelegate>

-(void)onLaunch:(MTKView*)view;

@end

extern AppMetalRenderer * gRenderer;

