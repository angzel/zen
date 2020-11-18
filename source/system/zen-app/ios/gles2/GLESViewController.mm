/*
 Copyright (c) 2013 MeherTJ G.

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "zen_app_config.h"

#if defined(ZEN_OS_IOS)

#if ZEN_APP_DRAW_API_GLES

#import "GLESViewController.h"
#import "GLESAppDelegate.h"

#include "AppRuntimeIOS.h"

static const int DefaultFPS = 60;

#define runtime AppRuntimeIOS::S()

ViewController * G_view_controller = nil;

@interface ViewController ()
@property (strong, nonatomic) EAGLContext *context;
@end

@implementation ViewController

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
#endif
