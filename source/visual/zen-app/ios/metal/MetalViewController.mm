
#include "zen_app_config.h"

#if ZEN_OS_APPLE_IOS && ZEN_DRAW_METAL

#include "zen_metal_id.h"

#import "zen_app_ios.h"

static const int DefaultFPS = 60;

#define runtime AppIOS::Get()

MetalViewController * G_view_controller = nil;

@implementation MetalViewController
{
    MTKView *_view;
	bool _launched;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	
	G_view_controller = self;
	
    _view = [[MTKView alloc] init];;

    _view.backgroundColor = UIColor.blueColor;
	_view.preferredFramesPerSecond = DefaultFPS;
	
	_view.clearColor = MTLClearColorMake(0, 0, 0, 1);
	_view.sampleCount = 1;
		
	_view.device = MTLCreateSystemDefaultDevice();
	
    if(!_view.device)
    {
        NSLog(@"Metal is not supported on this device");
        self.view = [[UIView alloc] initWithFrame:self.view.frame];
        return;
    }
	self.view = _view;
	Zen::Metal::Device::Get()->getID()->setMTKView(_view);
	
	_launched = false;
}

- (void)viewWillAppear:(BOOL)animated {
	
	[super viewWillAppear:animated];

	_view.delegate = self;

//	runtime->launch(...);
//	see& mtkView drawableSizeWillChange
}

- (void)dealloc
{
	if(G_view_controller == self)
	{
		G_view_controller = nil;
	}
}

- (void)drawInMTKView:(nonnull MTKView *)view
{
	// draw
	if(!_launched) return;
	
	runtime->update();
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
	[super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
}

- (Zen::AppTouch) toAppTouch:(UITouch*)uiTouch
{
	Zen::AppTouch touch;

	CGPoint pos = [uiTouch locationInView:self.view];
	CGSize size = self.view.bounds.size;

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
	_view.preferredFramesPerSecond = fps;
}
-(int)getFPS
{
	return (int)_view.preferredFramesPerSecond;
}

@end

#endif
