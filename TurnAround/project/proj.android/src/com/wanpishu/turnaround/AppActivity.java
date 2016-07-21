/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.wanpishu.turnaround;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.os.Bundle;
import android.provider.Settings;
import android.widget.Toast;

import com.wanpishu.gamebox.GBActivity;

import java.io.ByteArrayOutputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import org.OpenUDID.OpenUDID_manager;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;
import com.umeng.analytics.MobclickAgent;
import com.umeng.analytics.game.UMGameAgent;
import com.umeng.socialize.ShareAction;
import com.umeng.socialize.UMShareAPI;
import com.umeng.socialize.UMShareListener;
import com.umeng.socialize.bean.SHARE_MEDIA;
import com.umeng.socialize.media.UMImage;
import com.umeng.socialize.shareboard.SnsPlatform;
import com.umeng.socialize.utils.ShareBoardlistener;
import com.vungle.publisher.VunglePub;

public class AppActivity extends GBActivity {

	private static AppActivity mAppActivity;
	private static VunglePub vunglePub = VunglePub.getInstance();
	private static InterstitialAd mInterstitialAd1;
	private static InterstitialAd mInterstitialAd2;
	private static UMShareAPI mShareAPI;
	private static ShareAction mShareAction;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		mAppActivity = this;
		
		OpenUDID_manager.sync(this);
		
		UMGameAgent.setDebugMode(false);
		UMGameAgent.init(this);
		
		vunglePub.init(this, "56e286311563c6040b000008");
		
		mInterstitialAd1 = new InterstitialAd(this);
		mInterstitialAd1.setAdUnitId("ca-app-pub-1028707621106321/6261860498");
		mInterstitialAd1.setAdListener(new AdListener(){

			@Override
			public void onAdClosed() {
				// TODO Auto-generated method stub
				AppActivity.this.requestNewInterstitial(mInterstitialAd1);
			}			
		});
		requestNewInterstitial(mInterstitialAd1);
		
		mInterstitialAd2 = new InterstitialAd(this);
		mInterstitialAd2.setAdUnitId("ca-app-pub-1028707621106321/7738593694");
		mInterstitialAd2.setAdListener(new AdListener(){

			@Override
			public void onAdClosed() {
				// TODO Auto-generated method stub
				AppActivity.this.requestNewInterstitial(mInterstitialAd2);
			}			
		});
		requestNewInterstitial(mInterstitialAd2);
		
