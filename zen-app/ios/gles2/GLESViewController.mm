/*
 Copyright (c) 2013 ClearSky G.

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

#if ZEN_APP_DRAW_API_OPENGLES

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

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];

	UIScreen * main = [UIScreen mainScreen];
	float s = main.nativeBounds.size.width / main.bounds.size.width;
	CGSize size = self.view.bounds.size;

	runtime->launch({(float)size.width * s, (float)size.height * s});

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
	UIScreen * main = [UIScreen mainScreen];
	float s = main.nativeBounds.size.width / main.bounds.size.width;
	
	runtime->resize({(float)size.width*s, (float)size.height*s});

	[super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
}

- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch * uiTouch in touches)
	{
		Zen::AppTouch touch;

		CGPoint l = [uiTouch locationInView:self.view];

		touch.x = l.x;
		touch.y = l.y;
		touch.no = (uint32_t)(long)uiTouch;

		runtime->touchDown(touch);
	}
}
- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch * uiTouch in touches)
	{
		Zen::AppTouch touch;

		CGPoint l = [uiTouch locationInView:self.view];

		touch.x = l.x;
		touch.y = l.y;
		touch.no = (uint32_t)(long)uiTouch;

		runtime->touchMove(touch);
	}
}
- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch * uiTouch in touches)
	{
		Zen::AppTouch touch;

		CGPoint l = [uiTouch locationInView:self.view];

		touch.x = l.x;
		touch.y = l.y;
		touch.no = (uint32_t)(long)uiTouch;

		runtime->touchUp(touch);
	}
}
- (void) touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch * uiTouch in touches)
	{
		Zen::AppTouch touch;

		CGPoint l = [uiTouch locationInView:self.view];

		touch.x = l.x;
		touch.y = l.y;
		touch.no = (uint32_t)(long)uiTouch;

		runtime->touchCancel(touch);
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
