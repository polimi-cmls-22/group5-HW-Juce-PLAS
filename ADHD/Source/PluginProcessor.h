/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class ADHDAudioProcessor : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    ADHDAudioProcessor();
    //ADHDAudioProcessor(ADHDAudioProcessor & ADHDCopy);
    
    ~ADHDAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    const auto getTreeState();
    


    juce::AudioProcessorValueTreeState treeState;

    //declaration of variables
    
    float getRMSValue(int inOut, int channel);
    float getMAGValue(int inOut, int channel);
    
private:
    bool link = false;
    int distType {0};
    bool channelOnL{ true };
    bool channelOnR{ true };
    bool isMidSide{ false };
    bool destroy {false};
    float gain[2]{ 1.0, 1.0 };
    float dryWet[2]{ 1.0, 1.0 };
    float volume[2]{ 1.0, 1.0 };
    bool eqOn[2]{ false,false };
    int eqSelect[2]{ 0,0 };
    float eqQ[2]{ 0.1f, 0.1f };
    float eqFreq[2]{ 100.0f, 100.0f };
    float destroyGain = 0.0f;
    int overSampFactor{2};
    int lastSampleRate{44100};
    float toneFreq[2] = { 16000.0f, 16000.0f };

    juce::LinearSmoothedValue<float> rmsLevelInLeft,rmsLevelInRight,rmsLevelOutLeft,rmsLevelOutRight;
    juce::LinearSmoothedValue<float> magLevelInLeft,magLevelInRight,magLevelOutLeft,magLevelOutRight;
    
    juce::AudioBuffer<float> dryBufferL = juce::AudioBuffer<float>();
    juce::AudioBuffer<float> dryBufferR = juce::AudioBuffer<float>();
    
    juce::AudioBuffer<float> bufferL = juce::AudioBuffer<float>();
    juce::AudioBuffer<float> bufferR = juce::AudioBuffer<float>();
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    juce::NormalisableRange<float> freqRange(float min, float max, float interval);
    void updateFilters(int numOfFilter,int type,float freq, float q);
        
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    
    //custom private functions
    //distortion behaviour
    float expQuasiSim(float sample, float gainVal);
    float halfWaveAsDist(float sample,float gainVal);
    float linearMaPoco(float sample, float gainVal);
    float mixedGainDistortion(float sample, float gainVal);
    float destroyDistortion(float sample, float gainVal);
    //dsp modules declaration

    juce::dsp::Oversampling<float> oversamplingModuleL;
    juce::dsp::Oversampling<float> oversamplingModuleR;

    /*juce::dsp::StateVariableFilter::Filter<float> filterL;
    juce::dsp::StateVariableFilter::Filter<float> filterR;*/

    juce::dsp::StateVariableTPTFilter<float> filterL;
    juce::dsp::StateVariableTPTFilter<float> filterR;

    juce::dsp::StateVariableTPTFilter<float> toneFilterL;
    juce::dsp::StateVariableTPTFilter<float> toneFilterR;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADHDAudioProcessor)
    
    
};
