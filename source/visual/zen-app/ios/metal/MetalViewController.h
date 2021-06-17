
#include "zen_app_config.h"

#if ZEN_OS_APPLE_IOS && ZEN_DRAW_METAL

#import <UIKit/UIKit.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

// Our iOS view controller
@interface MetalViewController : UIViewController<MTKViewDelegate>

-(void)setFPS:(int)fps;
-(int)getFPS;

@end

extern MetalViewController * G_view_controller;

#endif
