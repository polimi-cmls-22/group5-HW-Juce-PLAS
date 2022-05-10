/*
 ==============================================================================
 
 PlasLookAndFeel.cpp
 Created: 12 Apr 2022 3:32:30pm
 Author:  Pox17
 
 ==============================================================================
 */

#include "PlasLookAndFeel.h"
#include <BinaryData.cpp>

PLASLookAndFeel::PLASLookAndFeel() 
{
    knobStripN = juce::Image();
    knobStripD = juce::Image();
    knobStripDisabled = juce::Image();
    frames = 0;
    destroy = false;
}

void PLASLookAndFeel::setKnobStripN(juce::Image src) {
    knobStripN = src;
}
void PLASLookAndFeel::setKnobStripD(juce::Image src) {
    knobStripD = src;
}
void PLASLookAndFeel::setKnobStripDisabled(juce::Image src) {
    knobStripDisabled = src;
}
void PLASLookAndFeel::setFrames(int fr) {
    frames = fr;
}
void PLASLookAndFeel::setDestroy(bool dest) {
    destroy = dest;
}
void PLASLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                       const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    juce::Image knobStrip = juce::Image();
    if(slider.isEnabled()) {
        if(destroy) {
            knobStrip = knobStripD;
        }
        else {
            knobStrip = knobStripN;
        }
    } else {
        knobStrip=knobStripDisabled;
    }
    float knobDimW = width;
    float knobDimH = height;
    float frameDimW = knobStrip.getWidth();
    float frameDimH = knobStrip.getHeight() / frames;
    if ((knobDimW / frameDimW) <= (knobDimH / frameDimH)) {
        knobDimH = (knobDimW / frameDimW) * frameDimH;
        knobDimW = (knobDimW / frameDimW) * frameDimW;
    }
    else {
        knobDimH = (knobDimH / frameDimH) * frameDimH;
        knobDimW = (knobDimH / frameDimH) * frameDimW;
    }
    const double rotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum());
    const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
    int centerX = width / 2;
    int centerY = height / 2;
    int sX = centerX - (knobDimW / 2);
    int sY = centerY - (knobDimH / 2);
    g.drawImage(knobStrip, sX, sY, knobDimW, knobDimH, 0, frameId * frameDimH, frameDimW, frameDimH);
}

/*
 knobs LookAndFeel implementation
 */

PLASBigKnobLookFeel::PLASBigKnobLookFeel() {
    PLASLookAndFeel::setKnobStripD(juce::ImageCache::getFromMemory(BinaryData::Primary_Destroyed_png, BinaryData::Primary_Destroyed_pngSize));
    PLASLookAndFeel::setKnobStripN(juce::ImageCache::getFromMemory(BinaryData::Primary_png, BinaryData::Primary_pngSize));
    PLASLookAndFeel::setKnobStripDisabled(juce::ImageCache::getFromMemory(BinaryData::Primary_Disabled_png, BinaryData::Primary_Disabled_pngSize));
    
    PLASLookAndFeel::setFrames(128);
}

PLASLittleKnobLookFeel::PLASLittleKnobLookFeel() {
    PLASLookAndFeel::setKnobStripD(juce::ImageCache::getFromMemory(BinaryData::Secondary_Destroyed_png, BinaryData::Secondary_Destroyed_pngSize));
    PLASLookAndFeel::setKnobStripN(juce::ImageCache::getFromMemory(BinaryData::Secondary_png, BinaryData::Secondary_pngSize));
    PLASLookAndFeel::setKnobStripDisabled(juce::ImageCache::getFromMemory(BinaryData::Secondary_Disabled_png, BinaryData::Secondary_Disabled_pngSize));
    PLASLookAndFeel::setFrames(128);
}


PLASDestroyButtonLookAndFeel::PLASDestroyButtonLookAndFeel() {
    buttonOff = juce::ImageCache::getFromMemory(BinaryData::dButtonOff_png, BinaryData::dButtonOff_pngSize);
    buttonOn = juce::ImageCache::getFromMemory(BinaryData::dButtonOn_png, BinaryData::dButtonOn_pngSize);
}

void PLASDestroyButtonLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    juce::Image btn = juce::Image();
    
    if (button.getToggleState()) {
        btn = buttonOn;
    }
    else {
        btn = buttonOff;
    }
    float btDimW = (float)button.getWidth();
    float btDimH = (float)button.getHeight();
    float frameDimW = (float)btn.getWidth();
    float frameDimH = (float)btn.getHeight();
    
    if ((btDimW/frameDimW) <= ( btDimH / frameDimH)) {
        btDimH = (btDimW / frameDimW) * frameDimH;
        btDimW = (btDimW / frameDimW) * frameDimW;
    }
    else {
        btDimH = (btDimH / frameDimH) * frameDimH;
        btDimW = (btDimH / frameDimH) * frameDimW;
    }
    int centerX = (button.getWidth()) / 2;
    int centerY = (button.getHeight()) / 2;
    int sX = centerX - (btDimW / 2);
    int sY = centerY - (btDimH / 2);
    g.drawImage(btn, sX, sY, btDimW, btDimH, 0, 0, frameDimW, frameDimH);
    
}

