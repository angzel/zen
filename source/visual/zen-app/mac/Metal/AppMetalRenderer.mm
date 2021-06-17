//
//  Renderer.m
//  macMetalGame
//
//  Created by MeherTj on 2020/12/19.
//

#import <simd/simd.h>
#import <ModelIO/ModelIO.h>

#import "AppMetalRenderer.h"
#import "zen_app_mac.h"

AppMetalRenderer * gRenderer = nullptr;

@implementation AppMetalRenderer
{
}

- (id)init {
	id re = [super init];
	gRenderer = (AppMetalRenderer*)re;
	return re;
}
- (void)onLaunch:(MTKView*)view
{
	CGSize size = view.bounds.size;
	AppMac::Get()->launch({ (float)size.width, (float)size.height });
}

- (void)drawInMTKView:(nonnull MTKView *)view
{
	AppMac::Get()->update();
	AppMac::Get()->draw();
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    /// Respond to drawable size or orientation changes here
	AppMac::Get()->resize({(float)size.width, (float)size.height});
}

@end

AppMetalRenderer * G_AppRenderer = nullptr;
