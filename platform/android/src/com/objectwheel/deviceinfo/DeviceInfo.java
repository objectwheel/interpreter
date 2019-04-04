package com.objectwheel.deviceinfo;

import android.os.Build;

public final class DeviceInfo {
    public static boolean isEmulator() {
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

    private static String capitalize(String s) {
        if (s == null || s.length() == 0)
            return "";
        char first = s.charAt(0);
        if (Character.isUpperCase(first))
            return s;
        else
            return Character.toUpperCase(first) + s.substring(1);
    }

    public static String getDeviceName() {
        String manufacturer = Build.MANUFACTURER;
        if (manufacturer.toLowerCase().contains("unknown") && DeviceInfo.isEmulator())
            manufacturer = "Google";

        String model = Build.MODEL;
        if (model.toLowerCase().contains("google_sdk")
            || model.toLowerCase().contains("emulator")
            || model.toLowerCase().contains("android sdk built for x86"))
            model = "Android";

        if (manufacturer.isEmpty() || model.toLowerCase().startsWith(manufacturer.toLowerCase()))
            return capitalize(model);
        else
            return capitalize(manufacturer) + " " + model;
    }
}
