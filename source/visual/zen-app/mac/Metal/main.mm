//
//  main.m
//  macMetalGame
//
//  Created by MeherTj on 2020/12/19.
//

#import <Cocoa/Cocoa.h>
#import "AppMetalDelegate.h"
#include "zen_app.h"

int main(int argc, const char * argv[]) {
	
	ZenAppMain(argc, argv);
	
    @autoreleasepool
	{
        // Setup code that might create autoreleased objects goes here.
		auto app = [NSApplication sharedApplication];
		auto delegate = [[AppMetalDelegate alloc] init];
		[app setDelegate:delegate];
		[app run];
    }
	return 0;
}
