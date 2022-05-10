/*
 ==============================================================================
 
 PLASLevelMeter.cpp
 Created: 3 May 2022 8:50:14am
 Author:  paolo ostan
 
 ==============================================================================
 */

#include "PLASLevelMeter.h"

PLASLevelMeter::PLASLevelMeter():PLASLevelMeter(
                                                juce::ImageCache::getFromMemory(BinaryData::Meter_png,BinaryData::Meter_pngSize),
                                                juce::ImageCache::getFromMemory(BinaryData::Meter_Destroyed_png,BinaryData::Meter_Destroyed_pngSize),
                                                9,
                                                false) {}


PLASLevelMeter::PLASLevelMeter(juce::Image aMeterStripN, juce::Image aMeterStripD, float aFrames, bool aDestroy) {
    meterStripD=aMeterStripD;
    meterStripN=aMeterStripN;
    frames = aFrames;
    destroy = aDestroy;
}

void PLASLevelMeter::paint(juce::Graphics &g) {
    auto bounds = getLocalBounds().toFloat();
    float width = bounds.getWidth();
    float height = bounds.getHeight();
    
    
    juce::Image meterStrip = juce::Image();
    
    if(destroy) {
        meterStrip = meterStripD;
    }
    else {
        meterStrip = meterStripN;
    }
    float meterDimW = width;
    float meterDimH = height;
    float frameDimW = meterStrip.getWidth();
    float frameDimH = (meterStrip.getHeight())/ frames;
    if ((meterDimW / frameDimW) <= (meterDimH / frameDimH)) {
        meterDimH = (meterDimW / frameDimW) * frameDimH;
        meterDimW = (meterDimW / frameDimW) * frameDimW;
    }
    else {
        meterDimH = (meterDimH / frameDimH) * frameDimH;
        meterDimW = (meterDimH / frameDimH) * frameDimW;
    }
    const double excursion = (level - minLevel) / (maxLevel - minLevel);
    const int frameId = (int)ceil(excursion * ((double)frames - 1.0));
    int centerX = width / 2;
    int centerY = height / 2;
    int sX = centerX - (meterDimW / 2);
    int sY = centerY - (meterDimH / 2);
    //juce::String dbg (frameId);
    //juce::Logger::outputDebugString (dbg);
    g.drawImage(meterStrip, sX, sY, meterDimW, meterDimH, 0, frameId * frameDimH, frameDimW, frameDimH );
}

void PLASLevelMeter::setLevel(const float value) {
    if(value<-60.0f)
        level = (-60.0f);
    else if (value > 6.0f)
        level = 6.0f;
    else
        level =value;
}
void PLASLevelMeter::setDestroy(bool aDestroy) {
    destroy = aDestroy;
}
