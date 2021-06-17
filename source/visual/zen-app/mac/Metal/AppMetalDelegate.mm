//
//  AppMetalDelegate.m
//  macMetalGame
//
//  Created by MeherTj on 2020/12/19.
//

#import "AppMetalDelegate.h"
#import "zen_app_mac.h"
#import "AppMetalViewController.h"

@interface AppMetalDelegate ()

@end

@implementation AppMetalDelegate
{
	BOOL paused;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	paused = NO;
    // Insert code here to initialize your application
	
	NSWindowStyleMask mask =
	NSWindowStyleMaskResizable | NSWindowStyleMaskClosable |
	NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskTitled |
	NSWindowStyleMaskFullSizeContentView | NSWindowStyleMaskUnifiedTitleAndToolbar;

	NSBackingStoreType store = NSBackingStoreBuffered;
	
	// init window
	auto screen = [[NSScreen mainScreen] frame];
	auto rect = NSMakeRect(0, 0, screen.size.width, screen.size.height);
	auto window = [[NSWindow alloc] initWithContentRect:rect styleMask:mask backing:store defer:NO];
	[window setContentMinSize:NSMakeSize(400, 300)];
	[window setBackgroundColor:[NSColor colorWithRed:0.2 green:0.2 blue:0.5 alpha:1.0]];
	[window setWindowController:[[NSWindowController alloc] init]];
	[window makeKeyAndOrderFront:nil];
	// toolbar
	[window setToolbar:[[NSToolbar alloc] init]];
	[window toggleToolbarShown:nil];
	
	[window setReleasedWhenClosed:NO];
//	[window toggleFullScreen:nil];
	
	auto vc = [[AppMetalViewController alloc] init];
	[window setContentViewController:vc];
}

-(void)applicationWillResignActive:(NSNotification *)notification
{
	if(!AppMac::Get()->isBGUpdate()) gViewController.mtkView.paused = YES;
	
	auto del = Zen::App::Get()->getRuntimeDelegate();
	if(del) del->onPause();
}

-(void)applicationWillBecomeActive:(NSNotification *)notification
{
	auto del = Zen::App::Get()->getRuntimeDelegate();
	if(del) del->onResume();
	
	gViewController.mtkView.paused = NO;	
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

@end
