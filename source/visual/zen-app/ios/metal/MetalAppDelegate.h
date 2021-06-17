
#include "zen_app_config.h"

#if ZEN_OS_APPLE_IOS && ZEN_DRAW_METAL

#import <UIKit/UIKit.h>

@interface MetalAppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@end

extern MetalAppDelegate * G_app_delegate;

#endif
