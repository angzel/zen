#include "zen_macro.h"

#if !ZEN_RUNTIME_VISUAL

#include "zen_utils.h"
#include "zen_file.h"
#include <sys/ioctl.h>
#include <unistd.h>

#include "zen_urlcoding.h"

namespace Zen {

	class Utils_i : public Utils
	{
	public:
		std::string _getDocumentPath(std::string const & s)
		{
			if(s.size() && s[0] == '/') {
				return s;
			}
			else return "./" + s;
		}
		
		std::string getAppID() override
		{
			return "";
		}
		std::string getDeviceID() override
		{
			return "";
		}

		bool isResourceExist(std::string const & path) override
		{
			return access(path.data(), 0) == 0;
		}

		bool isDocumentExist(std::string const & path) override
		{
			auto doc = _getDocumentPath(path);
			return access(doc.data(), 0) == 0;
		}

		std::vector<uint8_t> loadResource(std::string const & path) override
		{
			return LoadFileToBuffer(path);
		}

		std::vector<uint8_t> loadDocument(std::string const & path) override
		{
			return LoadFileToBuffer(_getDocumentPath(path));
		}

		bool saveDocument(std::string const & path, void const * data, size_t size) override
		{
			return WriteBufferToFile(_getDocumentPath(path), data, size);
		}

		std::vector<uint8_t> loadURL(std::string const & url) override
		{
			std::string path = _getDocumentPath("_utils_load_url.temp~");
			std::string cmd = "curl " + URLEncode(url) + " -o " + path + " 1>&- 2>&-";
			system(cmd.data());
			return loadResource(path);
		}

		std::string getSystemLanguage() override
		{
			return "";
		}
	public:
		static Utils_i * Get()
		{
			static auto me = new Utils_i;
			return me;
		}
	};

	Utils * Utils::Get()
	{
		return Utils_i::Get();
	}
}

#endif
