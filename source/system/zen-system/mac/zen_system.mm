#include "zen_os.h"

#if defined(ZEN_OS_MAC)
#include "zen_system.h"

#import <Foundation/Foundation.h>

static NSString* _DocumentPath(std::string const & path)
{
	NSString *home= [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
	return [home stringByAppendingPathComponent:[NSString stringWithUTF8String:path.data()]];
}

static NSString * _ResourcePath(std::string const & path)
{
	return [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:[NSString stringWithUTF8String:path.data()]];
}

namespace Zen {
	std::string GetPackageName()
	{
		return [[[NSBundle mainBundle] bundleIdentifier] UTF8String];
	}
	std::string GetDeviceUniqueID()
	{
		NSString * ret = nil;
		io_service_t platformExpert ;
		platformExpert = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching("IOPlatformExpertDevice")) ;

		if (platformExpert) {
			CFTypeRef uuidNumberAsCFString = IORegistryEntryCreateCFProperty(platformExpert, CFSTR("IOPlatformSerialNumber"), kCFAllocatorDefault, 0) ;
			if (uuidNumberAsCFString)   {
				ret = CFBridgingRelease(uuidNumberAsCFString);
				CFRelease(uuidNumberAsCFString); uuidNumberAsCFString = NULL;
			}
			IOObjectRelease(platformExpert); platformExpert = 0;
		}

		return [ret UTF8String];

	}

	bool IsResourceExist(std::string const & path)
	{
		NSFileManager * manager = [NSFileManager defaultManager];
		return [manager fileExistsAtPath:_ResourcePath(path)];

	}

	bool IsDocumentExist(std::string const & path)
	{
		NSFileManager * manager = [NSFileManager defaultManager];
		return [manager fileExistsAtPath:_DocumentPath(path)];
	}

	std::vector<uint8_t> LoadResourceContent(std::string const & path)
	{
		auto data = [NSData dataWithContentsOfFile:_ResourcePath(path)];
		if(data == nil) return {};

		auto bytes = (uint8_t const*)data.bytes;
		auto size = data.length;
		return std::vector<uint8_t>(bytes + 0, bytes + size);
	}

	std::vector<uint8_t> LoadDocumentContent(std::string const & path)
	{
		auto data = [NSData dataWithContentsOfFile:_DocumentPath(path)];
		if(data == nil) return {};

		auto bytes = (uint8_t const*)data.bytes;
		auto size = data.length;
		return std::vector<uint8_t>(bytes + 0, bytes + size);
	}

	bool SaveDocument(std::string const & path, void const * data, size_t size)
	{
		NSData * ocd = [NSData dataWithBytes:data length:size];
		return [ocd writeToFile:_DocumentPath(path) atomically:YES];
	}

	std::vector<uint8_t> LoadDataFromURL(std::string const & url)
	{
		auto content = [NSData dataWithContentsOfURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.data()]]];
		if(content == nil) return {};

		auto data = (uint8_t const *)content.bytes;
		auto size = content.length;
		return std::vector<uint8_t>(data, data + size);
	}

	std::string GetSystemLanguage()
	{
		NSUserDefaults * defaults = [NSUserDefaults standardUserDefaults];

		NSArray * allLanguages = [defaults objectForKey:@"AppleLanguages"];

		NSString * preferredLang = [allLanguages objectAtIndex:0];

		NSLog(@"当前语言:%@", preferredLang);
		
		if(!preferredLang) return "";
		return [preferredLang UTF8String];
	}
}

#endif
