package com.wanpishu.gamebox;

import org.OpenUDID.OpenUDID_manager;
import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.app.Service;  
import android.os.Vibrator;
import android.provider.Settings;
import android.telephony.TelephonyManager;
import android.view.WindowManager;

public abstract class GBActivity extends Cocos2dxActivity {
	static Context m_Context = null;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
		m_Context = this;
	}
	
	public abstract void pay(final String payInfo);
	
	public static String getDeviceId() {		
		return Settings.Secure.getString(m_Context.getContentResolver(),Settings.Secure.ANDROID_ID);	
	}
	
	public static String getUmChannelId() {
		String channelId = "wanpishu";
		try {
			ApplicationInfo appInfo = m_Context.getPackageManager().getApplicationInfo(m_Context.getPackageName(), PackageManager.GET_META_DATA);
			channelId = appInfo.metaData.getString("UMENG_CHANNEL");	
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return channelId;		
	}
	
	public static String getChannelId() {
		String channelId = "wanpishu";
		try {
			ApplicationInfo appInfo = m_Context.getPackageManager().getApplicationInfo(m_Context.getPackageName(), PackageManager.GET_META_DATA);
			channelId = appInfo.metaData.getString("ChannelId");
			if (channelId == null)
				channelId = getUmChannelId();
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			
			channelId = getUmChannelId();
		}
		
		return channelId;
	}
	
	public static String getImsi() {
		TelephonyManager mTelephonyMgr = (TelephonyManager)m_Context.getSystemService(Context.TELEPHONY_SERVICE);
		return mTelephonyMgr.getSubscriberId();
	}
	
	public static void vibration(float duration) {
		Vibrator vib = (Vibrator)m_Context.getSystemService(Service.VIBRATOR_SERVICE);   
		vib.vibrate((int)(duration * 1000));	
	}
	
	public static void purchase(final String payInfo) {
		((GBActivity)m_Context).pay(payInfo);
	}
	
	public static String getOpenUDID() {
		if (OpenUDID_manager.isInitialized())
			return OpenUDID_manager.getOpenUDID();
		return null;
	}
}
