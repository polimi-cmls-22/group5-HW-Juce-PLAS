/*
  ==============================================================================

    PLASLevelMeter.h
    Created: 3 May 2022 8:50:14am
    Author:  paolo ostan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class PLASLevelMeter: public juce::Component {
public:
    PLASLevelMeter();
    PLASLevelMeter(juce::Image aMeterStripN, juce::Image aMeterStripD,float aFrames,bool aDestroy);
    void paint(juce::Graphics& g) override;
    void setLevel(const float value);
    void setDestroy(bool aDestroy);
private:
    const float minLevel{-60.0f};
    const float maxLevel{+6.0f};
    float level {-60.0f};
    juce::Image meterStripD {};
    juce::Image meterStripN {};
    bool destroy {false};
    float frames;
     
};
