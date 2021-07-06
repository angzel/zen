package zen.app;

public class AppNative {
    /*
     * These below should called in main thread.
     */
	public static native void OnActivityCreate();
	
	/*
	 * These below should be called in GL thread.
	 */
	public static native void OnCreate();
	public static native void OnResize(int width, int height);

	public static native void OnStep();

	public static native void OnResume();
	public static native void OnPause();

	public static native void OnMotionDown(int tid, float x, float y);
	public static native void OnMotionMove(int tid, float x, float y);
	public static native void OnMotionUp(int tid, float x, float y);
	public static native void OnMotionCancel(int tid, float x, float y);
	public static native void OnBack();

	public static void SetFramesPerSecond(final float fps)
	{
		AppHelper.RunInUI(new Runnable() {
			@Override
			public void run() {
				AppHelper.SetFPS(fps);
			}
		});
	}
}
