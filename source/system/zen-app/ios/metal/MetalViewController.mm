
#include "zen_app_config.h"

#if ZEN_APP_DRAW_API_METAL

#include "zen_metal_id.h"

#import "AppRuntimeIOS.h"

static const int DefaultFPS = 60;

#define runtime AppRuntimeIOS::S()

ViewController * G_view_controller = nil;

@implementation ViewController
{
    MTKView *_view;
	bool _launched;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	G_view_controller = self;
	
    self.view = _view = [[MTKView alloc] init];;

    _view.backgroundColor = UIColor.blueColor;
	_view.preferredFramesPerSecond = DefaultFPS;

	auto device_id = Zen::Metal::Device::S()->getID();
	_view.device = device_id->device;
	
    if(!_view.device)
    {
        NSLog(@"Metal is not supported on this device");
        self.view = [[UIView alloc] initWithFrame:self.view.frame];
        return;
    }
	device_id->view = _view;
	
    device_id->depth_pixel_format = _view.depthStencilPixelFormat;
	device_id->color_pixel_format = _view.colorPixelFormat;
	_view.clearColor = MTLClearColorMake(0, 0, 0, 1);
    _view.sampleCount = 1;

	_launched = false;
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];

	_view.delegate = self;

//	float w = self.view.bounds.size.width, h = self.view.bounds.size.height;
//	float s = [UIScreen mainScreen].scale;

//	runtime->launch({w*s, h*s});
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
- (void)drawInMTKView:(nonnull MTKView *)view
{
	[self update];
	// draw
	runtime->draw();
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
	if(_launched)
	{
		runtime->resize({(float)size.width, (float)size.height});
	}
	else
	{
		runtime->launch({(float)size.width, (float)size.height});
		_launched = true;
	}
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id <UIViewControllerTransitionCoordinator>)coordinator
{
//	float s = [UIScreen mainScreen].scale;
//	runtime->resize({(float)size.width*s, (float)size.height*s});

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
	return true;
	return runtime->isRotatable();
}

- (BOOL) prefersStatusBarHidden
{
	return !runtime->isStatusVisible();
}


-(void)setFPS:(int)fps
{
	_view.preferredFramesPerSecond = fps;
}
-(int)getFPS
{
	return (int)_view.preferredFramesPerSecond;
}

@end

#endif
