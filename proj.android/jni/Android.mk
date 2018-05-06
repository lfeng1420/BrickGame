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
					../../Classes/MyListView.cpp \
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
					../../Classes/VolumeLayer.cpp \
					../../Classes/VolumeLayerEx.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../cocos2d/cocos/ui 
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../cocos2d/extensions

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
 LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
 LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
 $(call import-module,editor-support/cocostudio)
# $(call import-module,network)
 $(call import-module,extensions)
