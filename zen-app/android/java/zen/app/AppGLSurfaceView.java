package zen.app;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.Log;

import java.util.Timer;
import java.util.TimerTask;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

@SuppressLint({
		"NewApi", "ViewConstructor"
})
class AppGLSurfaceView extends GLSurfaceView
{
	static final String TAG = "ZEN-GLView-JAVA";

	Renderer mRenderer;

	public AppGLSurfaceView(Context context, int depth, int stencil)
	{
		super(context);

		init(depth, stencil);

		setFocusable(true);
		setFocusableInTouchMode(true);
		requestFocus();

		setRenderMode(RENDERMODE_WHEN_DIRTY);

		Log.v(TAG, "created");
	}

	static public double getNowMS()
	{
		return (double)System.nanoTime() / 1000000;
	}

	private void init(int depth, int stencil) {

		this.getHolder().setFormat(PixelFormat.TRANSLUCENT);

		setEGLContextFactory(new ContextFactory());

		setEGLConfigChooser(new ConfigChooser(depth, stencil));

		setRenderer(mRenderer = new Renderer());

		Log.v(TAG, "inited");
	}

	/**
	 * ConfigChooser
	 */
	private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser
	{

		public ConfigChooser(int depth, int stencil)
		{
			mDepthSize = depth;
			mStencilSize = stencil;
		}

		private static int EGL_OPENGL_ES2_BIT = 4;
		private static int[] configAttribs2 = {
				EGL10.EGL_RED_SIZE, 8,
				EGL10.EGL_GREEN_SIZE, 8,
				EGL10.EGL_BLUE_SIZE, 8,
				EGL10.EGL_ALPHA_SIZE, 8,
				EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
				EGL10.EGL_NONE
		};

		@Override
		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display)
		{

			/*
			 * Get the number of minimally matching EGL configurations
			 */
			int[] num_config = new int[1];
			egl.eglChooseConfig(display, configAttribs2, null, 0, num_config);

			int numConfigs = num_config[0];

			if (numConfigs <= 0)
			{
				throw new IllegalArgumentException("No configs match configSpec");
			}

			/*
			 * Allocate then read the array of minimally matching EGL configs
			 */
			EGLConfig[] configs = new EGLConfig[numConfigs];
			egl.eglChooseConfig(display, configAttribs2, configs, numConfigs, num_config);

			/*
			 * Now return the "best" one
			 */
			return chooseConfig(egl, display, configs);
		}

		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs)
		{
			EGLConfig chosen = null;
			int alpha = 0;
			for (EGLConfig config : configs)
			{
				int d = findConfigAttrib(egl, display, config, EGL10.EGL_DEPTH_SIZE, 0);
				int s = findConfigAttrib(egl, display, config, EGL10.EGL_STENCIL_SIZE, 0);

				// We need at least mDepthSize and mStencilSize bits
				if (d < mDepthSize || s < mStencilSize) continue;

				// We want an *exact* match for red/green/blue/alpha
				int a = findConfigAttrib(egl, display, config, EGL10.EGL_ALPHA_SIZE, 0);

				if(chosen == null || a > alpha)
				{
					alpha = a;
					chosen = config;
				}
			}
			return chosen;
		}

		private int findConfigAttrib(EGL10 egl, EGLDisplay display, EGLConfig config, int attribute, int defaultValue)
		{

			if (egl.eglGetConfigAttrib(display, config, attribute, mValue))
			{
				return mValue[0];
			}
			return defaultValue;
		}

		// Subclasses can adjust these values:
		protected int mDepthSize;
		protected int mStencilSize;
		private int[] mValue = new int[1];
	}

	/**
	 * Renderer
	 */
	private static class Renderer implements GLSurfaceView.Renderer
	{
		Renderer()
		{
			super();
		}
		public void onDrawFrame(GL10 gl)
		{
			AppNative.OnStep();
		}

		public void onSurfaceChanged(GL10 gl, int width, int height)
		{
			Log.v(TAG, "onSurfaceChanged");
			AppNative.OnResize(width, height);
		}

		public void onSurfaceCreated(GL10 gl, EGLConfig config)
		{
			Log.v(TAG, "onSurfaceCreated");
			AppNative.OnCreate();
		}
	}
	/**
	 *
	 * @author ClearSky
	 *
	 *	inner class ContextFactory
	 *
	 */

	private static class ContextFactory implements GLSurfaceView.EGLContextFactory
	{
		private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

		EGLContext mContext = null;

		public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig)
		{
			if(mContext != null)
			{
				Log.v(TAG, "Old EGLContext found, use it!");
				return mContext;
			}

			Log.v(TAG, "ContextFactory creating OpenGL ES 2.0 context");

			checkEglError("Before eglCreateContext", egl);

			int[] attrib_list = {
					EGL_CONTEXT_CLIENT_VERSION, 2,
					EGL10.EGL_NONE
			};
			EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);

			checkEglError("After eglCreateContext", egl);

			mContext = context;
			return context;
		}

		public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context)
		{
			Log.v(TAG, "Destroy Context");
			egl.eglDestroyContext(display, context);
			mContext = null;
		}

		public static void checkEglError(String prompt, EGL10 egl)
		{
			int error = 0;
			while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS)
			{
				Log.e(TAG, String.format("%s: EGL error: 0x%x", prompt, error));
			}
		}

	}
	public void onPause()
	{
		Log.v(TAG, "**** GL PAUSE");
	}
}
