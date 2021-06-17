//
//  GameViewController.m
//  macMetalGame
//
//  Created by MeherTj on 2020/12/19.
//

#import "AppMetalViewController.h"
#import "AppMetalRenderer.h"
#import "zen_app_mac.h"
#import "zen_metal_id.h"

@implementation AppMetalViewController
{
	AppMetalRenderer *_renderer;
}

-(id)init
{
	auto vc = [super init];
	gViewController = (AppMetalViewController*)vc;
	return vc;
}
-(void)loadView
{
	auto screen = [[NSScreen mainScreen] frame];
	auto rect = CGRectMake(0, 0, screen.size.width, screen.size.height);
	self.mtkView = [[MTKView alloc] initWithFrame:rect device:MTLCreateSystemDefaultDevice()];
	
	assert(self.mtkView.device != nil);
	
	self.view = self.mtkView;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	
	gViewController = self;
	
	if(!self.mtkView) return;
	
	Zen::Metal::Device::Get()->getID()->setMTKView(self.mtkView);

    _renderer = [[AppMetalRenderer alloc] init];

	self.mtkView.delegate = _renderer;
	
	[_renderer onLaunch:self.mtkView];
}

-(void)setFPS:(int)fps
{
	self.mtkView.preferredFramesPerSecond = fps;
}
-(int)getFPS
{
    return (int)self.mtkView.preferredFramesPerSecond;
}

@end

AppMetalViewController * gViewController = nullptr;
