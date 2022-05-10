/*
 ==============================================================================
 
 PlasLookAndFeel.h
 Created: 12 Apr 2022 2:12:55pm
 Author:  Pox17
 
 ==============================================================================
 */

#pragma once
#include <JuceHeader.h>

class PLASLookAndFeel : public juce::LookAndFeel_V4 {
public:
    PLASLookAndFeel();
    void setKnobStripN(juce::Image src);
    void setKnobStripD(juce::Image src);
    void setKnobStripDisabled(juce::Image src);
    void setFrames(int fr);
    void setDestroy(bool dest);
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;
private:
    juce::Image knobStripN;
    juce::Image knobStripD;
    juce::Image knobStripDisabled;
    juce::Image ButtonN;
    juce::Image buttonD;
    int frames;
    bool destroy;
};

class PLASBigKnobLookFeel : public PLASLookAndFeel {
public:
    PLASBigKnobLookFeel();
};
class PLASLittleKnobLookFeel : public PLASLookAndFeel {
public:
    PLASLittleKnobLookFeel();
};


class PLASDestroyButtonLookAndFeel : public juce::LookAndFeel_V4 {
public:
    PLASDestroyButtonLookAndFeel();
    void drawToggleButton(juce::Graphics&, juce::ToggleButton&, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) 	 override;
private:
    juce::Image buttonOn;
    juce::Image buttonOff;
};

class PLASButtonLookAndFeel : public juce::LookAndFeel_V4 {
public:
    PLASButtonLookAndFeel(juce::Image abuttonOnN,
                          juce::Image abuttonOffN,
                          juce::Image abuttonOnD,
                          juce::Image abuttonOffD,
                          bool destroy);
    void drawToggleButton(juce::Graphics&, juce::ToggleButton&, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)      override;
    void setDestroy(bool val);
private:
    juce::Image buttonOnN;
    juce::Image buttonOffN;
    juce::Image buttonOnD;
    juce::Image buttonOffD;
    bool destroy;
};

class PLASMSLRLookAndFeel : public juce::LookAndFeel_V4 {
public:
    PLASMSLRLookAndFeel();
    void drawToggleButton(juce::Graphics&, juce::ToggleButton&, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)      override;
    void setDestroy(bool val);
private:
    juce::Image lrOnN;
    juce::Image lrOffN;
    juce::Image lrOnD;
    juce::Image lrOffD;
    juce::Image msOnN;
    juce::Image msOffN;
    juce::Image msOnD;
    juce::Image msOffD;
    bool destroy;
};




class PLASButLinkLookAndFeel: public PLASButtonLookAndFeel {
public:
    PLASButLinkLookAndFeel();
};
class PLASButOnLookAndFeel: public PLASButtonLookAndFeel {
public:
    PLASButOnLookAndFeel();
};

class PLASButLPLookAndFeel: public PLASButtonLookAndFeel {
public:
    PLASButLPLookAndFeel();
};

class PLASButBPLookAndFeel: public PLASButtonLookAndFeel {
public:
    PLASButBPLookAndFeel();
};
class PLASButHPLookAndFeel: public PLASButtonLookAndFeel {
public:
    PLASButHPLookAndFeel();
};
class PLASButMSLookAndFeel: public PLASButtonLookAndFeel {
public:
    PLASButMSLookAndFeel();
};
class PLASButLRLookAndFeel: public PLASButtonLookAndFeel {
public:
    PLASButLRLookAndFeel();
};
/*
class PLASFiltersLookAndFeel : public juce::LookAndFeel_V4 {
public:
    PLASFiltersLookAndFeel();
    void drawComboBox     (     juce::Graphics & g     ,
            int      width,
            int      height,
            bool      isButtonDown,
            int      buttonX,
            int      buttonY,
            int      buttonW,
            int      buttonH,
            juce::ComboBox & cb
                      );
    void setOn(bool on);
    void setDestroy(bool val);
private:
    juce::Image LPOnN;
    juce::Image LPOffN;
    juce::Image LPOnD;
    juce::Image LPOffD;
    juce::Image BPOnN;
    juce::Image BPOffN;
    juce::Image BPOnD;
    juce::Image BPOffD;
    juce::Image HPOnN;
    juce::Image HPOffN;
    juce::Image HPOnD;
    juce::Image HPOffD;
    bool destroy;
    bool isOn;
};

*/
