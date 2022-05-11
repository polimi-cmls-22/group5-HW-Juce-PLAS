/*
  =============================================================================

    This file contains the basic framework code for a JUCE plugin editor

  =============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ChannelComponent.h"

//=============================================================================
ADHDAudioProcessorEditor::ADHDAudioProcessorEditor(ADHDAudioProcessor& p): AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    destroyDial.setRange(0.0, 1.0, 1. / 128);
    destroyDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    destroyDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    destroyDial.setLookAndFeel(&lkLeF);
    //LRButton.setToggleState(true, false);
    //linkButton.setToggleState(true, false);
    
    bg = juce::ImageCache::getFromMemory(BinaryData::GUI_definitivaBG_png, BinaryData::GUI_definitivaBG_pngSize);
    bgD = juce::ImageCache::getFromMemory(BinaryData::GUI_definitivaBG_Destroy_png, BinaryData::GUI_definitivaBG_Destroy_pngSize);
    
    lrLabel = juce::ImageCache::getFromMemory(BinaryData::LR_labels_png, BinaryData::LR_labels_pngSize);
    msLabel = juce::ImageCache::getFromMemory(BinaryData::MS_labels_png, BinaryData::MS_labels_pngSize);
    lrLabelMeter = juce::ImageCache::getFromMemory(BinaryData::LR_labels_meters_png, BinaryData::LR_labels_meters_pngSize);
    msLabelMeter = juce::ImageCache::getFromMemory(BinaryData::MS_labels_meters_png, BinaryData::MS_labels_meters_pngSize);


    linkButton.onClick = [this] {
        bool stateLink = linkButton.getToggleState();
        if(stateLink)
            setLinkedAttachments();
        else {
            setUnlinkedAttachments();
        }
    };
    
    LRButton.setRadioGroupId(MSLR);
    MSButton.setRadioGroupId(MSLR);
    //ATTACHMENTS
    linkAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "LINK", linkButton));
    msAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "MIDSIDE", MSButton));
    lrAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "LEFTRIGHT", LRButton));
    destroyAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "DESTROY", destroyButton));
    gainLLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "GAINL", *channelL.getInGainDial()));
    gainRRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "GAINR", *channelR.getInGainDial()));
    volumeLLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "VOLUMEL", *channelL.getOutGainDial()));
    volumeRRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "VOLUMER", *channelR.getOutGainDial()));
    dryWetLLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "DRYWETL", *channelL.getDryWetDial()));
    dryWetRRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "DRYWETR", *channelR.getDryWetDial()));
    freqLLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "FREQL", *channelL.getFreqDial()));
    freqRRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "FREQR", *channelR.getFreqDial()));
    qLLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "QL", *channelL.getQFactorDial()));
    qRRAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "QR", *channelR.getQFactorDial()));
    
    destroyGainAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "DESTROYGAIN", destroyDial));
    
    eqLPLLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQLPL", *channelL.getFilterLP()));
    eqBPLLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQBPL", *channelL.getFilterBP()));
    eqHPLLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQHPL", *channelL.getFilterHP()));
     
    filterListLAttachment.reset (new juce::AudioProcessorValueTreeState::ComboBoxAttachment (audioProcessor.treeState, "EQSELECTL", *channelL.getFilterList()));
    filterListRAttachment.reset (new juce::AudioProcessorValueTreeState::ComboBoxAttachment (audioProcessor.treeState, "EQSELECTR", *channelR.getFilterList()));
    
    eqLPRRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQLPR", *channelR.getFilterLP()));
    eqBPRRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQBPR", *channelR.getFilterBP()));
    eqHPRRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQHPR", *channelR.getFilterHP()));
    
    eqOnLLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQONL", *channelL.getFilterOn()));
    eqOnRRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQONR", *channelR.getFilterOn()));
    channelOnLLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "CHANNELONL", *channelL.getChannelOn()));
    channelOnRRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "CHANNELONR", *channelR.getChannelOn()));

    toneLLAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "TONEL", *channelL.getToneDial()));
    toneRRAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "TONER", *channelR.getToneDial()));

    destroyButton.setLookAndFeel(&dbLeF);
    
    linkButton.setLookAndFeel(&blLeF);
    MSButton.setLookAndFeel(&bMSLeF);
    LRButton.setLookAndFeel(&bLRLeF);
    //msLr.setLookAndFeel(&bMSLRLeF);

    
    channelL.setBKLeF(&bkLeF);
    channelR.setBKLeF(&bkLeF);
    channelL.setLKLeF(&lkLeF);
    channelR.setLKLeF(&lkLeF);
    channelL.setOnButtonsLeF(&bOLeF);
    channelR.setOnButtonsLeF(&bOLeF);
    channelL.setLPButtonsLeF(&bLPLeF);
    channelR.setLPButtonsLeF(&bLPLeF);
    channelL.setBPButtonsLeF(&bBPLeF);
    channelR.setBPButtonsLeF(&bBPLeF);
    channelL.setHPButtonsLeF(&bHPLeF);
    channelR.setHPButtonsLeF(&bHPLeF);
    
    
    
    destroyButton.onClick = [this] {
        bool state = destroyButton.getToggleState();
        bkLeF.setDestroy(state);
        lkLeF.setDestroy(state);
        blLeF.setDestroy(state);
        bOLeF.setDestroy(state);
        bLPLeF.setDestroy(state);
        bBPLeF.setDestroy(state);
        bHPLeF.setDestroy(state);
        bMSLRLeF.setDestroy(state);
        plMeterInL.setDestroy(state);
        plMeterInR.setDestroy(state);
        plMeterOutL.setDestroy(state);
        plMeterOutR.setDestroy(state);
        destroyDial.setEnabled(state);
        bLRLeF.setDestroy(state);
        bMSLeF.setDestroy(state);
        repaint();
        
    };
    
    
    
    LRButton.onClick = [this] {
        repaint();
    };
    MSButton.onClick = [this] {
        repaint();
    };
    bool state = destroyButton.getToggleState();
    destroyDial.setEnabled(state);
    bkLeF.setDestroy(state);
    lkLeF.setDestroy(state);
    blLeF.setDestroy(state);
    bOLeF.setDestroy(state);
    bLPLeF.setDestroy(state);
    bBPLeF.setDestroy(state);
    bHPLeF.setDestroy(state);
    bMSLRLeF.setDestroy(state);
    plMeterInL.setDestroy(state);
    plMeterInR.setDestroy(state);
    plMeterOutL.setDestroy(state);
    plMeterOutR.setDestroy(state);
    bLRLeF.setDestroy(state);
    bMSLeF.setDestroy(state);
    repaint();
    
    
    //ADD AND MAKE VISIBLE
    
    
    addAndMakeVisible(channelL);
    addAndMakeVisible(channelR);
    addAndMakeVisible(destroyDial);
    addAndMakeVisible(destroyButton);
    addAndMakeVisible(LRButton);
    addAndMakeVisible(MSButton);
    //addAndMakeVisible(msLr);
    
    addAndMakeVisible(linkButton);

    addAndMakeVisible(plMeterInL);
    addAndMakeVisible(plMeterInR);
    addAndMakeVisible(plMeterOutL);
    addAndMakeVisible(plMeterOutR);
       // Make sure that before the constructor has finished, you've set the
       // editor's size to whatever you need it to be.
       startTimerHz(24);
    
    setSize (1000, 432);
}

ADHDAudioProcessorEditor::~ADHDAudioProcessorEditor()
{
    channelL.setNullLookAndFeel();
    channelR.setNullLookAndFeel();

    destroyButton.setLookAndFeel(nullptr);
    destroyDial.setLookAndFeel(nullptr);
    //msLr.setLookAndFeel(nullptr);
    LRButton.setLookAndFeel(nullptr);
    MSButton.setLookAndFeel(nullptr);
    linkButton.setLookAndFeel(nullptr);
}

//=============================================================================
void ADHDAudioProcessorEditor::paint (juce::Graphics& g)
{
    if (destroyButton.getToggleState()) g.drawImageAt(bgD, 0, 0);
    else g.drawImageAt(bg, 0, 0);

    if (LRButton.getToggleState()) {
        g.drawImageAt(lrLabel, 77, 82+32);
        g.drawImageAt(lrLabelMeter, 859, 86+32);
    }
    else {
        g.drawImageAt(msLabel, 77, 82+32);
        g.drawImageAt(msLabelMeter, 859, 86+32);
    }
}

void ADHDAudioProcessorEditor::resized()
{
   // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // Link Button Bounds
    juce::Rectangle<int> linkButtonArea(109, 213, 22, 38);

    // Channels Bounds
    juce::Rectangle<int> channelAreaL(30, 62, 780, 160);
    juce::Rectangle<int> channelAreaR(30, 242, 780, 160);

    // Destroy Bounds
    juce::Rectangle<int> destroySectionArea(830, 62, 140, 340);
    
    const int xOffset = destroySectionArea.getX();
    const int yOffset = destroySectionArea.getY();

    juce::Rectangle<int> destroyButtonArea(20 + xOffset, 270 + yOffset, 100, 60);
    juce::Rectangle<int> destroyDialArea(30 + xOffset, 180 + yOffset, 80, 80);

    juce::Rectangle<int> LRButtonArea(30 + xOffset, 20 + yOffset, 30, 30);
    juce::Rectangle<int> MSButtonArea(78 + xOffset, 20 + yOffset, 30, 30);
    //juce::Rectangle<int> msLrArea(30 + xOffset, 20 + yOffset, 78, 30);
    // Setting Bounds
    linkButton.setBounds(linkButtonArea);

    channelL.setBounds(channelAreaL);
    channelR.setBounds(channelAreaR);

    destroyButton.setBounds(destroyButtonArea);
    destroyDial.setBounds(destroyDialArea);
    LRButton.setBounds(LRButtonArea);
    MSButton.setBounds(MSButtonArea);
    //msLr.setBounds(msLrArea);
    
    juce::Rectangle<int> inMeterAreaL(26 + xOffset, 65 + yOffset, 17, 105);
    juce::Rectangle<int> inMeterAreaR(44 + xOffset, 65 + yOffset, 17, 105);
    
    juce::Rectangle<int> outMeterAreaL(79 + xOffset, 65 + yOffset, 17, 105);
    juce::Rectangle<int> outMeterAreaR(97 + xOffset, 65 + yOffset, 17, 105);
    
    plMeterInL.setBounds(inMeterAreaL);
    plMeterInR.setBounds(inMeterAreaR);
    
    plMeterOutL.setBounds(outMeterAreaL);
    plMeterOutR.setBounds(outMeterAreaR);
    
    
    
}


void ADHDAudioProcessorEditor::setLinkedAttachments() {
    
    // R -> L
    gainRLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "GAINL", *channelR.getInGainDial()));
    volumeRLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "VOLUMEL", *channelR.getOutGainDial()));
    dryWetRLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "DRYWETL", *channelR.getDryWetDial()));
    freqRLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "FREQL", *channelR.getFreqDial()));
    qRLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "QL", *channelR.getQFactorDial()));
    eqLPRLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQLPL", *channelR.getFilterLP()));
    eqBPRLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQBPL", *channelR.getFilterBP()));
    eqHPRLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQHPL", *channelR.getFilterHP()));
    eqOnRLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQONL", *channelR.getFilterOn()));
    channelOnRLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "CHANNELONL", *channelR.getChannelOn()));
    toneRLAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "TONEL", *channelR.getToneDial()));
    
    // L -> R
    gainLRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "GAINR", *channelL.getInGainDial()));
    volumeLRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "VOLUMER", *channelL.getOutGainDial()));
    dryWetLRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "DRYWETR", *channelL.getDryWetDial()));
    freqLRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "FREQR", *channelL.getFreqDial()));
    qLRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "QR", *channelL.getQFactorDial()));
    eqLPLRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQLPR", *channelL.getFilterLP()));
    eqBPLRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQBPR", *channelL.getFilterBP()));
    eqHPLRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQHPR", *channelL.getFilterHP()));
    eqOnLRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQONR", *channelL.getFilterOn()));
    channelOnLRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "CHANNELONR", *channelL.getChannelOn()));
    toneLRAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "TONER", *channelL.getToneDial()));
   
}

void ADHDAudioProcessorEditor::setUnlinkedAttachments() {
    gainLRAttachment.reset ();
    gainRLAttachment.reset ();
    
    volumeLRAttachment.reset ();
    volumeRLAttachment.reset ();
    dryWetLRAttachment.reset ();
    dryWetRLAttachment.reset ();
    freqLRAttachment.reset ();
    freqRLAttachment.reset ();
    qLRAttachment.reset ();
    qRLAttachment.reset ();

    toneLRAttachment.reset();
    toneRLAttachment.reset();
    
    eqLPLRAttachment.reset ();
    eqBPLRAttachment.reset ();
    eqHPLRAttachment.reset ();
    
    eqLPRLAttachment.reset ();
    eqBPRLAttachment.reset ();
    eqHPRLAttachment.reset ();
    
    eqOnLRAttachment.reset ();
    eqOnRLAttachment.reset ();
    channelOnLRAttachment.reset ();
    channelOnRLAttachment.reset ();
}

void ADHDAudioProcessorEditor::timerCallback() {
    plMeterInL.setLevel(audioProcessor.getMAGValue(0,0));
    plMeterInR.setLevel(audioProcessor.getMAGValue(0,1));
    plMeterOutL.setLevel(audioProcessor.getMAGValue(1,0));
    plMeterOutR.setLevel(audioProcessor.getMAGValue(1,1));
    
    /*
    plMeterInL.setLevel(audioProcessor.getRMSValue(0,0));
    plMeterInR.setLevel(audioProcessor.getRMSValue(0,1));
    plMeterOutL.setLevel(audioProcessor.getRMSValue(1,0));
    plMeterOutR.setLevel(audioProcessor.getRMSValue(1,1));
    */
    
    plMeterInL.repaint();
    plMeterInR.repaint();
    plMeterOutL.repaint();
    plMeterOutR.repaint();
    
}
