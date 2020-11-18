#include "zen_os.h"

#if defined(ZEN_OS_ANDROID)
#include "zen_info.h"
#include "zen_android.h"
#include "zen_exception.h"
#include "zen_android.h"
#include "zen_log.h"
#include "zen_file.h"
#include <cstdint>
#include <vector>
#include <unistd.h>
#include <jni.h>


namespace Zen {
	using namespace std;

#define CLASS_NAME "zen/system/System"

	static std::string sUID = "";

	static std::string sDocumentHome = "";
	static std::string _GetDocumentHome()
	{
		if(sDocumentHome.size()) return sDocumentHome;

		JNIEnv *env = Zen::Android::GetEnv();
		if (!env) return "";

		jclass cls = env->FindClass(CLASS_NAME);
		musts(cls, "cannot find java class:" CLASS_NAME);

		jmethodID method = env->GetStaticMethodID(cls, "GetDocumentHome", "()Ljava/lang/String;");

		musts(method, "cannot find java method: GetDocumentHome");

		auto res = (jstring) env->CallStaticObjectMethod(cls, method);

		if (res) {
			return (sDocumentHome = env->GetStringUTFChars(res, nullptr));
		}
		else {
			return "";
		}

	}

	static std::string sPackageName = "";
	static std::string _GetPackageName()
	{
		if(sPackageName.size())	return sPackageName;

		JNIEnv *env = Zen::Android::GetEnv();
		if (!env) return "";

		jclass cls = env->FindClass(CLASS_NAME);
		musts(cls, "cannot find java class:" CLASS_NAME);

		jmethodID method = env->GetStaticMethodID(cls, "GetPackageName", "()Ljava/lang/String;");

		musts(method, "cannot find java method: GetPackageName");

		auto res = (jstring) env->CallStaticObjectMethod(cls, method);

		if (res) {
			return (sPackageName = env->GetStringUTFChars(res, nullptr));
		}
		else {
			return "";
		}

	}

	std::string GetDeviceUniqueID() {
		if(sUID.size()) return sUID;

		JNIEnv *env = Zen::Android::GetEnv();
		if (!env) return "";

		jclass cls = env->FindClass(CLASS_NAME);
		musts(cls, "cannot find java class:" CLASS_NAME);

		jmethodID method = env->GetStaticMethodID(cls, "GetDeviceUniqueID", "()Ljava/lang/String;");

		musts(method, "cannot find java method: GetDeviceUniqueID");

		auto res = (jstring) env->CallStaticObjectMethod(cls, method);

		if (res) {
			return (sUID = env->GetStringUTFChars(res, nullptr));
		}
		else {
			return "";
		}
	}

	bool IsResourceExist(std::string const &path) {

		JNIEnv *env = Zen::Android::GetEnv();
		if (!env) return false;

		jclass cls = env->FindClass(CLASS_NAME);
		musts(cls, "cannot find java class:" CLASS_NAME);

		jmethodID method = env->GetStaticMethodID(cls, "IsResourceExist", "(Ljava/lang/String;)Z");

		musts(method, "cannot find java method: IsResourceExist");

		auto j_path = env->NewStringUTF(path.data());

		auto res = env->CallStaticBooleanMethod(cls, method, j_path);

		return (bool) res;
	}

	std::vector<uint8_t> LoadResourceContent(std::string const &path) {

		JNIEnv *env = Zen::Android::GetEnv();
		if (!env) return {};

		jclass cls = env->FindClass(CLASS_NAME);
		musts(cls, "cannot find java class:"
			  CLASS_NAME);

		jmethodID method = env->GetStaticMethodID(cls, "LoadResourceContent",
												  "(Ljava/lang/String;)[B");

		musts(method, "cannot find java method: LoadResourceContent");

		auto j_path = env->NewStringUTF(path.data());
		auto res = (jbyteArray) env->CallStaticObjectMethod(cls, method, j_path);

		if (res == nullptr) return {};

		auto len = env->GetArrayLength(res);
		auto data = (uint8_t const *)(void *) env->GetByteArrayElements(res, nullptr);
		return std::vector<uint8_t>(data, data + len);
	}

	bool IsDocumentExist(std::string const &path) {
		auto abs_path = _GetDocumentHome() + path;
		return access(abs_path.data(), 0) == 0;
	}

	std::vector<uint8_t> LoadDocumentContent(std::string const &path) {
		auto abs_path = _GetDocumentHome() + path;
		std::fstream stream;
		stream.open(abs_path, std::ios::binary | std::ios::in);
		return Zen::LoadFileToBuffer(stream);
	}

	bool SaveDocument(std::string const &path, void const *data, size_t size) {
		auto abs_path = _GetDocumentHome() + path;

		Zen::LogI("save doc:%s", abs_path.data());

		std::fstream stream;
		stream.open(abs_path, std::ios::binary | std::ios::out);
		return (bool) stream.write((const char *) data, size);
	}

	std::vector<uint8_t> LoadDataFromURL(std::string const &url) {
		JNIEnv *env = Zen::Android::GetEnv();
		if (!env) return {};

		jclass cls = env->FindClass(CLASS_NAME);
		musts(cls, "cannot find java class:"
			  CLASS_NAME);

		jmethodID method = env->GetStaticMethodID(cls, "LoadDataFromURL",
												  "(Ljava/lang/String;)[B");

		musts(method, "cannot find java method: LoadDataFromURL");

		auto j_url = env->NewStringUTF(url.data());
		auto res = (jbyteArray) env->CallStaticObjectMethod(cls, method, j_url);

		if (res == nullptr) return {};

		auto len = env->GetArrayLength(res);
		auto data = (uint8_t const*)(void *) env->GetByteArrayElements(res, nullptr);
		return std::vector<uint8_t>(data, data + len);
	}
	std::string GetSystemLanguage()
	{
		JNIEnv *env = Zen::Android::GetEnv();
		if (!env) return {};

		jclass cls = env->FindClass(CLASS_NAME);
		musts(cls, "cannot find java class:" CLASS_NAME);

		jmethodID method = env->GetStaticMethodID(cls, "GetSystemLanguage", "()Ljava/lang/String;");

		musts(method, "cannot find java method: GetSystemLanguage");

		auto res = (jstring) env->CallStaticObjectMethod(cls, method);

		if (res == nullptr) return "";

		return env->GetStringUTFChars(res, nullptr);
	}
}

#endif