		mShareAPI = UMShareAPI.get(this);		
		mShareAction = new ShareAction(this);
	}
	
	@SuppressLint("DefaultLocale")
	private void requestNewInterstitial(InterstitialAd ad) {
		String android_id = Settings.Secure.getString(getContentResolver(), Settings.Secure.ANDROID_ID);
		String device_id = MD5(android_id).toUpperCase();
		AdRequest adRequest = new AdRequest.Builder().build();
		
		ad.loadAd(adRequest);
	}
	
	private static String MD5(String s) {
		 try {
		        // Create MD5 Hash
		        MessageDigest digest = java.security.MessageDigest
		                .getInstance("MD5");
		        digest.update(s.getBytes());
		        byte messageDigest[] = digest.digest();

		        // Create Hex String
		        StringBuffer hexString = new StringBuffer();
		        for (int i = 0; i < messageDigest.length; i++) {
		            String h = Integer.toHexString(0xFF & messageDigest[i]);
		            while (h.length() < 2)
		                h = "0" + h;
		            hexString.append(h);
		        }
		        return hexString.toString();
		 } catch (NoSuchAlgorithmException e) {
			 e.printStackTrace();
		 }
		 return "";		
	}

	@Override
	public void onResume() {
		// TODO Auto-generated method stub
		super.onResume();	
		UMGameAgent.onResume(this);
		vunglePub.onResume();		
	}

	@Override
	public void onPause() {
		// TODO Auto-generated method stub
		super.onPause();		
		UMGameAgent.onPause(this);
		vunglePub.onPause();		
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		vunglePub.clearEventListeners();
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		mShareAPI.onActivityResult(requestCode, resultCode, data);
	}

	@Override
	public void pay(final String payInfo) {
		// TODO Auto-generated method stub
		runOnUiThread(new Runnable()
		{
			@Override
			public void run() {
				// TODO Auto-generated method stub
				purchaseSuccess(payInfo);		
			}			
		});	
	}
	
	public void purchaseSuccess(final String payInfo){
		AppActivity.this.runOnGLThread(new Runnable(){
			@Override
			public void run() {
				// TODO Auto-generated method stub
				AppActivity.this.paySuccess(payInfo);
			}					
		});	
	}
	
	public void purchaseFail(final String payInfo){
		AppActivity.this.runOnGLThread(new Runnable(){
			@Override
			public void run() {
				// TODO Auto-generated method stub
				AppActivity.this.payFail(payInfo);
			}					
		});		
	}
	
	public void purchaseCancel(final String payInfo){
		AppActivity.this.runOnGLThread(new Runnable(){
			@Override
			public void run() {
				// TODO Auto-generated method stub
				AppActivity.this.payCancel(payInfo);
			}					
		});
	}
	
	public static void umengStartLevel(String level){
		UMGameAgent.startLevel(level);
	}
	
	public static void umengFailLevel(String level){
		UMGameAgent.failLevel(level);
	}
	
	public static void umengFinishLevel(String level){
		UMGameAgent.finishLevel(level);
	}
	
	public static void umengPay(int productPrice, String productName){
		UMGameAgent.pay(productPrice, productName, 1, 0, 99);
	}
	
	public static void umengEvent(String event, String param){
		if (param.isEmpty()){
			MobclickAgent.onEvent(mAppActivity, event);
		}
		else{
			MobclickAgent.onEvent(mAppActivity, event, param);
		}
	}
	
	public static void umengUse(String productName, int productNum){
		UMGameAgent.use(productName, productNum, 0);
	}
	
	public static boolean isVideoPlayable(){
		return vunglePub.isAdPlayable();
	}
	
	public static void playVideo(){
		vunglePub.playAd();			
	}
	
	public static void showAd1(){
		mAppActivity.runOnUiThread(new Runnable(){

			@Override
			public void run() {
				// TODO Auto-generated method stub
				if (mInterstitialAd1.isLoaded())
					mInterstitialAd1.show();
			}
			
		});
	}
	
	public static void showAd2(){
		mAppActivity.runOnUiThread(new Runnable(){

			@Override
			public void run() {
				// TODO Auto-generated method stub
				if (mInterstitialAd2.isLoaded())
					mInterstitialAd2.show();
			}
			
		});
	}
	
	private static ShareBoardlistener shareBoardListener = new ShareBoardlistener(){

		@Override
		public void onclick(SnsPlatform arg0, SHARE_MEDIA arg1) {
			// TODO Auto-generated method stub	
            mShareAction.setPlatform(arg1).setCallback(umShareListener).share();
		}
		
	};
	
	private static UMShareListener umShareListener = new UMShareListener(){

		@Override
		public void onCancel(SHARE_MEDIA arg0) {
			// TODO Auto-generated method stub
			Toast.makeText(mAppActivity, arg0 + " 分享成功啦", Toast.LENGTH_SHORT).show();
		}

		@Override
		public void onError(SHARE_MEDIA arg0, Throwable arg1) {
			// TODO Auto-generated method stub
			Toast.makeText(mAppActivity, arg0 + " 分享失败啦" + arg1.getMessage(), Toast.LENGTH_SHORT).show();
		}

		@Override
		public void onResult(SHARE_MEDIA arg0) {
			// TODO Auto-generated method stub
			Toast.makeText(mAppActivity, arg0 + " 分享成功啦", Toast.LENGTH_SHORT).show();
		}
		
	};
	
	public static void openShare(final String shareText, final String shareImage, final String shareTitle, final String shareUrl){		
		mAppActivity.runOnUiThread(new Runnable(){

			@Override
			public void run() {
				// TODO Auto-generated method stub
				UMImage image = new UMImage(mAppActivity, AppActivity.compressImage(shareImage));		
				final SHARE_MEDIA[] displaylist = new SHARE_MEDIA[]
                        {
                            SHARE_MEDIA.WEIXIN, SHARE_MEDIA.WEIXIN_CIRCLE,SHARE_MEDIA.SINA,
                            SHARE_MEDIA.QQ, SHARE_MEDIA.QZONE
                        };
                mShareAction.setDisplayList(displaylist)
                        .withText(shareText)
                        .withTitle(shareTitle)
                        .withTargetUrl(shareUrl)
                        .withMedia(image)                        
                        .setShareboardclickCallback(shareBoardListener)
                        .open();
			}

		});
	}
	
	public static Bitmap compressImage(String imagePath){
		double maxSize = 32;
		
		Bitmap bm = BitmapFactory.decodeFile(imagePath);
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		bm.compress(Bitmap.CompressFormat.JPEG, 100, baos);
		byte[] b = baos.toByteArray();
		double mid = b.length / 1024;
		if (mid > maxSize){
			double i = mid / maxSize;
			bm = zoomImage(bm, bm.getWidth() / Math.sqrt(i), bm.getHeight() / Math.sqrt(i));
		}
		
		return bm;
	}
	
	public static Bitmap zoomImage(Bitmap image, double newWidth, double newHeight){
        float width = image.getWidth();
        float height = image.getHeight();
        Matrix matrix = new Matrix();
        float scaleWidth = ((float) newWidth) / width;
        float scaleHeight = ((float) newHeight) / height;
        matrix.postScale(scaleWidth, scaleHeight);
        Bitmap bitmap = Bitmap.createBitmap(image, 0, 0, (int) width,
                        (int) height, matrix, true);
        return bitmap;		
	}
	
	public native void paySuccess(String payInfo);
	public native void payFail(String payInfo);
	public native void payCancel(String payInfo);
}
