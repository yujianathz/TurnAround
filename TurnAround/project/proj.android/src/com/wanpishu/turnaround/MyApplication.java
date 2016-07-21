package com.wanpishu.turnaround;

import com.umeng.socialize.PlatformConfig;

import android.app.Application;

public class MyApplication extends Application {
	{
		PlatformConfig.setWeixin("wx5e7be6f513325dbc", "7c9de200e1bd8d952348cee91ab5d824");
		PlatformConfig.setSinaWeibo("1399569506", "3a5c46f14f9f3f7c1c81f72fc086bcf7");
		PlatformConfig.setQQZone("1105256878", "MLus0OMbWtDdtVUS");
	}
}
