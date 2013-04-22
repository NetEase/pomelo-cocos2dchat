LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/ChatScene.cpp \
                   ../../Classes/LoginScene.cpp \
                   ../../Classes/UserScene.cpp \
                   ../../Classes/VisibleRect.cpp \
                   ../../Classes/CustomTableViewCell.cpp
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes                   

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static pomelo_static

COMMON_GLOBAL_CFLAGS:= -D__ANDROID__

include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,libpomelo) \
$(call import-module,extensions)
