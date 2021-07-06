#pragma once

#include "zen_macro.h"

#if defined(ZEN_OS_ANDROID)

#include <thread>
#include <string>
#include <cstdint>
#include <vector>
#include <unistd.h>
#include <jni.h>

#include "zen_log.h"
#include "zen_utils.h"
// you should set javavm when your app start.
// use JNI_OnLoad is a simple way.
#if 0
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    jint version = JNI_VERSION_1_6;
    Zen::Android::Get()->setJavaVM(vm, version);
    return version;
}
#endif

namespace Zen {
    class UtilsAndroid : public Utils {
    public:
        static UtilsAndroid *Get();

        virtual JNIEnv *getJNIEnv() = 0;

        virtual JavaVM *getJavaVM() = 0;

        virtual int getJNIVersion() = 0;

    public:
        typedef std::string const cstring;

        virtual std::tuple<JNIEnv *, jclass, jmethodID> findJavaMethod
                (cstring &cls, cstring &method, cstring &type) = 0;

        virtual void callJavaVoidMethod(cstring &cls, cstring &fun) = 0;

        virtual void callJavaVoidMethod(cstring &cls, cstring &fun, cstring &arg0) = 0;

        virtual bool callJavaBoolMethod(cstring &cls, cstring &fun) = 0;

        virtual bool callJavaBoolMethod(cstring &cls, cstring &fun, cstring &arg0) = 0;

        virtual std::string callJavaStringMethod(cstring &cls, cstring &fun) = 0;

        virtual std::string callJavaStringMethod(cstring &cls, cstring &fun, cstring &arg0) = 0;

        virtual std::vector<uint8_t> callJavaBytesMethod(cstring &cls, cstring &fun) = 0;

        virtual std::vector<uint8_t> callJavaBytesMethod(cstring &cls, cstring &fun, cstring &arg0) = 0;

    public:
        virtual std::string getFilesDir() = 0;

        virtual std::string getCacheDir() = 0;

        virtual std::string getExternalCacheDir() = 0;

    protected:
        UtilsAndroid() = default;
    };
}

#endif
