#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <list>
#include <string>
#include <cstdlib>
using namespace std;

#include "DCommon.h"
#include "DEvent.h"
#include "DGlobalMacro.h"
#include "DTypeDef.h"

// Event engine
#include "IEventHandler.h"
#include "EventEngine.h"

// cocos
#include "cocos2d.h"
USING_NS_CC;
#include "cocos-ext.h"
USING_NS_CC_EXT;

// json
#include "external/json/rapidjson.h"
#include "external/json/writer.h"
#include "external/json/document.h"
#include "external/json/stringbuffer.h"

// Global func
#include "GlobalFunc.h"


// Boom position offset
#define BOOM_ROW_COUNT			4
#define BOOM_COLUMN_COUNT		4
extern const bool BOOM_SHAPE[BOOM_ROW_COUNT][BOOM_COLUMN_COUNT];

// Oppsite direction
extern const int OPPISITE_DIR[DIR_MAX];

// Button id to direction
extern const int BTNID_2_DIR[BTNID_DIRMAX];

// Tetris shape
#define TETRIS_SHAPE_COUNT			23
#define TETRIS_NORMAL_SHAPE_COUNT	19
extern const bool TETRIS_SHAPE[TETRIS_SHAPE_COUNT][SMALL_BRICK_ROW_COUNT][SMALL_BRICK_ROW_COUNT];
extern const int TETRIS_SHAPE_ROW_COUNT[TETRIS_SHAPE_COUNT];
extern const int TETRIS_SHAPE_COL_COUNT[TETRIS_SHAPE_COUNT];
extern const int TETRIS_TYPE_START_IDX[TETRIS_SHAPE_COUNT];

// Default shape
extern const bool DEFAULT_SHAPE[ROW_COUNT * COLUMN_COUNT];

// Sound string
extern const char* BGM_START;
extern const char* EFFECT_ADD;
extern const char* EFFECT_BOOM;
extern const char* EFFECT_CHANGE;
extern const char* EFFECT_DELETE;
extern const char* EFFECT_NEXT;
extern const char* EFFECT_START;
extern const char* EFFECT_SOUNDOFF;
extern const char* EFFECT_SOUNDON;
extern const char* EFFECT_WALL;
extern const char* FONT_NAME;

// Setup string
extern const char* BTN_SPR;
extern const char* BTN_CLICK_SPR;