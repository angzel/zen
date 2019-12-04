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

#import "ViewController.h"
#import "AppRuntime.h"
#import "AppDelegate.h"

static const int DefaultFPS = 60;

#define runtime AppRuntime::GetDefault()

ViewController * G_view_controller = nil;

@interface ViewController ()

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;
@end

@implementation ViewController

- (void)beginGL {

	self.preferredFramesPerSecond = DefaultFPS;

	self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

	GLKView *view = (GLKView *)self.view;
	view.context = self.context;
	view.drawableDepthFormat = GLKViewDrawableDepthFormatNone;

	[EAGLContext setCurrentContext:self.context];

	self.view.multipleTouchEnabled = NO;
}
- (void)endGL {

	if(self.effect)
	{
		self.effect = nil;
	}
	if(self.context)
	{
		if ([EAGLContext currentContext] == self.context) {
			[EAGLContext setCurrentContext:nil];
		}
		self.context = nil;
	}
}

- (void)viewDidLoad {
	[super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
	[self beginGL];

	glEnable(GL_BLEND);

	self.rotatable = NO;

	self.status_visible = NO;

	G_view_controller = self;
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];

	float w = self.view.bounds.size.width, h = self.view.bounds.size.height;

	runtime->launch({w, h});

	runtime->update();
}

- (void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning];
	// Dispose of any resources that can be recreated.
	[self endGL];
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

- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch * uiTouch in touches)
	{
		Zen::App::Touch touch;

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
		Zen::App::Touch touch;

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
		Zen::App::Touch touch;

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
		Zen::App::Touch touch;

		CGPoint l = [uiTouch locationInView:self.view];

		touch.x = l.x;
		touch.y = l.y;
		touch.no = (uint32_t)(long)uiTouch;

		runtime->touchCancel(touch);
	}
}

- (BOOL) shouldAutorotate
{
	return self.rotatable;
}

//- (void) didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
//{
//	float w = self.view.bounds.size.width, h = self.view.bounds.size.height;
//}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id <UIViewControllerTransitionCoordinator>)coordinator
{
	[coordinator animateAlongsideTransition:^(id<UIViewControllerTransitionCoordinatorContext> context)
	 {
//		 NSLog(@"转屏前调入");
	 } completion:^(id<UIViewControllerTransitionCoordinatorContext> context)
	 {
//		 NSLog(@"转屏后调入");
	 }];
	runtime->resize({(float)size.width, (float)size.height});

	[super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
}

- (BOOL) prefersStatusBarHidden
{
	return !self.status_visible;
}

@end
