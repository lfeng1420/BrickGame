#include "stdafx.h"
#include "SetupControlBtnEx.h"
#include "BarrierLayer.h"


void CSetupControlBtnEx::InitUI()
{
    Size visibleSize1 = GET_VISIBLESIZE();
    Size visibleSize(visibleSize1.height, visibleSize1.width);
    Size bgSprSize = GET_CONTENTSIZE(m_pBGSprite);
    Size closeSize = GET_CONTENTSIZE(m_pCloseMenuItem);
    Size scaleLabelSize = GET_CONTENTSIZE(m_pScaleLabel);
    Size scaleVolumeSize = GET_CONTENTSIZE(m_pScaleVolume);
    Size scaleSliderSize = GET_CONTENTSIZE(m_pScaleSlider);
    Size offsetLabelSize = GET_CONTENTSIZE(m_pOffsetLabel);
    Size offsetVolumeSize = GET_CONTENTSIZE(m_pOffsetVolume);
    Size offsetSliderSize = GET_CONTENTSIZE(m_pOffsetSlider);

    float fMaxHeight = max(offsetLabelSize.height, offsetSliderSize.height);
    float fLabelMaxWidth = max(scaleLabelSize.width, offsetLabelSize.width);
    float fTotalWidth = fLabelMaxWidth + X_PADDING + scaleSliderSize.width + X_PADDING + scaleVolumeSize.width;
    float fYPadding = (bgSprSize.height - fMaxHeight * 2) / 8;
    float fXPadding = (visibleSize.width - fTotalWidth) * 0.5f;
    float fScaleY = visibleSize.height * 0.5f + fYPadding + fMaxHeight * 0.5f;
    float fOffsetY = visibleSize.height * 0.5f - fYPadding - fMaxHeight * 0.5f;
    float fLabelX = fXPadding + fLabelMaxWidth * 0.5f;
    float fSliderX = fXPadding + fLabelMaxWidth + X_PADDING + scaleSliderSize.width * 0.5f;
    float fVolumeX = fXPadding + fLabelMaxWidth + X_PADDING * 2 + scaleSliderSize.width + scaleVolumeSize.width * 0.5f;

    // Barrier layer
    m_pBarrierLayer->setContentSize(visibleSize);
    m_pBarrierLayer->setPosition(Vec2::ZERO);
    // Background
    m_pBGSprite->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.5f);
    // Close button
    m_pCloseMenuItem->setPosition(visibleSize.width * 0.5f + bgSprSize.width * 0.5f - closeSize.width * 0.5f,
        visibleSize.height * 0.5f + bgSprSize.height * 0.5f - closeSize.height * 0.5f);
    //Scale
    m_pScaleLabel->setPosition(fLabelX, fScaleY);
    m_pScaleSlider->setPosition(fSliderX, fScaleY);
    m_pScaleVolume->setPosition(fVolumeX, fScaleY);
    //Offset
    m_pOffsetLabel->setPosition(fLabelX, fOffsetY);
    m_pOffsetSlider->setPosition(fSliderX, fOffsetY);
    m_pOffsetVolume->setPosition(fVolumeX, fOffsetY);
}
