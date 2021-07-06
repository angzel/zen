package zen.app;

import android.annotation.SuppressLint;
import android.util.Log;

public class AppHelper
{
	static final String TAG = "ZEN-JAVA";

	public static AppActivity s_activity = null;
	public static AppGLSurfaceView s_view = null;
	public static float s_fps = 30.f;

	public static void Initialize(AppActivity _activity, AppGLSurfaceView _view)
	{
		s_activity = _activity;
		s_view = _view;
	}
	public static void RunInGL(Runnable run)
	{
		s_view.queueEvent(run);
	}
	public static void RunInUI(Runnable run)
	{
		s_activity.runOnUiThread(run);
	}
	public static void SetFPS(float fps)
	{
		Log.v(TAG, "reset fps:" + fps);
		if(s_fps == fps) return;
		s_fps = fps;
		if(s_activity != null) s_activity.updateTimer();
	}
}

