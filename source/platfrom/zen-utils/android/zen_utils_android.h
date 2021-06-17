#pragma once

#include "zen_macro.h"

#if defined(ZEN_OS_ANDROID)

#include <thread>
#include <string>
#include "zen_log.h"

#include <cstdint>
#include <vector>
#include <unistd.h>
#include <jni.h>

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
	class AndroidJNI
	{
	public:
		static AndroidJNI * Get();

		JNIEnv * getJNIEnv()
        {
		    JNIEnv * env = nullptr;
            if(mJavaVM->GetEnv((void**)&env, mJNIVersion) == JNI_OK)
            {
                Zen::LogD("java vm env:%x", env);
                return env;
            }
            return nullptr;
        }

		JavaVM * getJavaVM()
        {
		    return mJavaVM;
        }

        jint getJNIVersion()
        {
            return mJNIVersion;
        }

		void setJavaVM(JavaVM *  vm, jint version)
        {
		    mJavaVM = vm;
		    mJNIVersion = version;
        }

	protected:
        JavaVM * mJavaVM = nullptr;
		jint mJNIVersion;
	};
}

#endif
