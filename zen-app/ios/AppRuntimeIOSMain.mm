#include "zen_app_config.h"
#include "zen_app_runtime.h"

#include "AppRuntimeIOS.h"

int main(int argc, char * argv[]) {
	
	if(ZenAppMain(argc, (const char **)argv) != 0) return -1;

	@autoreleasepool {
	    return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
	}
}