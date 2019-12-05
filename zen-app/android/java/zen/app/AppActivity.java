package zen.app;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import java.util.Timer;
import java.util.TimerTask;

@SuppressLint({ "NewApi", "ClickableViewAccessibility" })
public class AppActivity extends Activity implements View.OnKeyListener, View.OnTouchListener {
	static final String TAG = "ZEN-ACTIVITY-JAVA";

	@Override
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		Log.v(TAG, "on create *****");
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		// 全屏
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		Log.v(TAG, "Package Name:" + this.getPackageName());
		Log.v(TAG, "Package Files Dir:" + this.getFilesDir().getAbsolutePath());
		Log.v(TAG, "Package Data Dir:" + this.getApplicationInfo().dataDir);

		AppGLSurfaceView view = new AppGLSurfaceView(getApplication(), 0, 0);
		this.setContentView(view);
		view.setOnKeyListener(this);
		view.setOnTouchListener(this);

		zen.system.System.initialize(this);

		Helper.initialize(this, view);

		AppNative.OnActivityCreate();
	}

	@Override
	protected void onPause() {
		super.onPause();
		Log.v(TAG, "on pause *****");
		this.stopTimer();
		Helper.runInGL(new Runnable() {
			@Override
			public void run() {
				AppNative.OnPause();
			}
		});
	}

	@Override
	protected void onResume() {
		super.onResume();
		Log.v(TAG, "on resume *****");
		this.startTimer();
		Helper.runInGL(new Runnable() {
			@Override
			public void run() {
				AppNative.OnResume();
			}
		});
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		Log.v(TAG, "onDestroy ****");
		this.stopTimer();
		System.exit(0);
	}

	/**
	 * Runnable for Touch Action.
	 */
	public static class TouchRunner implements Runnable {
		public float x;
		public float y;
		public int mask;
		public int no;

		TouchRunner() {
		}

		@Override
		public void run() {
			switch (mask) {
			case MotionEvent.ACTION_DOWN:
			case MotionEvent.ACTION_POINTER_DOWN:
				AppNative.OnMotionDown(no, x, y);
				break;
			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_POINTER_UP:
				AppNative.OnMotionUp(no, x, y);
				break;
			case MotionEvent.ACTION_CANCEL:
				AppNative.OnMotionCancel(no, x, y);
				break;
			case MotionEvent.ACTION_MOVE:
				AppNative.OnMotionMove(no, x, y);
				break;
			default:
			}
		}
	}

	@Override
	public boolean onTouch(View v, MotionEvent event) {
		int index = event.getActionIndex();
		int no = event.getPointerId(index);
		float x = event.getX(index);
		float y = event.getY(index);

		TouchRunner tr = new TouchRunner();
		tr.x = x;
		tr.y = y;
		tr.no = no;
		tr.mask = event.getActionMasked();
		Helper.runInGL(tr);
		return true;
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			Helper.runInGL(new Runnable() {

				@Override
				public void run() {
					// TODO Auto-generated method stub
					AppNative.OnBack();
				}

			});
			return true;
		}
		return false;
	}

	@Override
	public boolean onKey(View v, int keyCode, KeyEvent event) {
		Log.v(TAG, String.format("OnKey code:%d event:%s", keyCode, event.toString()));
		return false;
	}

	Timer mTimer = null;

	void stopTimer()
	{
		if(mTimer == null) return;
		mTimer.cancel();
		mTimer = null;
	}
	void startTimer()
	{
		if(mTimer != null) mTimer.cancel();
		mTimer = new Timer();
		TimerTask task = new TimerTask() {
			@Override
			public void run() {
				Helper.S_view.requestRender();
			}
		};
		mTimer.schedule(task, 0, 1000/Helper.S_frames_per_second);
		Log.v(TAG, "timer period:" + (1000/Helper.S_frames_per_second));
	}
	void updateTimer()
	{
		if(mTimer != null) startTimer();
	}
}
