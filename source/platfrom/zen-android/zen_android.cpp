#include "zen_macro.h"

#if defined(ZEN_OS_ANDROID)
#include "zen_utils.h"
#include "zen_utils_android.h"
#include "zen_exception.h"
#include "zen_utils_android.h"
#include "zen_log.h"
#include "zen_file.h"
#include "zen_utils_android.h"

extern "C" JNIEXPORT void JNICALL
        Java_zen_platform_Utils_initializeNative(JNIEnv* env, jclass clazz)
{
    Zen::LogD("init env:%x", env);
    if(Zen::AndroidJNI::Get()->getJavaVM() == nullptr) {
        JavaVM *vm = nullptr;

        if (env->GetJavaVM(&vm) == JNI_OK) {
            auto version = env->GetVersion();
            Zen::AndroidJNI::Get()->setJavaVM(vm, version);
            Zen::LogV("set java vm when [Utils_initializeNative] vm:%x, version:%d", vm, version);
        } else {
            Zen::LogW("java vm is not set");
        }
    }
}


namespace Zen {
    AndroidJNI * AndroidJNI::Get() {
        static auto me = new AndroidJNI;
        return me;
    }
}
namespace Zen {
	using namespace std;

#define CLASS_NAME "zen/platform/Utils"

	class UtilsAndroid : public Utils
    {
    public:
        std::string mDeviceID = "";

        std::string mDataDir = "";

        std::string mPackageID = "";

        std::string _getDataDir()
        {
            if(mDataDir.size()) return mDataDir;

            JNIEnv *env = Zen::AndroidJNI::Get()->getJNIEnv();
            if (!env) return "";

            jclass cls = env->FindClass(CLASS_NAME);
            musts(cls, "cannot find java class:" CLASS_NAME);

            jmethodID method = env->GetStaticMethodID(cls, "getDataDir", "()Ljava/lang/String;");

            musts(method, "cannot find java method: GetDocumentHome");

            auto res = (jstring) env->CallStaticObjectMethod(cls, method);

            if (res) {
                return (mDataDir = env->GetStringUTFChars(res, nullptr));
            }
            else {
                return "";
            }

        }

        std::string getAppID() override
        {
            if(mPackageID.size()) return mPackageID;

            JNIEnv *env = Zen::AndroidJNI::Get()->getJNIEnv();
            if (!env) return "";

            auto cls1 = env->FindClass("android/widget/TextView");

            musts(cls1, "cannot find java class:" "android/widget/TextView");

            static jclass cls;
            cls = env->FindClass(CLASS_NAME);
            musts(cls, "cannot find java class:" CLASS_NAME "-Y");

            jmethodID method = env->GetStaticMethodID(cls, "getAppID", "()Ljava/lang/String;");

            musts(method, "cannot find java method: getAppID");

            auto res = (jstring) env->CallStaticObjectMethod(cls, method);

            if (res) {
                return (mPackageID = env->GetStringUTFChars(res, nullptr));
            }
            else {
                return "";
            }

        }

        std::string getDeviceID() override {
            if(mDeviceID.size()) return mDeviceID;

            JNIEnv *env = Zen::AndroidJNI::Get()->getJNIEnv();
            if (!env) return "";

            jclass cls = env->FindClass(CLASS_NAME);
            musts(cls, "cannot find java class:" CLASS_NAME);

            jmethodID method = env->GetStaticMethodID(cls, "getDeviceID", "()Ljava/lang/String;");

            musts(method, "cannot find java method: getDeviceID");

            auto res = (jstring) env->CallStaticObjectMethod(cls, method);

            if (res) {
                return (mDeviceID = env->GetStringUTFChars(res, nullptr));
            }
            else {
                return "";
            }
        }

        bool isResourceExist(std::string const &path) override {

            JNIEnv *env = Zen::AndroidJNI::Get()->getJNIEnv();
            if (!env) return false;

            jclass cls = env->FindClass(CLASS_NAME);
            musts(cls, "cannot find java class:" CLASS_NAME);

            jmethodID method = env->GetStaticMethodID(cls, "isResourceExist", "(Ljava/lang/String;)Z");

            musts(method, "cannot find java method: isResourceExist");

            auto j_path = env->NewStringUTF(path.data());

            auto res = env->CallStaticBooleanMethod(cls, method, j_path);

            return (bool) res;
        }

        std::vector<uint8_t> loadResource(std::string const &path) override {

            JNIEnv *env = Zen::AndroidJNI::Get()->getJNIEnv();
            if (!env) return {};

            jclass cls = env->FindClass(CLASS_NAME);
            musts(cls, "cannot find java class:"
                    CLASS_NAME);

            jmethodID method = env->GetStaticMethodID(cls, "loadResource",
                                                      "(Ljava/lang/String;)[B");

            musts(method, "cannot find java method: loadResource");

            auto j_path = env->NewStringUTF(path.data());
            auto res = (jbyteArray) env->CallStaticObjectMethod(cls, method, j_path);

            if (res == nullptr) return {};

            auto len = env->GetArrayLength(res);
            auto data = (uint8_t const *)(void *) env->GetByteArrayElements(res, nullptr);
            return std::vector<uint8_t>(data, data + len);
        }

        bool isDocumentExist(std::string const &path) override {
            auto abs_path = _getDataDir() + path;
            return access(abs_path.data(), 0) == 0;
        }

        std::vector<uint8_t> loadDocument(std::string const &path) override {
            auto abs_path = _getDataDir() + path;
            std::fstream stream;
            stream.open(abs_path, std::ios::binary | std::ios::in);
            return Zen::LoadFileToBuffer(stream);
        }

        bool saveDocument(std::string const &path, void const *data, size_t size) override {
            auto abs_path = _getDataDir() + path;

            Zen::LogI("save doc:%s", abs_path.data());

            std::fstream stream;
            stream.open(abs_path, std::ios::binary | std::ios::out);
            return (bool) stream.write((const char *) data, size);
        }

        std::vector<uint8_t> loadURL(std::string const &url) override {
            JNIEnv *env = Zen::AndroidJNI::Get()->getJNIEnv();
            if (!env) return {};

            jclass cls = env->FindClass(CLASS_NAME);
            musts(cls, "cannot find java class:"
                    CLASS_NAME);

            jmethodID method = env->GetStaticMethodID(cls, "loadURL",
                                                      "(Ljava/lang/String;)[B");

            musts(method, "cannot find java method: loadURL");

            auto j_url = env->NewStringUTF(url.data());
            auto res = (jbyteArray) env->CallStaticObjectMethod(cls, method, j_url);

            if (res == nullptr) return {};

            auto len = env->GetArrayLength(res);
            auto data = (uint8_t const*)(void *) env->GetByteArrayElements(res, nullptr);
            return std::vector<uint8_t>(data, data + len);
        }
        std::string getSystemLanguage() override
        {
            JNIEnv *env = Zen::AndroidJNI::Get()->getJNIEnv();
            if (!env) return {};

            jclass cls = env->FindClass(CLASS_NAME);
            musts(cls, "cannot find java class:" CLASS_NAME);

            jmethodID method = env->GetStaticMethodID(cls, "getSystemLanguage", "()Ljava/lang/String;");

            musts(method, "cannot find java method: getSystemLanguage");

            auto res = (jstring) env->CallStaticObjectMethod(cls, method);

            if (res == nullptr) return "";

            return env->GetStringUTFChars(res, nullptr);
        }
    };
	Utils * Utils::Get()
    {
	    static auto me = new UtilsAndroid;
	    return me;
    }
}

#endif
