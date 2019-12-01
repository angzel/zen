#pragma once

#include <string>
#include <vector>

namespace Zen { namespace System {

	std::string GetPackageName();

	std::string GetDeviceUniqueID();

	bool IsResourceExist(std::string const & path);

	bool IsDocumentExist(std::string const & path);

	std::vector<uint8_t> LoadResourceContent(std::string const & path);

	std::vector<uint8_t> LoadDocumentContent(std::string const & path);

	bool SaveDocument(std::string const & path, void const * data, size_t size);

	std::vector<uint8_t> LoadDataFromURL(std::string const & url);

	std::string GetSystemLanguage();
}}
