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

#include "zen_android.h"
#include "zen_app_runtime.h"
#include "zen_log.h"
#include "zen_exception.h"
#include "AppNative.h"
#include "AppRuntimeAndroid.h"

#define LOG_FUNC() Zen::LogV(__FUNCTION__)

static JNIEnv * sMainEnv = nullptr;
static JNIEnv * sGLEnv = nullptr;

static std::thread::id sMainThreadID;
static std::thread::id sGLThreadID;
static std::string sDocumentHome;
static std::string sDataDir;
static std::string sPackageName;

namespace Zen {
	namespace Android {
		JNIEnv * GetMainEnv()
		{
			return sMainEnv;
		}
		JNIEnv * GetGLEnv()
		{
			return sGLEnv;
		}
		bool IsGLThread()
		{
			return std::this_thread::get_id() == sGLThreadID;
		}
		bool IsMainThread()
		{
			return std::this_thread::get_id() == sMainThreadID;
		}

		JNIEnv * GetCurrentEnv()
		{
			if(IsGLThread()) return GetGLEnv();
			else if(IsMainThread()) return GetMainEnv();
			else return nullptr;
		}

		void SetFramePerSecond(float fps) {
			auto env = GetCurrentEnv();

			jclass cls = env->FindClass(CLASS_NAME);
			musts(cls, "cannot find java class:" CLASS_NAME);

			jmethodID method = env->GetStaticMethodID(cls, "SetFramesPerSecond", "(F)V");

			musts(method, "cannot find java method: SetFramePerSecond");

			env->CallStaticVoidMethod(cls, method, (jfloat)fps);
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
ZEN_APP_FUNC(OnActivityCreate)(JNIEnv * env, jobject obj);
ZEN_APP_FUNC(OnCreate)(JNIEnv * env, jobject obj);
ZEN_APP_FUNC(OnResize)(JNIEnv * env, jobject obj, jint width, jint height);
ZEN_APP_FUNC(OnStep)(JNIEnv * env, jobject obj);
ZEN_APP_FUNC(OnPause)(JNIEnv * env, jobject obj);
ZEN_APP_FUNC(OnResume)(JNIEnv * env, jobject obj);

ZEN_APP_FUNC(OnMotionDown)(JNIEnv * env, jobject obj,  jint no, jfloat x, jfloat y);
ZEN_APP_FUNC(OnMotionUp)(JNIEnv * env, jobject obj,  jint no, jfloat x, jfloat y);
ZEN_APP_FUNC(OnMotionMove)(JNIEnv * env, jobject obj, jint no, jfloat x, jfloat y);
ZEN_APP_FUNC(OnMotionCancel)(JNIEnv * env, jobject obj,  jint no, jfloat x, jfloat y);
ZEN_APP_FUNC(OnBack)(JNIEnv * env, jobject obj);
}

#define runtime Zen::AppRuntimeAndroid::GetDefault()

extern "C" {
static int mViewWidth = 0;
static int mViewHeight = 0;

static bool mInited = false;
static bool mIsPaused = false;

ZEN_APP_FUNC(OnActivityCreate)(JNIEnv * env, jobject obj)
{
	sMainEnv = env;
	sMainThreadID = std::this_thread::get_id();

	LOG_FUNC();
}

ZEN_APP_FUNC(OnCreate)(JNIEnv * env, jobject obj)
{
	LOG_FUNC();

	sGLEnv = env;
	sGLThreadID = std::this_thread::get_id();

	Zen::Android::SetEnv(env);

	ZenAppMain(0, nullptr);

	mInited = false;

}
ZEN_APP_FUNC(OnResize)(JNIEnv * env, jobject obj,  jint width, jint height)
{
	Zen::LogV("RESIZE %d %d", width, height);

	if(mInited)
	{
		if(mViewWidth != width || mViewHeight != height)
		{
			runtime->resize({(float)width, (float)height});
		}
		else
		{
			return;
		}
	}
	else
	{
		runtime->launch({(float)width, (float)height});
		mInited = true;
	}

	mViewWidth = width;
	mViewHeight = height;
}

ZEN_APP_FUNC(OnStep)(JNIEnv * env, jobject obj)
{
	runtime->update();
	runtime->draw();
}

ZEN_APP_FUNC(OnPause)(JNIEnv * env, jobject obj)
{
	LOG_FUNC();
	runtime->pause();
}

ZEN_APP_FUNC(OnResume)(JNIEnv * env, jobject obj)
{
	LOG_FUNC();
	if(mInited) runtime->resume();
}

static const int MaxTouchCount = 10;
static Zen::AppTouch mTouches[MaxTouchCount];

ZEN_APP_FUNC(OnMotionDown)(JNIEnv * env, jobject obj,  jint no, jfloat x, jfloat y)
{
	if(no >= MaxTouchCount)
	{
		Zen::LogE("too many touch point %d", (int)no);
		return;
	}
	auto & touch = mTouches[no];
	touch.x = x;
	touch.y = y;
	touch.no = no;

	if(runtime->isMultiTouch() || no == 0)
	{
		runtime->touchDown(touch);
	}
	Zen::LogV("%s %f %f (%d)", __FUNCTION__, touch.x, touch.y, touch.no);
}

ZEN_APP_FUNC(OnMotionUp)(JNIEnv * env, jobject obj,  jint no, jfloat x, jfloat y)
{
	if(no >= MaxTouchCount)
	{
		Zen::LogE("too many touch point %d", (int)no);
		return;
	}
	auto & touch = mTouches[no];
	touch.x = x;
	touch.y = y;
	touch.no = no;

	if(runtime->isMultiTouch() || no == 0)
	{
		runtime->touchUp(touch);
	}
	Zen::LogV("%s %f %f (%d)", __FUNCTION__, touch.x, touch.y, touch.no);
}
ZEN_APP_FUNC(OnMotionMove)(JNIEnv * env, jobject obj, jint no, jfloat x, jfloat y)
{
	if(no >= MaxTouchCount)
	{
		return;
	}
	auto & touch = mTouches[no];

	touch.x = x;
	touch.y = y;

	if(runtime->isMultiTouch() || no == 0)
	{
		runtime->touchMove(touch);
	}
}
ZEN_APP_FUNC(OnMotionCancel)(JNIEnv * env, jobject obj,  jint no, jfloat x, jfloat y)
{
	Zen::LogV("%s %d", __FUNCTION__, no);

	if(no >= MaxTouchCount)
	{
		Zen::LogE("too many touch point %d", (int)no);
		return;
	}
	auto & touch = mTouches[no];

	touch.x = x;
	touch.y = y;

	if(runtime->isMultiTouch() || no == 0)
	{
		runtime->touchCancel(touch);
	}
	LOG_FUNC();
}
ZEN_APP_FUNC(OnBack)(JNIEnv * env, jobject obj)
{
	runtime->back();
	LOG_FUNC();
}
}

#endif
