LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := gamebox_static
LOCAL_MODULE_FILENAME := libgamebox

LOCAL_SRC_FILES := ../../Classes/GBDelegate.cpp 	\
									 ../../Classes/GBUtils.cpp      \
									 ../../Classes/GBShaderEffect.cpp		\
									 ../../Classes/Data/GBCsv.cpp				\
									 ../../Classes/Data/GBLang.cpp				\
									 ../../Classes/Data/GBSqlite.cpp			\
									 ../../Classes/Data/GBValue.cpp	\
									 ../../Classes/MD5/md5.cpp	\
									 ../../Classes/Platform/android/GBUtils_android.cpp	\
									 ../../Classes/UI/GBButton.cpp	\
									 ../../Classes/UI/GBList.cpp		\
									 ../../Classes/UI/GBPanel.cpp		\
									 ../../Classes/UI/GBForm.cpp			\
									 ../../Classes/UI/GBNumberLabel.cpp	\
									 ../../Classes/UI/GBShaderButton.cpp	\
									 ../../Classes/Network/GBSocketBase.cpp	\
									 ../../Classes/Network/GBSocketClient.cpp
									 
ifeq ($(CC_ENABLE_SCRIPT_BINDING),1)
LOCAL_SRC_FILES += ../../Classes/Script/lua-bindings/lua_gamebox.cpp
LOCAL_SRC_FILES += ../../Classes/Data/GBUpdate.cpp
endif									 
									 
ifdef APP_USE_ANYSDK
LOCAL_SRC_FILES += ../../Classes/Platform/android/GBAnySDK_android.cpp
endif									 							 						 																 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes								\
										$(LOCAL_PATH)/../../Classes/UI						\
										$(LOCAL_PATH)/../../Classes/Data					\
										$(LOCAL_PATH)/../../Classes/Platform			\
										$(LOCAL_PATH)/../../Classes/Network				\
										$(LOCAL_PATH)/../../../Cocos2dx			\
										$(LOCAL_PATH)/../../../Cocos2dx/cocos			\
										$(LOCAL_PATH)/../../../Cocos2dx/cocos/ui  \
										$(LOCAL_PATH)/../../../Cocos2dx/cocos/network  \
										$(LOCAL_PATH)/../../../Cocos2dx/cocos/audio/include \
										$(LOCAL_PATH)/../../../Cocos2dx/cocos/storage/local-storage  \
										$(LOCAL_PATH)/../../../Cocos2dx/cocos/platform
										
ifeq ($(CC_ENABLE_SCRIPT_BINDING),1)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Cocos2dx/cocos/scripting/lua-bindings/manual
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Cocos2dx/external/lua
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Cocos2dx/external/lua/tolua
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Cocos2dx/external/lua/luajit/include
endif										
										
ifdef APP_USE_ANYSDK
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Cocos2dx/external/anysdk/include
LOCAL_C_INCLUDES +=	$(LOCAL_PATH)/../../../Cocos2dx/external/anysdk/android
endif										

ifdef APP_USE_ANYSDK
LOCAL_WHOLE_STATIC_LIBRARIES += PluginProtocolStatic
endif

ifeq ($(CC_ENABLE_SCRIPT_BINDING),1)
LOCAL_STATIC_LIBRARIES += cocos2d_lua_static
else
LOCAL_STATIC_LIBRARIES += cocos_ui_static
endif
LOCAL_STATIC_LIBRARIES += audioengine_static
LOCAL_STATIC_LIBRARIES += cocos_localstorage_static
						
include $(BUILD_STATIC_LIBRARY)

ifeq ($(CC_ENABLE_SCRIPT_BINDING),1)
$(call import-module,scripting/lua-bindings/proj.android)
else
$(call import-module,cocos)
$(call import-module,ui)
endif
$(call import-module,audio/android)
$(call import-module,storage/local-storage)

ifdef APP_USE_ANYSDK
$(call import-module,external/anysdk/android)
endif