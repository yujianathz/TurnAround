LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := turnaround_shared

LOCAL_MODULE_FILENAME := libturnaround

ifeq ($(USE_ARM_MODE),1)
LOCAL_ARM_MODE := arm
endif

LOCAL_SRC_FILES := main.cpp 																\
									 ../../Classes/AppDelegate.cpp						\
									 ../../Classes/AppUpdateScene.cpp					\
									 ../../Classes/AppUtils.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes									\
   									$(LOCAL_PATH)/../../../../GameBox/Classes				\
										$(LOCAL_PATH)/../../../../GameBox/Classes/UI		\
										$(LOCAL_PATH)/../../../../GameBox/Classes/Data		\
										$(LOCAL_PATH)/../../../../GameBox/Classes/Script/lua-bindings			\
										$(LOCAL_PATH)/../../../../Cocos2dx			\
										$(LOCAL_PATH)/../../../../Cocos2dx/cocos				\
										$(LOCAL_PATH)/../../../../Cocos2dx/cocos/ui			\
										$(LOCAL_PATH)/../../../../Cocos2dx/cocos/network			\
										$(LOCAL_PATH)/../../../../Cocos2dx/cocos/scripting/lua-bindings/auto			\
										$(LOCAL_PATH)/../../../../Cocos2dx/cocos/scripting/lua-bindings/manual			\
										$(LOCAL_PATH)/../../../../Cocos2dx/cocos/audio/include  \
										$(LOCAL_PATH)/../../../../Cocos2dx/cocos/platform								\
										$(LOCAL_PATH)/../../../../Cocos2dx/cocos/platform/android/jni		\
										$(LOCAL_PATH)/../../../../Cocos2dx/external/lua/lua		\
										$(LOCAL_PATH)/../../../../Cocos2dx/external/lua/tolua		\
										$(LOCAL_PATH)/../../../../Cocos2dx/external/lua/cjson
										

LOCAL_STATIC_LIBRARIES := gamebox_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,../../GameBox/proj.android/jni)
