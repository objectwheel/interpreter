package com.objectwheel.interpreter;
import android.os.Bundle;
import android.app.Activity;
import java.lang.reflect.Method;
import org.qtproject.qt5.android.bindings.QtActivity;
import org.qtproject.qt5.android.QtNative;

public class InterpreterActivity extends QtActivity {

    @Override
    public void onCreate(Bundle bundle)
    {
        setTheme(getResources().getIdentifier("NormalTheme", "style", getPackageName()));
        super.onCreate(bundle);
        setFullScreen(true);
    }

    private void setFullScreen(final boolean fullScreen)
    {
        try {
            Method m = QtNative.class.getDeclaredMethod("setFullScreen", Boolean.TYPE);
            m.setAccessible(true);
            m.invoke(null, fullScreen);
        } catch (Exception e) { }
    }
}
