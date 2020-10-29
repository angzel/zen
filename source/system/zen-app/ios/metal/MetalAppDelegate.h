
#include "zen_app_config.h"

#if ZEN_APP_DRAW_API_METAL

#import <UIKit/UIKit.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@end

extern AppDelegate * G_app_delegate;

#endif
