/*
  ==============================================================================

    ChannelComponent.h
    Created: 19 Apr 2022 3:56:17pm
    Author:  stefa

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ChannelComponent : public juce::Component, public juce::Slider::Listener
{
public:
    enum RadioButtonIds
    {
        Filters = 1001
    };
    ChannelComponent();
    ~ChannelComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;
    
    // GETTERS
    juce::Slider* getFreqDial();
    juce::Slider* getQFactorDial();
    juce::Slider* getInGainDial();
    juce::Slider* getOutGainDial();
    juce::Slider* getToneDial();
    juce::Slider* getDryWetDial();

    juce::ToggleButton* getChannelOn();
    juce::ToggleButton* getFilterOn();
    
    juce::ToggleButton* getFilterLP();
    juce::ToggleButton* getFilterBP();
    juce::ToggleButton* getFilterHP();
    
    juce::ComboBox* getFilterList();
    //LOOKANDFEEL
    void setBKLeF(juce::LookAndFeel* lef);
    void setLKLeF(juce::LookAndFeel* lef);
    void setOnButtonsLeF(juce::LookAndFeel* lef);
    
    void setLPButtonsLeF(juce::LookAndFeel* lef);
    void setBPButtonsLeF(juce::LookAndFeel* lef);
    void setHPButtonsLeF(juce::LookAndFeel* lef);
    
    void setFilterListLeF(juce::LookAndFeel* lef);

    //DESTRUCTOR
    void setNullLookAndFeel();
    
private:
    // Dials
    juce::Slider freqDial;
    juce::Slider qFactorDial;

    juce::Slider inGainDial;
    juce::Slider outGainDial;
    juce::Slider toneDial;

    juce::Slider drywetDial;

    // Buttons
    juce::ToggleButton channelOn;

    juce::ToggleButton filterOn;
    
    juce::ToggleButton filterLP;
    juce::ToggleButton filterBP;
    juce::ToggleButton filterHP;
    
    juce::ComboBox filterList;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelComponent)
};
