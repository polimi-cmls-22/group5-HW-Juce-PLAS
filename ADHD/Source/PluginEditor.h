/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ChannelComponent.h"
#include "PlasLookAndFeel.h"
#include "PLASLevelMeter.h"


//==============================================================================
/**
*/
class ADHDAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Timer
{
public:
    enum RadioButtonIds
    {
        MSLR = 1002
    };
    ADHDAudioProcessorEditor (ADHDAudioProcessor&);
    ~ADHDAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ADHDAudioProcessor& audioProcessor; 

    juce::Image bg;
    juce::Image bgD;
    juce::Image lrLabel;
    juce::Image msLabel;
    juce::Image lrLabelMeter;
    juce::Image msLabelMeter;

    ChannelComponent channelL;
    ChannelComponent channelR;

    juce::Slider destroyDial;
    juce::ToggleButton destroyButton;
    juce::ToggleButton LRButton;
    juce::ToggleButton MSButton;
    //juce::ToggleButton msLr;
    
    juce::ToggleButton linkButton;
    
    PLASLevelMeter plMeterInL {};
    PLASLevelMeter plMeterInR {};
    PLASLevelMeter plMeterOutL {};
    PLASLevelMeter plMeterOutR {};
    
    void setMonoAttachments();
    void setLinkedAttachments();
    void setUnlinkedAttachments();
    void timerCallback() override;
    
    //ATTACHMENTS
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> linkAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> msAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lrAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> destroyAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainLLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainLRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> destroyGainAttachment;

    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainRRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainRLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeLLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeLRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeRRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeRLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetLLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetLRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetRLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetRRAttachment;
    
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqLLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqLRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqRLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqRRAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneLLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneLRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneRLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneRRAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> qLLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> qLRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> qRLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> qRRAttachment;
    
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqOnLLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqOnLRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqOnRLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqOnRRAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqLPLLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqBPLLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqHPLLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqLPLRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqBPLRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqHPLRAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterListLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterListRAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqLPRRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqBPRRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqHPRRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqLPRLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqBPRLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqHPRLAttachment;
    
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> channelOnLLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> channelOnLRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> channelOnRRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> channelOnRLAttachment;

    
    //LOOK AND FEEL
    
    PLASBigKnobLookFeel bkLeF {};
    PLASLittleKnobLookFeel lkLeF {};
    PLASDestroyButtonLookAndFeel dbLeF {};
    PLASButLinkLookAndFeel blLeF {};
    PLASButOnLookAndFeel bOLeF {};
    PLASButLPLookAndFeel bLPLeF {};
    PLASButBPLookAndFeel bBPLeF {};
    PLASButHPLookAndFeel bHPLeF {};
    PLASMSLRLookAndFeel bMSLRLeF {};
    PLASButLRLookAndFeel bLRLeF {};
    PLASButMSLookAndFeel bMSLeF {};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADHDAudioProcessorEditor);
};
