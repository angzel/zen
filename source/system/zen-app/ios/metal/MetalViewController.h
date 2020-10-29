
#include "zen_app_config.h"

#if ZEN_APP_DRAW_API_METAL

#import <UIKit/UIKit.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

// Our iOS view controller
@interface ViewController : UIViewController<MTKViewDelegate>

-(void)setFPS:(int)fps;
-(int)getFPS;

@end

extern ViewController * G_view_controller;

#endif
