#pragma once

#include <string>
#include <vector>

namespace Zen {
	class Utils
	{
	public:
		static Utils * Get();

		virtual std::string getAppID() = 0;

		virtual std::string getDeviceID() = 0;

		virtual bool isResourceExist(std::string const & path) = 0;

		virtual bool isDocumentExist(std::string const & path) = 0;

		virtual std::vector<uint8_t> loadResource(std::string const & path) = 0;

		virtual std::vector<uint8_t> loadDocument(std::string const & path) = 0;

		virtual bool saveDocument(std::string const & path, void const * data, size_t size) = 0;

		virtual std::vector<uint8_t> loadURL(std::string const & url) = 0;

		virtual std::string getSystemLanguage() = 0;
		
		virtual std::string getDocumentPath(std::string const & path) = 0;
	protected:
		Utils() = default;
	};
}