//PLASButtonLookAndFeel

PLASButtonLookAndFeel::PLASButtonLookAndFeel(juce::Image abuttonOnN,
                                             juce::Image abuttonOffN,
                                             juce::Image abuttonOnD,
                                             juce::Image abuttonOffD,
                                             bool aDestroy){
    buttonOnN = abuttonOnN;
    buttonOffN = abuttonOffN;
    buttonOnD = abuttonOnD;
    buttonOffD = abuttonOffD;
    destroy = aDestroy;
}

void PLASButtonLookAndFeel::setDestroy(bool val) {
    destroy = val;
}


void PLASButtonLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    juce::Image btn = juce::Image();
    if(button.isEnabled()) {
        if(!destroy) {
            if (button.getToggleState()) {
                btn = buttonOnN;
            }
            else {
                btn = buttonOffN;
            }
        } else {
            if (button.getToggleState()) {
                btn = buttonOnD;
            }
            else {
                btn = buttonOffD;
            }
        }
    } else {
        btn = buttonOffN;
    }
    float btDimW = (float)button.getWidth();
    float btDimH = (float)button.getHeight();
    float frameDimW = (float)btn.getWidth();
    float frameDimH = (float)btn.getHeight();
    
    if ((btDimW/frameDimW) <= ( btDimH / frameDimH)) {
        btDimH = (btDimW / frameDimW) * frameDimH;
        btDimW = (btDimW / frameDimW) * frameDimW;
    }
    else {
        btDimH = (btDimH / frameDimH) * frameDimH;
        btDimW = (btDimH / frameDimH) * frameDimW;
    }
    int centerX = (button.getWidth()) / 2;
    int centerY = (button.getHeight()) / 2;
    int sX = centerX - (btDimW / 2);
    int sY = centerY - (btDimH / 2);
    g.drawImage(btn, sX, sY, btDimW, btDimH, 0, 0, frameDimW, frameDimH);
    
}


PLASButLinkLookAndFeel::PLASButLinkLookAndFeel(): PLASButtonLookAndFeel(juce::ImageCache::getFromMemory(BinaryData::Link_Button_png, BinaryData::Link_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::Unlink_Button_png, BinaryData::Unlink_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::Destroyed_Link_Button_png, BinaryData::Destroyed_Link_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::Unlink_Button_png, BinaryData::Unlink_Button_pngSize), false)
{
}
PLASButOnLookAndFeel::PLASButOnLookAndFeel(): PLASButtonLookAndFeel(juce::ImageCache::getFromMemory(BinaryData::ON_Button_png, BinaryData::ON_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_Button_png, BinaryData::OFF_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::Destroyed_ON_Button_png, BinaryData::Destroyed_ON_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_Button_png, BinaryData::OFF_Button_pngSize), false)
{
}
PLASButBPLookAndFeel::PLASButBPLookAndFeel(): PLASButtonLookAndFeel(juce::ImageCache::getFromMemory(BinaryData::ON_BP_Button_png, BinaryData::ON_BP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_BP_Button_png, BinaryData::OFF_BP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::Destroyed_ON_BP_Button_png, BinaryData::Destroyed_ON_BP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_BP_Button_png, BinaryData::OFF_BP_Button_pngSize),false)
{
}
PLASButLPLookAndFeel::PLASButLPLookAndFeel(): PLASButtonLookAndFeel(juce::ImageCache::getFromMemory(BinaryData::ON_LP_Button_png, BinaryData::ON_LP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_LP_Button_png, BinaryData::OFF_LP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::Destroyed_ON_LP_Button_png, BinaryData::Destroyed_ON_LP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_LP_Button_png, BinaryData::OFF_LP_Button_pngSize),false)
{
}
PLASButHPLookAndFeel::PLASButHPLookAndFeel(): PLASButtonLookAndFeel(juce::ImageCache::getFromMemory(BinaryData::ON_HP_Button_png, BinaryData::ON_HP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_HP_Button_png, BinaryData::OFF_HP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::Destroyed_ON_HP_Button_png, BinaryData::Destroyed_ON_HP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_HP_Button_png, BinaryData::OFF_HP_Button_pngSize),false)
{
}


