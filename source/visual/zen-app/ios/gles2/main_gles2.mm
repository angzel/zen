#include "zen_app_config.h"
#include "zen_app.h"

#if ZEN_OS_APPLE_IOS && ZEN_DRAW_GLES

#include "zen_app_ios.h"

int main(int argc, char * argv[]) {
	
	if(ZenAppMain(argc, (const char **)argv) != 0) return -1;

	@autoreleasepool {
	    return UIApplicationMain(argc, argv, nil, NSStringFromClass([GLAppDelegate class]));
	}
}
#endif
