package zen.platform;

import java.io.InputStream;
import java.net.URL;
import java.util.Locale;
import java.util.Vector;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.res.AssetManager;
import android.os.Build;
import android.os.LocaleList;
import android.util.Log;

@SuppressLint("NewApi")
public class Utils {
    static final String TAG = "ZEN-LOG:Java_Utils";

    static Activity activity = null;

    static native void initializeNative();

    // initialize when your Main Activity onLaunch.
    public static void initialize(Activity _activity) {
        activity = _activity;
        initializeNative();
    }

    public static String getAppID() {
        return activity.getPackageName();
    }

    public static String getDeviceID() {
        String uid = "35" + //we make this look like a valid IMEI
                Build.BOARD.length() % 10 + Build.BRAND.length() % 10 +
                Build.CPU_ABI.length() % 10 + Build.DEVICE.length() % 10 +
                Build.DISPLAY.length() % 10 + Build.HOST.length() % 10 +
                Build.ID.length() % 10 + Build.MANUFACTURER.length() % 10 +
                Build.MODEL.length() % 10 + Build.PRODUCT.length() % 10 +
                Build.TAGS.length() % 10 + Build.TYPE.length() % 10 +
                Build.USER.length() % 10; //13 digits
        return uid;
    }

    public static String getDataDir() {
        ApplicationInfo info = activity.getApplicationInfo();
        return info.dataDir;
    }

    public static byte[] loadResource(String file_name) {

        AssetManager am = activity.getApplicationContext().getAssets();
        Vector<Byte> data = new Vector<Byte>();
        try {
            InputStream stream = am.open(file_name);
            if (stream == null) {
                Log.v(TAG, "open file error");
                return null;
            }
            int once = 1024;
            byte[] buf = new byte[once];

            int off = 0;
            while (true) {
                int len = stream.read(buf, off, once);
                for (int i = 0; i < len; ++i) {
                    data.add(buf[i]);
                }
                if (len < once) {
                    break;
                }
            }
        } catch (Exception e) {
            Log.v(TAG, e.toString());
        }
        if (data.size() == 0)
            return null;

        byte[] res = new byte[data.size()];
        for (int i = 0; i < res.length; ++i)
            res[i] = data.get(i);
        return res;
    }

    public static boolean isResourceExist(String file_name) {
        AssetManager am = activity.getApplicationContext().getAssets();
        try {
            am.open(file_name);
            return true;
        } catch (Exception e) {
            Log.v(TAG, e.toString());
            return false;
        }
    }

    public static byte[] loadURL(String url) {
        Vector<Byte> data = new Vector<Byte>();
        try {
            URL connect = new URL(url);
            InputStream stream = connect.openStream();

            if (stream == null) {
                Log.v(TAG, "open file error");
                return null;
            }
            int once = 1024*32;
            byte[] buf = new byte[once];

            int off = 0;
            while (true) {
                int len = stream.read(buf, off, once);
                for (int i = 0; i < len; ++i)
                    data.add(buf[i]);
                if (len < once)
                    break;
            }
        } catch (Exception e) {
            Log.v(TAG, e.toString());
        }
        if (data.size() == 0) return null;

        byte[] res = new byte[data.size()];
        for (int i = 0; i < res.length; ++i)
            res[i] = data.get(i);
        return res;
    }

    public static String getSystemLanguage() {
        Locale locale;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            locale = LocaleList.getDefault().get(0);
        } else {
            locale = Locale.getDefault();
        }
        String language = locale.getLanguage() + "-" + locale.getCountry();
        Log.v(TAG, language);
        return language;
    }
}
