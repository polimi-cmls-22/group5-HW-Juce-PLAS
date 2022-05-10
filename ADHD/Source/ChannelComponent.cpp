/*
  ==============================================================================

    ChannelComponent.cpp
    Created: 19 Apr 2022 3:56:17pm
    Author:  stefa

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ChannelComponent.h"


//==============================================================================
ChannelComponent::ChannelComponent()
{   
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    freqDial.setRange(0.0, 1.0, 1. / 128);
    freqDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    freqDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    freqDial.setValue(0.5);

    qFactorDial.setRange(0.0, 1.0, 1. / 128);
    qFactorDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    qFactorDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    qFactorDial.setValue(0.5);
    qFactorDial.setDoubleClickReturnValue(true, 0.5);

    inGainDial.setRange(0.0, 1.0, 1. / 128);
    inGainDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    inGainDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    inGainDial.setValue(0.5);

    toneDial.setRange(0.0, 1.0, 1. / 128);
    toneDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    toneDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    toneDial.setValue(1);

    outGainDial.setRange(0.0, 1.0, 1. / 128);
    outGainDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    outGainDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    outGainDial.setValue(0.5);

    drywetDial.setRange(0.0, 1.0, 1. / 128);
    drywetDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    drywetDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    drywetDial.setValue(1);

    //channelOn.setToggleState(true, false);
    //filterOn.setToggleState(true, false);

    //NAMES
    freqDial.setName("freqDial");
    qFactorDial.setName("qFactorDial");
    inGainDial.setName("inGainDial");
    toneDial.setName("toneDial");
    outGainDial.setName("outGainDial");
    drywetDial.setName("drywetDial");

    channelOn.setName("channelOn");
    filterOn.setName("filterOn");
    filterList.setName("filterList");
    
    filterList.addItem("LowPass", 1);
    filterList.addItem("BandPass", 2);
    filterList.addItem("HighPass", 3);
    
    
    filterLP.setName("filterLP");
    filterBP.setName("filterBP");
    filterHP.setName("filterHP");
    
    filterOn.onClick = [this] {
        bool stateFilt = filterOn.getToggleState();
        bool stateOn = channelOn.getToggleState();
        filterLP.setEnabled(stateFilt&&stateOn);
        filterBP.setEnabled(stateFilt&&stateOn);
        filterHP.setEnabled(stateFilt&&stateOn);
        freqDial.setEnabled(stateFilt&&stateOn);
        qFactorDial.setEnabled(stateFilt&&stateOn);
        repaint();
    };
    
    
    channelOn.onClick = [this] {
        bool stateOn = channelOn.getToggleState();
        inGainDial.setEnabled(stateOn);
        outGainDial.setEnabled(stateOn);
        freqDial.setEnabled(stateOn);
        qFactorDial.setEnabled(stateOn);
        toneDial.setEnabled(stateOn);
        drywetDial.setEnabled(stateOn);
        filterOn.setEnabled(stateOn);
        bool stateFilt = filterOn.getToggleState();
        filterLP.setEnabled(stateFilt&&stateOn);
        filterBP.setEnabled(stateFilt&&stateOn);
        filterHP.setEnabled(stateFilt&&stateOn);
        freqDial.setEnabled(stateFilt&&stateOn);
        qFactorDial.setEnabled(stateFilt&&stateOn);
        repaint();
    };
    
    
    filterLP.setRadioGroupId(Filters);
    filterBP.setRadioGroupId(Filters);
    filterHP.setRadioGroupId(Filters);
    
    
    bool stateOn = channelOn.getToggleState();
    inGainDial.setEnabled(stateOn);
    outGainDial.setEnabled(stateOn);
    freqDial.setEnabled(stateOn);
    qFactorDial.setEnabled(stateOn);
    toneDial.setEnabled(stateOn);
    drywetDial.setEnabled(stateOn);
    filterOn.setEnabled(stateOn);
    bool stateFilt = filterOn.getToggleState();
    filterLP.setEnabled(stateFilt&&stateOn);
    filterBP.setEnabled(stateFilt&&stateOn);
    filterHP.setEnabled(stateFilt&&stateOn);
    freqDial.setEnabled(stateFilt&&stateOn);
    qFactorDial.setEnabled(stateFilt&&stateOn);
    
    //ADD AND MAKE VISIBLE
    addAndMakeVisible(freqDial);
    addAndMakeVisible(qFactorDial);

    addAndMakeVisible(inGainDial);
    addAndMakeVisible(outGainDial);
    addAndMakeVisible(toneDial);
    addAndMakeVisible(drywetDial);

    addAndMakeVisible(channelOn);

    addAndMakeVisible(filterOn);
    
    addAndMakeVisible(filterLP);
    addAndMakeVisible(filterBP);
    addAndMakeVisible(filterHP);
    
    //addAndMakeVisible(filterList);
    // Listeners
    
    
}

ChannelComponent::~ChannelComponent()
{
    setNullLookAndFeel();
}

void ChannelComponent::paint (juce::Graphics& g)
{
    const int buttonDimension = 30; //px
    const int primaryDialDimension = 120; //px
    const int secondaryDialDimension = 80; //px

    //Buttons Bounds
    juce::Rectangle<int> channelOnArea(20, 65, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterOnArea(123, 20, buttonDimension, buttonDimension);
    
    juce::Rectangle<int> filterLPArea(157, 20, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterBPArea(193, 20, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterHPArea(229, 20, buttonDimension, buttonDimension);
     
    juce::Rectangle<int> filtersArea(157, 20, (3*buttonDimension)+(2*6), buttonDimension);

    //Dials Bounds
    juce::Rectangle<int> freqDialArea(109, 62, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> qFactorDialArea(189, 62, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> inGainDialArea(292, 20, primaryDialDimension, primaryDialDimension);
    juce::Rectangle<int> toneDialArea(428, 40, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> outGainDialArea(524, 20, primaryDialDimension, primaryDialDimension);
    juce::Rectangle<int> drywetDialArea(660, 40, secondaryDialDimension, secondaryDialDimension);

    /*g.setColour(juce::Colours::aquamarine);
    g.drawRect(inGainDialArea);
    g.drawRect(outGainDialArea);
    g.drawRect(toneDialArea);
    g.drawRect(drywetDialArea);
    g.drawRect(channelOnArea);

    g.setColour(juce::Colours::pink);
    g.drawRect(filterOnArea);
    g.drawRect(filterLPArea);
    g.drawRect(filterBPArea);
    g.drawRect(filterHPArea);
    g.drawRect(filtersArea);
    g.drawRect(freqDialArea);
    g.drawRect(qFactorDialArea);*/
}

