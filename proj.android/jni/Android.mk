LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := lfeng

LOCAL_MODULE_FILENAME := liblfeng

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/AddBrickGame.cpp \
					../../Classes/BarrierLayer.cpp \
					../../Classes/DataManager.cpp \
					../../Classes/EventEngine.cpp \
					../../Classes/FlappybirdGame.cpp \
					../../Classes/FroggerGame.cpp \
					../../Classes/GameBase.cpp \
					../../Classes/GameLogic.cpp \
					../../Classes/GameManager.cpp \
					../../Classes/GameScene.cpp \
					../../Classes/GameSceneEx.cpp \
					../../Classes/HitBrickGame.cpp \
					../../Classes/LoadScene.cpp \
					../../Classes/MatchGame.cpp \
					../../Classes/OverGame.cpp \
					../../Classes/PinballGame.cpp \
					../../Classes/PinballGameEx.cpp \
					../../Classes/RaceGame.cpp \
					../../Classes/SelectGame.cpp \
					../../Classes/SetupScene.cpp \
					../../Classes/SetupSceneEx.cpp \
					../../Classes/SnakeGame.cpp \
					../../Classes/stdafx.cpp \
					../../Classes/TankGame.cpp \
					../../Classes/TetrisGame.cpp \
					../../Classes/TetrisGameEx.cpp \
					../../Classes/TimerManager.cpp \
					../../Classes/SetupVolume.cpp \
					../../Classes/SetupVolumeEx.cpp \
					../../Classes/SetupControlBtn.cpp \
                    ../../Classes/SetupControlBtnEx.cpp


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../cocos2d/extensions

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,extensions)