PLASButLRLookAndFeel::PLASButLRLookAndFeel():PLASButtonLookAndFeel(
    
    juce::ImageCache::getFromMemory(BinaryData::LR_Button_png, BinaryData::LR_Button_pngSize),
    juce::ImageCache::getFromMemory(BinaryData::OFF_LR_Button_png, BinaryData::OFF_LR_Button_pngSize),
    juce::ImageCache::getFromMemory(BinaryData::Destroyed_LR_Button_png,BinaryData::Destroyed_LR_Button_pngSize),
                                                                 juce::ImageCache::getFromMemory(BinaryData::OFF_LR_Button_png, BinaryData::OFF_LR_Button_pngSize),false){}

PLASButMSLookAndFeel::PLASButMSLookAndFeel():PLASButtonLookAndFeel( juce::ImageCache::getFromMemory(BinaryData::MS_Button_png, BinaryData::MS_Button_pngSize),
    juce::ImageCache::getFromMemory(BinaryData::OFF_MS_Button_png, BinaryData::OFF_MS_Button_pngSize),
    juce::ImageCache::getFromMemory(BinaryData::Destroyed_MS_Button_png, BinaryData::Destroyed_MS_Button_pngSize),
    juce::ImageCache::getFromMemory(BinaryData::OFF_MS_Button_png, BinaryData::OFF_MS_Button_pngSize),
    false){}


