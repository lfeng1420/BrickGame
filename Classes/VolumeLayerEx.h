#pragma once
#include "VolumeLayer.h"

class CVolumeLayerEx : public CVolumeLayer
{
public:
	// Init UI
	virtual void InitUI();

	// CREATE_FUNC macro
	CREATE_FUNC(CVolumeLayerEx);
};