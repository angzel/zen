package zen.app;

import android.annotation.SuppressLint;
import android.util.Log;

public class Helper
{
	static final String TAG = "ZEN-JAVA";

	public static AppActivity S_activity = null;
	public static AppGLSurfaceView S_view = null;
	public static int S_frames_per_second = 30;

	public static void initialize(AppActivity _activity, AppGLSurfaceView _view)
	{
		S_activity = _activity;
		S_view = _view;
	}
	public static void runInGL(Runnable run)
	{
		S_view.queueEvent(run);
	}
	public static void runInUI(Runnable run)
	{
		S_activity.runOnUiThread(run);
	}
	public static void resetFramesPerSecond(int fps)
	{
		Log.v(TAG, "reset fps:" + fps);
		if(S_frames_per_second == fps) return;
		S_frames_per_second = fps;
		if(S_activity != null) S_activity.updateTimer();
	}
}

