/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#include "zen_app_config.h"


#if ZEN_OS_APPLE_IOS && ZEN_DRAW_GLES

#import <UIKit/UIKit.h>

@interface GLAppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@end

extern GLAppDelegate * G_app_delegate;

#endif