PLASMSLRLookAndFeel::PLASMSLRLookAndFeel(){
    
    lrOnN=juce::ImageCache::getFromMemory(BinaryData::LR_Button_png, BinaryData::LR_Button_pngSize);
    lrOffN=juce::ImageCache::getFromMemory(BinaryData::OFF_LR_Button_png, BinaryData::OFF_LR_Button_pngSize);
    lrOnD=juce::ImageCache::getFromMemory(BinaryData::Destroyed_LR_Button_png,BinaryData::Destroyed_LR_Button_pngSize);
    lrOffD=juce::ImageCache::getFromMemory(BinaryData::OFF_LR_Button_png, BinaryData::OFF_LR_Button_pngSize);
    msOnN = juce::ImageCache::getFromMemory(BinaryData::MS_Button_png, BinaryData::MS_Button_pngSize);
    msOffN = juce::ImageCache::getFromMemory(BinaryData::OFF_MS_Button_png, BinaryData::OFF_MS_Button_pngSize);
    msOnD=juce::ImageCache::getFromMemory(BinaryData::Destroyed_MS_Button_png, BinaryData::Destroyed_MS_Button_pngSize);
    msOffD=juce::ImageCache::getFromMemory(BinaryData::OFF_MS_Button_png, BinaryData::OFF_MS_Button_pngSize);
    destroy = false;
}
void PLASMSLRLookAndFeel::setDestroy(bool val) {
    destroy = val;
}
void PLASMSLRLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    juce::Image btnLR = juce::Image();
    juce::Image btnMS = juce::Image();
    if(!destroy) {
        if (button.getToggleState()) {
            btnMS = msOnN;
            btnLR = lrOffN;
        }
        else {
            btnMS = msOffN;
            btnLR = lrOnN;
        }
    } else {
        if (button.getToggleState()) {
            btnMS = msOnD;
            btnLR = lrOffD;
        }
        else {
            btnMS = msOffD;
            btnLR = lrOnD;
        }
    }
    
    float stripeDimW = (float)button.getWidth();
    float stripeDimH = (float)button.getHeight();
    float btnMSDimW = stripeDimH;
    float btnMSDimH = stripeDimH;
    float btnLRDimW = stripeDimH;
    float btnLRDimH = stripeDimH;
    
    float frameMSDimW = (float)btnMS.getWidth();
    float frameMSDimH = (float)btnMS.getHeight();
    
    float frameLRDimW = (float)btnLR.getWidth();
    float frameLRDimH = (float)btnLR.getHeight();
    
    if ((btnMSDimW/frameMSDimW) <= ( btnMSDimH / frameMSDimH)) {
        btnMSDimH = (btnMSDimW / frameMSDimW) * frameMSDimH;
        btnMSDimW = (btnMSDimW / frameMSDimW) * frameMSDimW;
    }
    else {
        btnMSDimH = (btnMSDimH / frameMSDimH) * frameMSDimH;
        btnMSDimW = (btnMSDimH / frameMSDimH) * frameMSDimW;
    }
    
    if ((btnLRDimW/frameLRDimW) <= ( btnLRDimH / frameLRDimH)) {
        btnLRDimH = (btnLRDimW / frameLRDimW) * frameLRDimH;
        btnLRDimW = (btnLRDimW / frameLRDimW) * frameLRDimW;
    }
    else {
        btnLRDimH = (btnLRDimH / frameLRDimH) * frameLRDimH;
        btnLRDimW = (btnLRDimH / frameLRDimH) * frameLRDimW;
    }
    //int centerX = (button.getWidth()) / 2;
    int centerY = (button.getHeight()) / 2;
    int sMSX = stripeDimW - btnMSDimW;
    int sMSY = centerY - (btnMSDimH / 2);
    int sLRX = 0;
    int sLRY = centerY - (btnLRDimH / 2);
    
    g.drawImage(btnLR, sLRX, sLRY, btnLRDimW, btnLRDimH, 0, 0, frameLRDimW, frameLRDimH);
    g.drawImage(btnMS, sMSX, sMSY, btnMSDimW, btnMSDimH, 0, 0, frameMSDimW, frameMSDimH);
    
}
/*
 void PLASFiltersLookAndFeel::drawComboBox     (     juce::Graphics & g     ,
 int      width,
 int      height,
 bool      isButtonDown,
 int      buttonX,
 int      buttonY,
 int      buttonW,
 int      buttonH,
 juce::ComboBox & cb
 ) {
 juce::Image btnLP;
 juce::Image btnBP;
 juce::Image btnHP;
 
 if(!destroy) {
 if(isOn) {
 if (cb.getSelectedId()==1) {
 btnLP = LPOnN;
 btnBP = BPOffN;
 btnHP = HPOffN;
 
 }
 else if (cb.getSelectedId()==2){
 btnLP = LPOffN;
 btnBP = BPOnN;
 btnHP = HPOffN;
 } else if (cb.getSelectedId()==3){
 btnLP = LPOffN;
 btnBP = BPOffN;
 btnHP = HPOnN;
 }
 
 } else {
 if (cb.getSelectedId()==1) {
 btnLP = LPOnD;
 btnBP = BPOffD;
 btnHP = HPOffD;
 
 }
 else if (cb.getSelectedId()==2){
 btnLP = LPOffD;
 btnBP = BPOnD;
 btnHP = HPOffD;
 } else if (cb.getSelectedId()==3){
 btnLP = LPOffD;
 btnBP = BPOffD;
 btnHP = HPOnD;
 }
 }
 
 } else {
 btnLP = LPOffD;
 btnBP = BPOffD;
 btnHP = HPOffD;
 
 }
 float stripeDimW = width;
 float stripeDimH = height;
 float btnLPDimW = stripeDimH;
 float btnLPDimH = stripeDimH;
 float btnBPDimW = stripeDimH;
 float btnBPDimH = stripeDimH;
 float btnHPDimW = stripeDimH;
 float btnHPDimH = stripeDimH;
 
 
 float frameMSDimW = (float)btnMS.getWidth();
 float frameMSDimH = (float)btnMS.getHeight();
 
 float frameLRDimW = (float)btnLR.getWidth();
 float frameLRDimH = (float)btnLR.getHeight();
 
 if ((btnMSDimW/frameMSDimW) <= ( btnMSDimH / frameMSDimH)) {
 btnMSDimH = (btnMSDimW / frameMSDimW) * frameMSDimH;
 btnMSDimW = (btnMSDimW / frameMSDimW) * frameMSDimW;
 }
 else {
 btnMSDimH = (btnMSDimH / frameMSDimH) * frameMSDimH;
 btnMSDimW = (btnMSDimH / frameMSDimH) * frameMSDimW;
 }
 
 if ((btnLRDimW/frameLRDimW) <= ( btnLRDimH / frameLRDimH)) {
 btnLRDimH = (btnLRDimW / frameLRDimW) * frameLRDimH;
 btnLRDimW = (btnLRDimW / frameLRDimW) * frameLRDimW;
 }
 else {
 btnLRDimH = (btnLRDimH / frameLRDimH) * frameLRDimH;
 btnLRDimW = (btnLRDimH / frameLRDimH) * frameLRDimW;
 }
 //int centerX = (button.getWidth()) / 2;
 int centerY = (button.getHeight()) / 2;
 int sMSX = stripeDimW - btnMSDimW;
 int sMSY = centerY - (btnMSDimH / 2);
 int sLRX = 0;
 int sLRY = centerY - (btnLRDimH / 2);
 
 g.drawImage(btnLR, sLRX, sLRY, btnLRDimW, btnLRDimH, 0, 0, frameLRDimW, frameLRDimH);
 g.drawImage(btnMS, sMSX, sMSY, btnMSDimW, btnMSDimH, 0, 0, frameMSDimW, frameMSDimH);
 
 
 }
 
 void PLASFiltersLookAndFeel::setOn(bool on){
 isOn=on;
 }
 void PLASFiltersLookAndFeel::setDestroy(bool val){
 destroy = val;
 }
 */
