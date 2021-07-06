#include "zen_macro.h"

#if defined(ZEN_OS_ANDROID)

#include "zen_utils.h"
#include "zen_utils_android.h"
#include "zen_exception.h"
#include "zen_utils_android.h"
#include "zen_log.h"
#include "zen_file.h"
#include "zen_utils_android.h"

static JavaVM *sJavaVM = nullptr;
static jint sJNIVersion = 0;

extern "C" JNIEXPORT void JNICALL
Java_zen_utils_Utils_initializeNative(JNIEnv *env, jclass clazz) {
    Zen::LogD("init env: %x, %s", env, __func__);

    if (sJavaVM) return;

    JavaVM *vm = nullptr;

    if (env->GetJavaVM(&vm) == JNI_OK) {
        auto version = env->GetVersion();
        sJavaVM = vm;
        sJNIVersion = version;
        Zen::LogV("set java vm. %s vm:%x, version:%d", __func__, vm, version);
    } else {
        Zen::LogW("java vm not set. %s", __func__);
    }
}

namespace Zen {
    using namespace std;

#define CLASS_NAME "zen/utils/Utils"

    class UtilsAndroid_i : public UtilsAndroid {
    public:
        JNIEnv *getJNIEnv() override {
            auto vm = getJavaVM();
            if (!vm) {
                Zen::LogE("java vm not setting. %s", __func__);
                return nullptr;
            }

            JNIEnv *env = nullptr;
            if (vm->GetEnv((void **) &env, getJNIVersion()) == JNI_OK) {
                Zen::LogV("%s env: %d", __func__, env);
                return env;
            }
            Zen::LogE("java vm get NULL env");
            return nullptr;
        }

        JavaVM *getJavaVM() override {
            if (!sJavaVM) {
                Zen::LogE(
                        "call:%f error:javaVM is nil, you should call zen_utils_Utils_initializeNative first",
                        __func__);
            }
            return sJavaVM;
        }

        int getJNIVersion() override {
            return (int) sJNIVersion;
        }

        /// callJava
        std::tuple<JNIEnv *, jclass, jmethodID> findJavaMethod
                (cstring &cls, cstring &method, cstring &type) override {
            JNIEnv *env = getJNIEnv();
            musts(env, "null java JNIEnv");

            jclass jc = env->FindClass(cls.c_str());
            musts(jc, "cannot find java class:" + cls);

            jmethodID jm = env->GetStaticMethodID(jc, method.c_str(), type.c_str());

            musts(jm, "cannot find java method:" + method);

            return {env, jc, jm};
        }

        void callJavaVoidMethod(cstring &cls, cstring &fun) override {
            auto j = findJavaMethod(cls, fun, "()V");
            JNIEnv *env = std::get<0>(j);
            env->CallStaticVoidMethod(std::get<1>(j), std::get<2>(j));
        }

        void callJavaVoidMethod(cstring &cls, cstring &fun, cstring &arg0) override {
            auto j = findJavaMethod(cls, fun, "(Ljava/lang/String;)V");
            JNIEnv *env = std::get<0>(j);
            jstring js0 = env->NewStringUTF(arg0.c_str());
            env->CallStaticVoidMethod(std::get<1>(j), std::get<2>(j), js0);
        }

        bool callJavaBoolMethod(cstring &cls, cstring &fun) override {

            auto j = findJavaMethod(cls, fun, "()Z");
            JNIEnv *env = std::get<0>(j);
            return JNI_FALSE != env->CallStaticBooleanMethod(std::get<1>(j), std::get<2>(j));
        }

        bool callJavaBoolMethod(cstring &cls, cstring &fun, cstring &arg0) override {
            auto j = findJavaMethod(cls, fun, "(Ljava/lang/String;)Z");
            JNIEnv *env = std::get<0>(j);
            auto js0 = env->NewStringUTF(arg0.c_str());
            return JNI_FALSE != env->CallStaticObjectMethod(std::get<1>(j), std::get<2>(j), js0);
        }

        std::string callJavaStringMethod(cstring &cls, cstring &fun) override {
            auto j = findJavaMethod(cls, fun, "()Ljava/lang/String;");
            JNIEnv *env = std::get<0>(j);
            auto res = env->CallStaticObjectMethod(std::get<1>(j), std::get<2>(j));
            if (!res) {
                LogW("%s::%s return null string.", cls.data(), fun.data());
                return "";
            }
            return env->GetStringUTFChars((jstring)res, 0);
        }

