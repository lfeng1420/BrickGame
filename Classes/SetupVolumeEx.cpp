#include "stdafx.h"
#include "SetupVolumeEx.h"
#include "BarrierLayer.h"


void CSetupVolumeEx::InitUI()
{
	Size visibleSize1 = GET_VISIBLESIZE();
	Size visibleSize(visibleSize1.height, visibleSize1.width);
	Size bgSprSize = GET_CONTENTSIZE(m_pBGSprite);
	Size closeSize = GET_CONTENTSIZE(m_pCloseMenuItem);
	Size bgmLabelSize = GET_CONTENTSIZE(m_pBGMLabel);
	Size bgmVolumeSize = GET_CONTENTSIZE(m_pBGMVolume);
	Size bgmSliderSize = GET_CONTENTSIZE(m_pBGMSlider);
	Size effectLabelSize = GET_CONTENTSIZE(m_pEffectLabel);
	Size effectVolumeSize = GET_CONTENTSIZE(m_pEffetVolume);
	Size effectSliderSize = GET_CONTENTSIZE(m_pEffectSlider);

	float fMaxHeight = max(effectLabelSize.height, effectSliderSize.height);
	float fLabelMaxWidth = max(bgmLabelSize.width, effectLabelSize.width);
	float fTotalWidth = fLabelMaxWidth + X_PADDING + bgmSliderSize.width + X_PADDING + bgmVolumeSize.width;
	float fYPadding = (bgSprSize.height - fMaxHeight * 2) / 8;
	float fXPadding = (visibleSize.width - fTotalWidth) * 0.5f;
	float fBGMY = visibleSize.height * 0.5f + fYPadding + fMaxHeight * 0.5f;
	float fEffectY = visibleSize.height * 0.5f - fYPadding - fMaxHeight * 0.5f;
	float fLabelX = fXPadding + fLabelMaxWidth * 0.5f;
	float fSliderX = fXPadding + fLabelMaxWidth + X_PADDING + bgmSliderSize.width * 0.5f;
	float fVolumeX = fXPadding + fLabelMaxWidth + X_PADDING * 2 + bgmSliderSize.width + bgmVolumeSize.width * 0.5f;

	// Barrier layer
	m_pBarrierLayer->setContentSize(visibleSize);
	m_pBarrierLayer->setPosition(Vec2::ZERO);
	// Background
	m_pBGSprite->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.5f);
	// Close button
	m_pCloseMenuItem->setPosition(visibleSize.width * 0.5f + bgSprSize.width * 0.5f - closeSize.width * 0.5f,
		visibleSize.height * 0.5f + bgSprSize.height * 0.5f - closeSize.height * 0.5f);
	//BGM
	m_pBGMLabel->setPosition(fLabelX, fBGMY);
	m_pBGMSlider->setPosition(fSliderX, fBGMY);
	m_pBGMVolume->setPosition(fVolumeX, fBGMY);
	//Effect
	m_pEffectLabel->setPosition(fLabelX, fEffectY);
	m_pEffectSlider->setPosition(fSliderX, fEffectY);
	m_pEffetVolume->setPosition(fVolumeX, fEffectY);
}
