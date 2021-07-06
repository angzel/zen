#include "zen_macro.h"

#if ZEN_RUNTIME_VISUAL && ZEN_OS_APPLE_MAC

#include "zen_utils_mac.h"
#import <Foundation/Foundation.h>

namespace Zen {

	class UtilsMac_i : public UtilsMac
	{
	public:
		NSString* DocumentPath(NSString* path) override
		{
			NSString *home= [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
			return [home stringByAppendingPathComponent:path];
		}

		NSString * ResourcePath(NSString* path) override
		{
			return [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:path];
		}
		
		NSString * toString(std::string const & s) override
		{
			return [NSString stringWithUTF8String:s.data()];
		}
		
		std::string toString(NSString * s) override
		{
			return [s UTF8String];
		}
		
		std::string getAppID() override
		{
			auto s = NSBundle.mainBundle.bundleIdentifier;
			if(s == 0) return "";
			return s.UTF8String;
		}
		std::string getDeviceID() override
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
			if(ret == 0) return "";
			return [ret UTF8String];
		}
		
		std::string getDocumentPath(std::string const & path) override {
			return toString(DocumentPath(toString(path)));
		}
		
		bool isResourceExist(std::string const & path) override
		{
			NSFileManager * manager = [NSFileManager defaultManager];
			return [manager fileExistsAtPath:ResourcePath(toString(path))];

		}

		bool isDocumentExist(std::string const & path) override
		{
			NSFileManager * manager = [NSFileManager defaultManager];
			return [manager fileExistsAtPath:DocumentPath(toString(path))];
		}

		std::vector<uint8_t> loadResource(std::string const & path) override
		{
			auto data = [NSData dataWithContentsOfFile:ResourcePath(toString(path))];
			if(data == nil) return {};

			auto bytes = (uint8_t const*)data.bytes;
			auto size = data.length;
			return std::vector<uint8_t>(bytes + 0, bytes + size);
		}

		std::vector<uint8_t> loadDocument(std::string const & path) override
		{
			auto data = [NSData dataWithContentsOfFile:DocumentPath(toString(path))];
			if(data == nil) return {};

			auto bytes = (uint8_t const*)data.bytes;
			auto size = data.length;
			return std::vector<uint8_t>(bytes + 0, bytes + size);
		}

		bool saveDocument(std::string const & path, void const * data, size_t size) override
		{
			NSData * ocd = [NSData dataWithBytes:data length:size];
			return [ocd writeToFile:DocumentPath(toString(path)) atomically:YES];
		}

		std::vector<uint8_t> loadURL(std::string const & url) override
		{
			auto content = [NSData dataWithContentsOfURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.data()]]];
			if(content == nil) return {};

			auto data = (uint8_t const *)content.bytes;
			auto size = content.length;
			return std::vector<uint8_t>(data, data + size);
		}

		std::string getSystemLanguage() override
		{
			NSUserDefaults * defaults = [NSUserDefaults standardUserDefaults];

			NSArray * allLanguages = [defaults objectForKey:@"AppleLanguages"];

			NSString * preferredLang = [allLanguages objectAtIndex:0];

			NSLog(@"current lang:%@", preferredLang);

			if(!preferredLang) return "";
			return [preferredLang UTF8String];
		}
	public:
		static UtilsMac_i * Get()
		{
			static auto me = new UtilsMac_i;
			return me;
		}
	};

	UtilsMac * UtilsMac::Get()
	{
		return UtilsMac_i::Get();
	}
	Utils * Utils::Get()
	{
		return UtilsMac_i::Get();
	}
}

#endif