void ChannelComponent::resized()
{
    const int buttonDimension = 30; //px
    const int primaryDialDimension = 120; //px
    const int secondaryDialDimension = 80; //px

    //Buttons Bounds
    juce::Rectangle<int> channelOnArea(20, 65, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterOnArea(123, 20, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterLPArea(157, 20, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterBPArea(193, 20, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterHPArea(229, 20, buttonDimension, buttonDimension);
    juce::Rectangle<int> filtersArea(157, 20, (3*buttonDimension)+(2*6), buttonDimension);

    //Dials Bounds
    juce::Rectangle<int> freqDialArea(109, 62, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> qFactorDialArea(189, 62, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> inGainDialArea(292, 20, primaryDialDimension, primaryDialDimension);
    juce::Rectangle<int> toneDialArea(428, 40, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> outGainDialArea(524, 20, primaryDialDimension, primaryDialDimension);
    juce::Rectangle<int> drywetDialArea(660, 40, secondaryDialDimension, secondaryDialDimension);
    
        

    //SETTING BOUNDS
    freqDial.setBounds(freqDialArea);
    qFactorDial.setBounds(qFactorDialArea);
    inGainDial.setBounds(inGainDialArea);
    toneDial.setBounds(toneDialArea);
    outGainDial.setBounds(outGainDialArea);
    drywetDial.setBounds(drywetDialArea);

    channelOn.setBounds(channelOnArea);
    filterOn.setBounds(filterOnArea);
    
    filterLP.setBounds(filterLPArea);
    filterBP.setBounds(filterBPArea);
    filterHP.setBounds(filterHPArea);
    
    filterList.setBounds(filtersArea);
}

void ChannelComponent::sliderValueChanged(juce::Slider* slider)
{   
    /*if (slider == &freqDial) { DBG("freqDial: " << freqDial.getValue()); };
    if (slider == &qFactorDial) { DBG("qFactorDial: " << qFactorDial.getValue()); };
    if (slider == &inGainDial) { DBG("inGainDial: " << inGainDial.getValue()); };
    if (slider == &outGainDial) { DBG("outGainDial: " << outGainDial.getValue()); };
    if (slider == &drywetDial) { DBG("drywetDial: " << drywetDial.getValue()); };*/
}

juce::Slider* ChannelComponent::getFreqDial()
{
    return &freqDial;
}

juce::Slider* ChannelComponent::getQFactorDial()
{
    return &qFactorDial;
}

juce::Slider* ChannelComponent::getInGainDial()
{
    return &inGainDial;
}

juce::Slider* ChannelComponent::getOutGainDial()
{
    return &outGainDial;
}

juce::Slider* ChannelComponent::getToneDial()
{
    return &toneDial;
}

juce::Slider* ChannelComponent::getDryWetDial()
{
    return &drywetDial;
}

juce::ToggleButton* ChannelComponent::getChannelOn()
{
    return &channelOn;
}

juce::ToggleButton* ChannelComponent::getFilterOn()
{
    return &filterOn;
}

juce::ToggleButton* ChannelComponent::getFilterLP()
{
    return &filterLP;
}

juce::ToggleButton* ChannelComponent::getFilterBP()
{
    return &filterBP;
}

juce::ToggleButton* ChannelComponent::getFilterHP()
{
    return &filterHP;
}

juce::ComboBox* ChannelComponent::getFilterList()
{
    return &filterList;
}

void ChannelComponent::setBKLeF(juce::LookAndFeel *lef){
    inGainDial.setLookAndFeel(lef);
    outGainDial.setLookAndFeel(lef);
    
}
void ChannelComponent::setLKLeF(juce::LookAndFeel *lef){
    freqDial.setLookAndFeel(lef);
    qFactorDial.setLookAndFeel(lef);
    toneDial.setLookAndFeel(lef);
    drywetDial.setLookAndFeel(lef);
    
}
void ChannelComponent::setOnButtonsLeF(juce::LookAndFeel *lef){
    channelOn.setLookAndFeel(lef);
    filterOn.setLookAndFeel(lef);
}

void ChannelComponent::setLPButtonsLeF(juce::LookAndFeel* lef) {
    filterLP.setLookAndFeel(lef);
}
void ChannelComponent::setBPButtonsLeF(juce::LookAndFeel* lef){
    filterBP.setLookAndFeel(lef);
}
void ChannelComponent::setHPButtonsLeF(juce::LookAndFeel* lef){
    filterHP.setLookAndFeel(lef);
}
 

void ChannelComponent::setFilterListLeF(juce::LookAndFeel* lef){
    filterList.setLookAndFeel(lef);
}

void ChannelComponent::setNullLookAndFeel()
{
    setBKLeF(nullptr);
    setLKLeF(nullptr);
    setOnButtonsLeF(nullptr);

    setLPButtonsLeF(nullptr);
    setBPButtonsLeF(nullptr);
    setHPButtonsLeF(nullptr);

    setFilterListLeF(nullptr);
}