        std::string callJavaStringMethod(cstring &cls, cstring &fun, cstring &arg0) override {
            auto j = findJavaMethod(cls, fun, "(Ljava/lang/String;)Ljava/lang/String;");
            JNIEnv *env = std::get<0>(j);
            auto js0 = env->NewStringUTF(arg0.c_str());
            auto res = env->CallStaticObjectMethod(std::get<1>(j), std::get<2>(j), js0);
            if (!res) {
                LogW("%s::%s return null string.", cls.data(), fun.data());
                return "";
            }
            return env->GetStringUTFChars((jstring)res, 0);
        }

        std::vector<uint8_t> callJavaBytesMethod(cstring &cls, cstring &fun) override {
            auto j = findJavaMethod(cls, fun, "()[B");
            JNIEnv *env = std::get<0>(j);
            auto res = env->CallStaticObjectMethod(std::get<1>(j), std::get<2>(j));
            if (!res) {
                LogW("%s::%s return null object.", cls.data(), fun.data());
                return {};
            }
            auto len = env->GetArrayLength((jbyteArray)res);
            auto data = env->GetByteArrayElements((jbyteArray)res, 0);
            return std::vector<uint8_t>(data, data + len);
        }

        std::vector<uint8_t>
        callJavaBytesMethod(cstring &cls, cstring &fun, cstring &arg0) override {
            auto j = findJavaMethod(cls, fun, "(Ljava/lang/String;)[B");
            JNIEnv *env = std::get<0>(j);
            auto js0 = env->NewStringUTF(arg0.c_str());
            auto res = env->CallStaticObjectMethod(std::get<1>(j), std::get<2>(j), js0);
            if (!res) {
                LogW("%s::%s return null object.", cls.data(), fun.data());
                return {};
            }
            auto len = env->GetArrayLength((jbyteArray)res);
            auto data = env->GetByteArrayElements((jbyteArray)res, 0);
            return std::vector<uint8_t>(data, data + len);
        }
    public:

        std::string getFilesDir() override {
            static std::string path;
            if (path.size()) return path;

            path = callJavaStringMethod(CLASS_NAME, "getFilesDir");
            return path;
        }

        std::string getCacheDir() override {
            static std::string path;
            if (path.size()) return path;
            path = callJavaStringMethod(CLASS_NAME, "getCacheDir");
            return path;
        }

        std::string getExternalCacheDir() override {
            static std::string path;
            if (path.size()) return path;
            path = callJavaStringMethod(CLASS_NAME, "getExternalCacheDir");
            return path;
        }

        std::string getDocumentPath(std::string const &path) override {
            if (path.empty()) return getFilesDir();
            if (path[0] == '/') return path;
            return getFilesDir() + "/" + path;
        }

        std::string getAppID() override {
            static std::string app_id;
            if (app_id.size()) return app_id;

            app_id = callJavaStringMethod(CLASS_NAME, "getAppID");
            if(app_id.empty()) {
                LogW("cannot get a valid package name");
            }
            return app_id;
        }

        std::string getDeviceID() override {
            static std::string device_id;
            if (device_id.size()) return device_id;

            device_id = callJavaStringMethod(CLASS_NAME, "getDeviceID");
            if(device_id.empty()) {
                LogW("cannot get a valid device id");
            }
            return device_id;
        }

        bool isResourceExist(std::string const &path) override {

            return callJavaBoolMethod(CLASS_NAME, "isResourceExist", path);
        }

        std::vector<uint8_t> loadResource(std::string const &path) override {
            return callJavaBytesMethod(CLASS_NAME, "loadResource", path);
        }

        bool isDocumentExist(std::string const &path) override {
            return access(getDocumentPath(path).c_str(), 0) == 0;
        }

        std::vector<uint8_t> loadDocument(std::string const &path) override {
            auto doc = getDocumentPath(path);
            LogV("%s path:%s", __func__, doc.data());
            return Zen::LoadFileToBuffer(doc);
        }

        bool saveDocument(std::string const &path, void const *data, size_t size) override {
            auto doc = getDocumentPath(path);
            LogV("%s path:%s %x %u", __func__, doc.data(), data, size);
            auto ok = Zen::WriteBufferToFile(doc.data(), data, size);
            if (!ok) {
                LogW("%s failed! path:%s", doc.data());
            }
            return ok;
        }

        std::vector<uint8_t> loadURL(std::string const &url) override {

            return callJavaBytesMethod(CLASS_NAME, "loadURL", url);
        }

        std::string getSystemLanguage() override {
            return callJavaStringMethod(CLASS_NAME, "getSystemLanguage");
        }

        static UtilsAndroid_i *Get() {
            static auto me = new UtilsAndroid_i;
            return me;
        }
    };

    UtilsAndroid *UtilsAndroid::Get() {
        return UtilsAndroid_i::Get();
    }

    Utils *Utils::Get() {
        return UtilsAndroid_i::Get();
    }
}

#endif
