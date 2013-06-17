/*
 *  Copyright 2013 ARM Limited
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of ARM Limited nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY ARM LIMITED AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
package com.arm.ne10.demo;


import com.arm.ne10.demo.R;

import android.os.Bundle;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.webkit.JavascriptInterface;
import android.webkit.WebSettings;
import android.webkit.WebView;

/* suppress warning of setJavaScriptEnabled */
@SuppressLint("SetJavaScriptEnabled")
public class MainActivity extends Activity {

    public class MyJavaInterface {
        MainActivity mParent;
        MyJavaInterface(MainActivity parent) {
            mParent = parent;
        }
        @JavascriptInterface
        public String NE10RunTest() {
            return mParent.NE10RunTest();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        WebView myWebView = (WebView) findViewById(R.id.webview);
        WebSettings webSettings = myWebView.getSettings();
        webSettings.setJavaScriptEnabled(true);

        myWebView.addJavascriptInterface(new MyJavaInterface(this), "MyJavaClass");

        myWebView.loadUrl("file:///android_asset/demo.html");

    }

    /* A native method that is implemented by native library, which is packaged
     * with this application.
     */
    public native String NE10RunTest();

    /* this is used to load the native library on application
     * startup. The library has already been unpacked into
     * /data/data/com.example.demo/lib/libNE10_test_demo.so at
     * installation time by the package manager.
     * it's under libs/armeabi/ directory under project dir
     */
    static {
        System.loadLibrary("NE10_test_demo");
    }
}
