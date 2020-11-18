#include "zen_app_config.h"
#include "zen_app.h"

#if defined(ZEN_OS_IOS)
#include "AppRuntimeIOS.h"

int main(int argc, char * argv[]) {
	
	if(ZenAppMain(argc, (const char **)argv) != 0) return -1;

	@autoreleasepool {
	    return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
	}
}
#endif
