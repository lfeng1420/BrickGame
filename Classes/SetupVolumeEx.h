#pragma once
#include "SetupVolume.h"

class CSetupVolumeEx : public CSetupVolume
{
public:
	// Init UI
	virtual void InitUI();

	// CREATE_FUNC macro
	CREATE_FUNC(CSetupVolumeEx);
};