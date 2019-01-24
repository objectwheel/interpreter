package com.objectwheel.testemulator;

import android.os.Build;
import android.util.Log;

public final class TestEmulator {
    public static boolean isEmulator() {
        Log.v("####", "LAAAAAAAAAAAAAAAAAAAAN");
        return Build.FINGERPRINT.startsWith("generic")
                || Build.FINGERPRINT.startsWith("unknown")
                || Build.MODEL.contains("google_sdk")
                || Build.MODEL.contains("Emulator")
                || Build.MODEL.contains("Android SDK built for x86")
                || Build.MANUFACTURER.contains("BlueStacks")
                || Build.MANUFACTURER.contains("Genymotion")
                || (Build.BRAND.startsWith("generic") && Build.DEVICE.startsWith("generic"))
                || "google_sdk".equals(Build.PRODUCT);
    }
}
