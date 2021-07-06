#include "zen_macro.h"

#if ZEN_OS_APPLE_IOS
#include "zen_utils_ios.h"

namespace Zen {

	class UtilsIOS_i : public UtilsIOS
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
			return [[[NSBundle mainBundle] bundleIdentifier] UTF8String];
		}
		std::string getDeviceID() override
		{
			return [[[UIDevice currentDevice] identifierForVendor] UUIDString].UTF8String;
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
		static UtilsIOS_i * Get()
		{
			static auto me = new UtilsIOS_i;
			return me;
		}
	};

	Utils * Utils::Get()
	{
		return UtilsIOS_i::Get();
	}
	UtilsIOS * UtilsIOS::Get()
	{
		return UtilsIOS_i::Get();
	}
}

#endif
