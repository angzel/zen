/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */
#include "zen_app_config.h"

#if ZEN_OS_APPLE_IOS && ZEN_DRAW_GLES

#import "GLESViewController.h"
#import "GLESAppDelegate.h"

#include "zen_app_ios.h"

static const int DefaultFPS = 60;

#define runtime AppIOS::Get()

GLViewController * G_view_controller = nil;

@interface GLViewController ()
@property (strong, nonatomic) EAGLContext *context;
@end

@implementation GLViewController

- (void)viewDidLoad {
	[super viewDidLoad];
	G_view_controller = self;

	self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	[EAGLContext setCurrentContext:self.context];

	GLKView *view = (GLKView *)self.view;

	view.context = self.context;
	view.drawableDepthFormat = GLKViewDrawableDepthFormatNone;

	self.view.multipleTouchEnabled = NO;
	[self setFPS:DefaultFPS];
}

- (CGSize) toPhysicalSize:(CGSize)size
{
	CGFloat scale = UIScreen.mainScreen.nativeScale;
	return { size.width * scale, size.height * scale };
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];

	CGSize rsize = [self toPhysicalSize:self.view.bounds.size];

	runtime->launch({(float)rsize.width, (float)rsize.height});

	runtime->update();
}

- (void)dealloc
{
	if(G_view_controller == self)
	{
		G_view_controller = nil;
	}
}

- (void)update
{
	runtime->update();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
	runtime->draw();
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id <UIViewControllerTransitionCoordinator>)coordinator
{
	CGSize rsize = [self toPhysicalSize:size];
	runtime->resize({(float)rsize.width, (float)rsize.height});

	[super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
}
- (Zen::AppTouch) toAppTouch:(UITouch*)uiTouch
{
	Zen::AppTouch touch;

	CGPoint pos = [uiTouch locationInView:self.view];
	CGSize size = self.view.bounds.size;

//	printf(">> %.2f %.2f\n", pos.x, pos.y);

	touch.x = pos.x / size.width;
	touch.y = 1 - pos.y / size.height;
	touch.no = (uint32_t)(long)uiTouch;
	return touch;
}
- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch * uiTouch in touches)
	{
		runtime->touchDown([self toAppTouch:uiTouch]);
	}
}
- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch * uiTouch in touches)
	{
		runtime->touchMove([self toAppTouch:uiTouch]);
	}
}
- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch * uiTouch in touches)
	{
		runtime->touchUp([self toAppTouch:uiTouch]);
	}
}
- (void) touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch * uiTouch in touches)
	{
		runtime->touchCancel([self toAppTouch:uiTouch]);
	}
}

- (BOOL) shouldAutorotate
{
	return runtime->isRotatable();
}

- (BOOL) prefersStatusBarHidden
{
	return !runtime->isStatusVisible();
}


-(void)setFPS:(int)fps
{
	self.preferredFramesPerSecond = fps;
}
-(int)getFPS
{
	return (int)self.preferredFramesPerSecond;
}

@end

#endif
