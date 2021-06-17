/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */
#include "zen_app_config.h"

#if ZEN_DRAW_GLES && ZEN_OS_APPLE_IOS

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <CoreMotion/CoreMotion.h>

@interface GLViewController : GLKViewController

-(void)setFPS:(int)fps;
-(int)getFPS;

@end

extern GLViewController * G_view_controller;

#endif
