#if defined(__ANDROID__)

#include <jni.h>
#include <thread>
#include <string>
#include <unistd.h>
#include <sys/stat.h>

#define ZEN_APP_FUNC(name) JNIEXPORT void JNICALL Java_zen_app_AppNative_ ## name
#define ZEN_APP_NAME "void Java_zen_app_AppNative"
#define ZEN_FUNC_R (__func__ + sizeof(NAME_PRE))

#define CLASS_NAME "zen/app/AppNative"

#include "zen_utils_android.h"
#include "zen_app.h"
#include "zen_log.h"
#include "zen_exception.h"
#include "AppNative.h"
#include "AppRuntimeAndroid.h"

#define LOG_FUNC() Zen::LogV(__FUNCTION__)

//static JNIEnv * sMainEnv = nullptr;
//static JNIEnv * sGLEnv = nullptr;

//static std::thread::id sMainThreadID;
//static std::thread::id sGLThreadID;
//static std::string sDocumentHome;
//static std::string sDataDir;
//static std::string sPackageName;

static int &sGetThreadNo() {
    thread_local int no = 0;
    return no;
}

namespace Zen {
    namespace Android {

        void SetFramePerSecond(float fps) {
            auto env = Zen::UtilsAndroid::Get()->getJNIEnv();
            musts(env, "invalid jni env");

            jclass cls = env->FindClass(CLASS_NAME);
            musts(cls, "cannot find java class:" CLASS_NAME);

            jmethodID method = env->GetStaticMethodID(cls, "SetFramesPerSecond", "(F)V");

            musts(method, "cannot find java method: SetFramePerSecond");

            env->CallStaticVoidMethod(cls, method, (jfloat) fps);
        }
    }
}


/**
 order:

 Java_zen_app_AppNative_OnActivityCreate
 Java_zen_app_AppNative_OnCreate
 Java_zen_app_AppNative_OnResize
 */
extern "C" {
ZEN_APP_FUNC(OnActivityCreate)(JNIEnv *env, jclass obj);
ZEN_APP_FUNC(OnCreate)(JNIEnv *env, jclass obj);
ZEN_APP_FUNC(OnResize)(JNIEnv *env, jclass obj, jint width, jint height);
ZEN_APP_FUNC(OnStep)(JNIEnv *env, jclass obj);
ZEN_APP_FUNC(OnPause)(JNIEnv *env, jclass obj);
ZEN_APP_FUNC(OnResume)(JNIEnv *env, jclass obj);

ZEN_APP_FUNC(OnMotionDown)(JNIEnv *env, jclass obj, jint no, jfloat x, jfloat y);
ZEN_APP_FUNC(OnMotionUp)(JNIEnv *env, jclass obj, jint no, jfloat x, jfloat y);
ZEN_APP_FUNC(OnMotionMove)(JNIEnv *env, jclass obj, jint no, jfloat x, jfloat y);
ZEN_APP_FUNC(OnMotionCancel)(JNIEnv *env, jclass obj, jint no, jfloat x, jfloat y);
ZEN_APP_FUNC(OnBack)(JNIEnv *env, jclass obj);
}

#define runtime Zen::AppRuntimeAndroid::Get()

extern "C" {
static int s_view_width = 0;
static int s_view_height = 0;

static bool s_inited = false;
static bool s_paused = false;

ZEN_APP_FUNC(OnActivityCreate)(JNIEnv *env, jclass obj) {
//	sMainEnv = env;
//	sMainThreadID = std::this_thread::get_id();
//	auto thNo = sGetThreadNo();
//	Zen::LogV("ThreadNo:%d", thNo);
//	sGetThreadNo() = 1;
    LOG_FUNC();
}

ZEN_APP_FUNC(OnCreate)(JNIEnv *env, jclass obj) {
//	auto thNo = sGetThreadNo();
//	Zen::LogV("ThreadNo:%d", thNo);
//	sGetThreadNo() = 2;
//	sGLEnv = env;
//	sGLThreadID = std::this_thread::get_id();

    ZenAppMain(0, nullptr);

    s_inited = false;

    LOG_FUNC();
}
ZEN_APP_FUNC(OnResize)(JNIEnv *env, jclass obj, jint width, jint height) {
    Zen::LogV("RESIZE %d %d", width, height);

    if (s_inited) {
        if (s_view_width != width || s_view_height != height) {
            runtime->resize({(float) width, (float) height});
        } else {
            return;
        }
    } else {
        runtime->launch({(float) width, (float) height});
        s_inited = true;
    }

    s_view_width = width;
    s_view_height = height;
}

ZEN_APP_FUNC(OnStep)(JNIEnv *env, jclass obj) {
    runtime->update();
    runtime->draw();
}

ZEN_APP_FUNC(OnPause)(JNIEnv *env, jclass obj) {
    LOG_FUNC();
    runtime->pause();
}

ZEN_APP_FUNC(OnResume)(JNIEnv *env, jclass obj) {
    LOG_FUNC();
    if (s_inited) runtime->resume();
}

static const int MaxTouchCount = 16;
static Zen::AppTouch &sFix(Zen::AppTouch &touch) {
    touch.x /= s_view_width;
    touch.y = 1 - touch.y / s_view_height;
    return touch;
}
static bool sIsValidTouch(int no) {
    return no < MaxTouchCount && (runtime->isMultiTouch() || no == 0);
}

ZEN_APP_FUNC(OnMotionDown)(JNIEnv *env, jclass obj, jint no, jfloat x, jfloat y) {
    if (!sIsValidTouch(no)) return;

    Zen::AppTouch touch;
    touch.no = no;
    touch.x = x;
    touch.y = y;
    runtime->touchDown(sFix(touch));

    Zen::LogV("%s %f %f (%d)", __FUNCTION__, touch.x, touch.y, touch.no);
}

ZEN_APP_FUNC(OnMotionUp)(JNIEnv *env, jclass obj, jint no, jfloat x, jfloat y) {
    if (!sIsValidTouch(no)) return;
    Zen::AppTouch touch;
    touch.x = x;
    touch.y = y;
    touch.no = no;
    runtime->touchUp(sFix(touch));
    Zen::LogV("%s %f %f (%d)", __FUNCTION__, touch.x, touch.y, touch.no);
}
ZEN_APP_FUNC(OnMotionMove)(JNIEnv *env, jclass obj, jint no, jfloat x, jfloat y) {
    if (!sIsValidTouch(no)) return;
    Zen::AppTouch touch;
    touch.no = no;
    touch.x = x;
    touch.y = y;
    runtime->touchMove(sFix(touch));
}
ZEN_APP_FUNC(OnMotionCancel)(JNIEnv *env, jclass obj, jint no, jfloat x, jfloat y) {
    Zen::LogV("%s %d", __FUNCTION__, no);
    if (!sIsValidTouch(no)) return;

    Zen::AppTouch touch;
    touch.no = no;
    touch.x = x;
    touch.y = y;
    runtime->touchCancel(sFix(touch));
}
ZEN_APP_FUNC(OnBack)(JNIEnv *env, jclass obj) {
    runtime->back();
    LOG_FUNC();
}
}

#endif
