/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#define _USE_MATH_DEFINES

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>
#include <cmath>


//==============================================================================
ADHDAudioProcessor::ADHDAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                 .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                 .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                 ),
treeState(*this, nullptr, "Parameters", createParameters()),
oversamplingModuleL(1, overSampFactor, juce::dsp::Oversampling<float>::FilterType::filterHalfBandFIREquiripple),
oversamplingModuleR(1, overSampFactor, juce::dsp::Oversampling<float>::FilterType::filterHalfBandFIREquiripple)

#endif
{
    treeState.addParameterListener("CHANNELONL", this);
    treeState.addParameterListener("CHANNELONR", this);
    
    treeState.addParameterListener("LINK", this);
    treeState.addParameterListener("DESTROY", this);
    treeState.addParameterListener("MIDSIDE", this);
    treeState.addParameterListener("LEFTRIGHT", this);
    treeState.addParameterListener("GAINL", this);
    treeState.addParameterListener("GAINR", this);
    treeState.addParameterListener("DRYWETL", this);
    treeState.addParameterListener("DRYWETR", this);
    treeState.addParameterListener("VOLUMEL", this);
    treeState.addParameterListener("VOLUMER", this);
    treeState.addParameterListener("EQONL", this);
    treeState.addParameterListener("EQSELECTL", this);
    treeState.addParameterListener("EQLPL", this);
    treeState.addParameterListener("EQBPL", this);
    treeState.addParameterListener("EQHPL", this);
    
    
    treeState.addParameterListener("EQONR", this);
    treeState.addParameterListener("EQSELECTR", this);
    treeState.addParameterListener("EQLPR", this);
    treeState.addParameterListener("EQBPR", this);
    treeState.addParameterListener("EQHPR", this);
    treeState.addParameterListener("QL", this);
    treeState.addParameterListener("FREQL", this);
    treeState.addParameterListener("QR", this);
    treeState.addParameterListener("FREQR", this);
    treeState.addParameterListener("DISTTYPE", this);

    treeState.addParameterListener("DESTROYGAIN", this);

    treeState.addParameterListener("TONEL", this);
    treeState.addParameterListener("TONER", this);
}


ADHDAudioProcessor::~ADHDAudioProcessor()
{
    
    treeState.removeParameterListener("CHANNELONL", this);
    treeState.removeParameterListener("CHANNELONR", this);
    
    
    treeState.removeParameterListener("LEFTRIGHT", this);
    treeState.removeParameterListener("LINK", this);
    treeState.removeParameterListener("DESTROY", this);
    treeState.removeParameterListener("MIDSIDE", this);
    treeState.removeParameterListener("GAINL", this);
    treeState.removeParameterListener("GAINR", this);
    treeState.removeParameterListener("DRYWETL", this);
    treeState.removeParameterListener("DRYWETR", this);
    treeState.removeParameterListener("VOLUMEL", this);
    treeState.removeParameterListener("VOLUMER", this);
    treeState.removeParameterListener("EQONL", this);
    treeState.removeParameterListener("EQSELECTL", this);
    treeState.addParameterListener("EQLPL", this);
    treeState.addParameterListener("EQBPL", this);
    treeState.addParameterListener("EQHPL", this);
    
    treeState.removeParameterListener("EQONR", this);
    treeState.removeParameterListener("EQSELECTR", this);
    
    treeState.removeParameterListener("EQLPR", this);
    treeState.removeParameterListener("EQBPR", this);
    treeState.removeParameterListener("EQHPR", this);
    treeState.removeParameterListener("QL", this);
    treeState.removeParameterListener("EQFREQL", this);
    treeState.removeParameterListener("QR", this);
    treeState.removeParameterListener("DISTTYPE", this);
    treeState.removeParameterListener("FREQR", this);

    treeState.removeParameterListener("DESTROYGAIN", this);

    treeState.removeParameterListener("TONEL", this);
    treeState.removeParameterListener("TONER", this);

}

//==============================================================================
const juce::String ADHDAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ADHDAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool ADHDAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool ADHDAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double ADHDAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ADHDAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int ADHDAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ADHDAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ADHDAudioProcessor::getProgramName (int index)
{
    return {};
}

void ADHDAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ADHDAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    //initialization of the dsp module in order to process the audio in input to the plugin
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    lastSampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    dryBufferL = juce::AudioBuffer<float>(1, (int)(samplesPerBlock * pow(2, overSampFactor)));
    dryBufferR = juce::AudioBuffer<float>(1, (int)(samplesPerBlock * pow(2, overSampFactor)));
    bufferL = juce::AudioBuffer<float>(1, (int)(samplesPerBlock ));
    bufferR = juce::AudioBuffer<float>(1, (int)(samplesPerBlock ));
    //initialization of the oversampling block specifying the maximum num of samples per block
    oversamplingModuleL.initProcessing(samplesPerBlock);
    oversamplingModuleR.initProcessing(samplesPerBlock);
    
    filterL.reset();
    filterR.reset();
    toneFilterL.reset();
    toneFilterR.reset();
    filterL.prepare(spec);
    filterR.prepare(spec);
    toneFilterL.prepare(spec);
    toneFilterR.prepare(spec);

    // FILTERS
    toneFilterL.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    toneFilterR.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    
    toneFilterL.setResonance(0.5f);
    toneFilterR.setResonance(0.5f);

    toneFilterL.setCutoffFrequency(toneFreq[0]);
    toneFilterR.setCutoffFrequency(toneFreq[1]);
    
    rmsLevelInLeft.reset(sampleRate* pow(2, overSampFactor), 0.5);
    rmsLevelInRight.reset(sampleRate* pow(2, overSampFactor), 0.5);
    rmsLevelOutLeft.reset(sampleRate, 0.5);
    rmsLevelOutRight.reset(sampleRate, 0.5);
    
    rmsLevelInLeft.setCurrentAndTargetValue(-60.0f);
    rmsLevelInRight.setCurrentAndTargetValue(-60.0f);
    rmsLevelOutLeft.setCurrentAndTargetValue(-60.0f);
    rmsLevelOutRight.setCurrentAndTargetValue(-60.0f);
    
    magLevelInLeft.reset(sampleRate* pow(2, overSampFactor), 0.2);
    magLevelInRight.reset(sampleRate* pow(2, overSampFactor), 0.2);
    magLevelOutLeft.reset(sampleRate, 0.2);
    magLevelOutRight.reset(sampleRate, 0.2);
    
    magLevelInLeft.setCurrentAndTargetValue(-60.0f);
    magLevelInRight.setCurrentAndTargetValue(-60.0f);
    magLevelOutLeft.setCurrentAndTargetValue(-60.0f);
    magLevelOutRight.setCurrentAndTargetValue(-60.0f);
    
}

void ADHDAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ADHDAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    
    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif
    
    return true;
#endif
}
#endif




bool destroy = false;

void ADHDAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    bool isInternalMidSide = isMidSide;
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    
    
    
    //multi channel audio block that englobes the audio buffer used as audio source before oversampling
    
    
    //juce::dsp::AudioBlock<float> overSBlockEq(buffer);
    
    
    
    
    for (int sample = 0; sample < bufferL.getNumSamples(); sample++) {
        bufferL.getWritePointer(0)[sample] = buffer.getReadPointer(0)[sample];
        bufferR.getWritePointer(0)[sample] = buffer.getReadPointer(1)[sample];
        
    }
    
    juce::dsp::AudioBlock<float> overSBlockL(buffer);
    juce::dsp::AudioBlock<float> overSBlockR(buffer);
    juce::dsp::AudioBlock<float> srcBlockL(bufferL);
    juce::dsp::AudioBlock<float> srcBlockR(bufferR);
    //oversampling the audio signal
    overSBlockL = oversamplingModuleL.processSamplesUp(srcBlockL);
    overSBlockR = oversamplingModuleR.processSamplesUp(srcBlockR);
    
    
    
    //Mid-Side encoding
    if (isInternalMidSide) {
        for (int sample = 0; sample < overSBlockL.getNumSamples(); sample++) {
            float* dataL = overSBlockL.getChannelPointer(0);
            float* dataR = overSBlockR.getChannelPointer(0);
            
            float dataMid = dataL[sample] + dataR[sample];
            float dataSide = dataL[sample] - dataR[sample];
            
            // giusto?
            dataL[sample] = dataMid * sqrt(2) / 2;
            dataR[sample] = dataSide * sqrt(2) / 2;
            
        }
    }
    
    
    juce::dsp::AudioBlock<float> oversDryBlockL(dryBufferL);
    juce::dsp::AudioBlock<float> oversDryBlockR(dryBufferR);
    
    // drycopy save before distortion
    //for (int ch = 0; ch < overSBlock.getNumChannels(); ++ch) {
    float* dataL = overSBlockL.getChannelPointer(0);
    float* dryDataCopyL = oversDryBlockL.getChannelPointer(0);
    float* dataR = overSBlockR.getChannelPointer(0);
    float* dryDataCopyR = oversDryBlockR.getChannelPointer(0);
    
    for (int sample = 0; sample < overSBlockL.getNumSamples(); sample++) {
        dryDataCopyL[sample] = dataL[sample];
        dryDataCopyR[sample] = dataR[sample];
        
        
    }
    
    
    
    
    rmsLevelInLeft.skip(dryBufferL.getNumSamples());
    rmsLevelInRight.skip(dryBufferR.getNumSamples());
    {
        const auto value= juce::Decibels::gainToDecibels(dryBufferL.getRMSLevel(0, 0, dryBufferL.getNumSamples()));
        if(value < rmsLevelInLeft.getCurrentValue())
            rmsLevelInLeft.setTargetValue(value);
        else
            rmsLevelInLeft.setCurrentAndTargetValue(value);
    }
    
    {
        const auto value= juce::Decibels::gainToDecibels(dryBufferR.getRMSLevel(0, 0, dryBufferR.getNumSamples()));
        if(value < rmsLevelInRight.getCurrentValue())
            rmsLevelInRight.setTargetValue(value);
        else
            rmsLevelInRight.setCurrentAndTargetValue(value);
    }
    
    magLevelInLeft.skip(dryBufferL.getNumSamples());
    magLevelInRight.skip(dryBufferR.getNumSamples());
    {
        const auto value= juce::Decibels::gainToDecibels(dryBufferL.getMagnitude(0, 0, dryBufferL.getNumSamples()));
        if(value < magLevelInLeft.getCurrentValue())
            magLevelInLeft.setTargetValue(value);
        else
            magLevelInLeft.setCurrentAndTargetValue(value);
    }
    
    {
        const auto value= juce::Decibels::gainToDecibels(dryBufferR.getMagnitude(0, 0, dryBufferR.getNumSamples()));
        if(value < magLevelInRight.getCurrentValue())
            magLevelInRight.setTargetValue(value);
        else
            magLevelInRight.setCurrentAndTargetValue(value);
    }
    
    
    //EQ BLOCK R
    if (eqOn[0]) {
        filterL.process(juce::dsp::ProcessContextReplacing <float>(overSBlockL));
    }
    if (eqOn[1]) {
        filterR.process(juce::dsp::ProcessContextReplacing <float>(overSBlockR));
    }
    
    
    
    
    
    // distortion
    //        for (int ch = 0; ch < overSBlock.getNumChannels(); ++ch) {
    float* dataDistL = overSBlockL.getChannelPointer(0);
    float* dataDistR = overSBlockR.getChannelPointer(0);
    
    for (int sample = 0; sample < overSBlockL.getNumSamples(); sample++) {
        if(destroy) {
            //placeholder to insert the destroy function
            dataDistL[sample] = dataDistL[sample] * gain[0];
            dataDistR[sample] = dataDistR[sample] * gain[1];
            dataDistL[sample] = destroyDistortion(dataDistL[sample], destroyGain);//halfWaveAsDist(data[sample], gain[ch]);
            dataDistR[sample] = destroyDistortion(dataDistR[sample], destroyGain);//halfWaveAsDist(data[sample], gain[ch]);
        } else {
            
            dataDistL[sample] = mixedGainDistortion(dataDistL[sample], gain[0]);//halfWaveAsDist(data[sample], gain[ch]);
            dataDistR[sample] = mixedGainDistortion(dataDistR[sample], gain[1]);//halfWaveAsDist(data[sample], gain[ch]);
        }
        
        /*
         if (distType == 0) {
         dataDistL[sample] = halfWaveAsDist(dataDistL[sample], gain[0]);//halfWaveAsDist(data[sample], gain[ch]);
         dataDistR[sample] = halfWaveAsDist(dataDistR[sample], gain[1]);//halfWaveAsDist(data[sample], gain[ch]);
         }
         else if (distType == 1) {
         dataDistL[sample] = expQuasiSim(dataDistL[sample], gain[0]);//halfWaveAsDist(data[sample], gain[ch]);
         dataR[sample] = expQuasiSim(dataDistR[sample], gain[1]);//halfWaveAsDist(data[sample], gain[ch]);
         
         }
         else if (distType == 2) {
         dataL[sample] = linearMaPoco(dataDistL[sample], gain[0]);//halfWaveAsDist(data[sample], gain[ch]);
         dataR[sample] = linearMaPoco(dataDistR[sample], gain[1]);//halfWaveAsDist(data[sample], gain[ch]);
         }
         */
    }
    //      }
    
    // tone processing
    toneFilterL.process(juce::dsp::ProcessContextReplacing <float>(overSBlockL));
    toneFilterR.process(juce::dsp::ProcessContextReplacing <float>(overSBlockR));

    
    //volume application

    overSBlockL.multiplyBy(volume[0]);
    overSBlockR.multiplyBy(volume[1]);
    //oversDryBlockL.multiplyBy(volume[0]);
    //oversDryBlockR.multiplyBy(volume[1]);


    //parallel drywet channels Sum
    //for (int ch = 0; ch < overSBlock.getNumChannels(); ++ch) {
    float* dataDwL = overSBlockL.getChannelPointer(0);
    float* dryDataCopyDwL = oversDryBlockL.getChannelPointer(0);
    float* dataDwR = overSBlockR.getChannelPointer(0);
    float* dryDataCopyDwR = oversDryBlockR.getChannelPointer(0);
    if(!channelOnL){
        dryWet[0]=0;
    } else{
        dryWet[0]=*treeState.getRawParameterValue("DRYWETL");
    }
    if(!channelOnR){
        dryWet[1]=0;
    } else{
        dryWet[1]=*treeState.getRawParameterValue("DRYWETR");
    }
    for (int sample = 0; sample < overSBlockL.getNumSamples(); sample++) {
        dataDwL[sample] = (dryWet[0] * dataDwL[sample]) + ((1.0f - dryWet[0]) * dryDataCopyDwL[sample]);
        dataDwR[sample] = (dryWet[1] * dataDwR[sample]) + ((1.0f - dryWet[1]) * dryDataCopyDwR[sample]);
    }
    //}
    
    /*
     for (int ch = 0; ch < overSBlock.getNumChannels(); ++ch) {
     float* data = overSBlock.getChannelPointer(ch);
     //float* dryDataCopy = oversDryBlock.getChannelPointer(ch);
     for (int sample = 0; sample < overSBlock.getNumSamples(); sample++) {
     
     data[sample] = data[sample] * volume[ch];
     
     }
     }
     */
    //mid-side decoding
    
    if (isInternalMidSide) {
        for (int sample = 0; sample < overSBlockL.getNumSamples(); sample++) {
            float* dataMid = overSBlockL.getChannelPointer(0);
            float* dataSide = overSBlockR.getChannelPointer(0);
            
            float dataL = dataMid[sample] + dataSide[sample];
            float dataR = dataMid[sample] - dataSide[sample];
            
            dataMid[sample] = dataL / sqrt(2);
            dataSide[sample] = dataR / sqrt(2);
            
            
        }
    }
    
    
    
    //downsampling
    oversamplingModuleL.processSamplesDown(srcBlockL);
    oversamplingModuleR.processSamplesDown(srcBlockR);
    
    
    
    buffer.clear(0, 0, buffer.getNumSamples());
    buffer.clear(1, 0, buffer.getNumSamples());
    for (int sample = 0; sample < bufferL.getNumSamples(); sample++) {
        buffer.getWritePointer(0)[sample] = bufferL.getReadPointer(0)[sample];
        buffer.getWritePointer(1)[sample] = bufferR.getReadPointer(0)[sample];
    }
    rmsLevelOutLeft.skip(bufferL.getNumSamples());
    rmsLevelOutRight.skip(bufferR.getNumSamples());
    {
        const auto value= juce::Decibels::gainToDecibels(bufferL.getRMSLevel(0, 0, bufferL.getNumSamples()));
        if(value < rmsLevelOutLeft.getCurrentValue())
            rmsLevelOutLeft.setTargetValue(value);
        else
            rmsLevelOutLeft.setCurrentAndTargetValue(value);
    }
    
    {
        const auto value= juce::Decibels::gainToDecibels(bufferR.getRMSLevel(0, 0, bufferR.getNumSamples()));
        if(value < rmsLevelOutRight.getCurrentValue())
            rmsLevelOutRight.setTargetValue(value);
        else
            rmsLevelOutRight.setCurrentAndTargetValue(value);
    }
    
    
    magLevelOutLeft.skip(bufferL.getNumSamples());
    magLevelOutRight.skip(bufferR.getNumSamples());
    {
        const auto value= juce::Decibels::gainToDecibels(bufferL.getMagnitude(0, 0, bufferL.getNumSamples()));
        if(value < magLevelOutLeft.getCurrentValue())
            magLevelOutLeft.setTargetValue(value);
        else
            magLevelOutLeft.setCurrentAndTargetValue(value);
    }
    
    {
        const auto value= juce::Decibels::gainToDecibels(bufferR.getMagnitude(0, 0, bufferR.getNumSamples()));
        if(value < magLevelOutRight.getCurrentValue())
            magLevelOutRight.setTargetValue(value);
        else
            magLevelOutRight.setCurrentAndTargetValue(value);
    }
    
    
}

float ADHDAudioProcessor::halfWaveAsDist(float sample, float gainVal) {
    if (sample > 0) {
        sample = 1 - exp(-abs(sample * gainVal));
    }
    else {
        sample = 0;
    }
    
    return sample;
}

void ADHDAudioProcessor::updateFilters(int numOfFilter, int type, float q, float freq)
{
    if (numOfFilter == 0) {
        if (type == 0) {
            /*filterL.parameters->type = juce::dsp::
            StateVariableFilter::Parameters<float>::Type::lowPass;
            filterL.parameters->setCutOffFrequency(lastSampleRate, freq, q);*/
            filterL.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
            filterL.setCutoffFrequency(freq);
            filterL.setResonance(q);
        } else if(type==1){
            /*filterL.parameters->type = juce::dsp::
            StateVariableFilter::Parameters<float>::Type::bandPass;
            filterL.parameters->setCutOffFrequency(lastSampleRate, freq, q);*/
            filterL.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
            filterL.setCutoffFrequency(freq);
            filterL.setResonance(q);
        }
        else if (type == 2) {
            /*filterL.parameters->type = juce::dsp::
            StateVariableFilter::Parameters<float>::Type::highPass;
            filterL.parameters->setCutOffFrequency(lastSampleRate, freq, q);*/
            filterL.setType(juce::dsp::StateVariableTPTFilterType::highpass);
            filterL.setCutoffFrequency(freq);
            filterL.setResonance(q);
        }
    } else if (numOfFilter == 1) {
        if (type == 0) {
            /*filterR.parameters->type = juce::dsp::
            StateVariableFilter::Parameters<float>::Type::lowPass;
            filterR.parameters->setCutOffFrequency(lastSampleRate, freq, q);*/
            filterR.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
            filterR.setCutoffFrequency(freq);
            filterR.setResonance(q);
        }
        else if (type == 1) {
            /*filterR.parameters->type = juce::dsp::
            StateVariableFilter::Parameters<float>::Type::bandPass;
            filterR.parameters->setCutOffFrequency(lastSampleRate, freq, q);*/
            filterR.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
            filterR.setCutoffFrequency(freq);
            filterR.setResonance(q);
        }
        else if (type == 2) {
            /*filterR.parameters->type = juce::dsp::
            StateVariableFilter::Parameters<float>::Type::highPass;
            filterR.parameters->setCutOffFrequency(lastSampleRate, freq, q);*/
            filterR.setType(juce::dsp::StateVariableTPTFilterType::highpass);
            filterR.setCutoffFrequency(freq);
            filterR.setResonance(q);
        }
    }
}

float ADHDAudioProcessor::expQuasiSim(float sample, float gainVal)
{
    if (sample > 0) {
        sample = 1 - exp(-abs(sample *0.1*gainVal));
    }
    else {
        sample = 0.3f*(1 - exp(+abs(sample * 0.1 * gainVal)));
    }
    return sample;
}

float ADHDAudioProcessor::linearMaPoco(float sample, float gainVal) {
    if (sample > 0) {
        sample = 1 - exp(-abs(sample * 0.1 * gainVal));
    }
    else {
        sample = -0.5*(1-exp(-abs(sample * 0.1 * gainVal)));
    }
    return sample;
}

float ADHDAudioProcessor::mixedGainDistortion(float sample, float gainVal) {
    float sampleX, sampleY;
    if (sample > 0) {
        sampleX = 1 - exp(-abs((gainVal + 1) * 5 * sample));
        sampleY = sample;
    }

    else {
        sampleX = -(1 - exp(-abs((gainVal + 1) * 3 * sample))) * 0.5;
        sampleY = sample;
    }

    sample = gainVal * sampleX + (1 - gainVal) * sampleY;

    return sample;

}

float ADHDAudioProcessor::destroyDistortion(float sample, float gainVal) {
    float distSample;
    distSample = sin((1 + gainVal) * sample * M_PI) + 0.2 * sin(pow(gainVal, 2) * sample * M_PI);
    if (distSample < -1) distSample = -1;
    if (distSample > 1) distSample = 1;

    return distSample;
}
//==============================================================================
bool ADHDAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ADHDAudioProcessor::createEditor()
{
    //this is commented only for the purpose of a fast parameter layout
    //for custom gui return the specific one not the generic one
    
    
    return new ADHDAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void ADHDAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void ADHDAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (treeState.state.getType()))
            treeState.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ADHDAudioProcessor();
}

//Adding the function used to create parameters in the valuetreeState
juce::AudioProcessorValueTreeState::ParameterLayout ADHDAudioProcessor::createParameters() {
    
    std::vector<std::unique_ptr<juce::RangedAudioParameter >> parameters;
    
    //reserving the space for the number of parameters we want to create
    parameters.reserve(40);
    
    auto bypassLVal = std::make_unique<juce::AudioParameterBool>("CHANNELONL", "channel ON L", true);
    parameters.push_back(std::move(bypassLVal));
    auto bypassRVal = std::make_unique<juce::AudioParameterBool>("CHANNELONR", "channel ON R", true);
    parameters.push_back(std::move(bypassRVal));
    
    auto linkVal = std::make_unique<juce::AudioParameterBool>("LINK", "link",false);
    parameters.push_back(std::move(linkVal));
    
    
    const char* msSettingsCh[2] = { "LeftRight", "MidSide"};
    const juce::StringArray msSettings(msSettingsCh, 2);
    auto midSideVal = std::make_unique<juce::AudioParameterBool>("MIDSIDE", "MidSide",false);
    parameters.push_back(std::move(midSideVal));
    auto leftRightVal = std::make_unique<juce::AudioParameterBool>("LEFTRIGHT", "LeftRight",true);
    parameters.push_back(std::move(leftRightVal));
    auto destroyVal = std::make_unique<juce::AudioParameterBool>("DESTROY", "destroy",false);
    parameters.push_back(std::move(destroyVal));
    
    auto gainValL = std::make_unique<juce::AudioParameterFloat>("GAINL", "Drive Gain L/MID", 0.0f, 1.0f, 0.5f);
    parameters.push_back(std::move(gainValL));
    auto gainValR = std::make_unique<juce::AudioParameterFloat>("GAINR", "Drive Gain R/SIDE", 0.0f, 1.0f, 0.5f);
    parameters.push_back(std::move(gainValR));
    auto dryWetValL = std::make_unique<juce::AudioParameterFloat> ("DRYWETL", "Dry Wet L/MID", 0.0f, 1.0f, 1.0f);
    parameters.push_back(std::move(dryWetValL));
    auto dryWetValR = std::make_unique<juce::AudioParameterFloat>("DRYWETR", "Dry Wet R/SIDE", 0.0f, 1.0f, 1.0f);
    parameters.push_back(std::move(dryWetValR));
    auto volumeValL = std::make_unique<juce::AudioParameterFloat>("VOLUMEL", "volume L/MID", 0.0f, 1.5f, 1.0f);
    parameters.push_back(std::move(volumeValL));
    auto volumeValR = std::make_unique<juce::AudioParameterFloat>("VOLUMER", "volume R/SIDE", 0.0f, 1.5f, 1.0f);
    parameters.push_back(std::move(volumeValR));
    
    const char* eqSettingsCh[3] = { "lowPass", "bandPass","highPass"};
    const juce::StringArray eqSettings(eqSettingsCh, 3);
    auto eqOnL = std::make_unique<juce::AudioParameterBool>("EQONL", "eq On L", false);
    parameters.push_back(std::move(eqOnL));
    auto eqSelectL = std::make_unique<juce::AudioParameterChoice>("EQSELECTL","eq select L",eqSettings,0);
    parameters.push_back(std::move(eqSelectL));
    
    auto eqHPL = std::make_unique<juce::AudioParameterBool>("EQHPL", "eq Hp L", false);
    parameters.push_back(std::move(eqHPL));
    auto eqBPL = std::make_unique<juce::AudioParameterBool>("EQBPL", "eq Bp L", false);
    parameters.push_back(std::move(eqBPL));
    auto eqLPL = std::make_unique<juce::AudioParameterBool>("EQLPL", "eq Lp L", true);
    parameters.push_back(std::move(eqLPL));
    
    
    
    
    auto eqQL = std::make_unique<juce::AudioParameterFloat> ("QL", "Q factor L", 0.1f, 2.0f, 1/sqrt(2));
    parameters.push_back(std::move(eqQL));
    /*auto freqEqL = std::make_unique<juce::AudioParameterFloat> ("FREQL", "CutOff freq L", 20.0f, 20000.0f, 100.0f);
    parameters.push_back(std::move(freqEqL));*/

    auto freqEqL = std::make_unique<juce::AudioParameterFloat>("FREQL", "CutOff freq L", 0.0f, 1.0f, 0.5f);
    parameters.push_back(std::move(freqEqL));

    auto eqOnR = std::make_unique<juce::AudioParameterBool>("EQONR", "eq On R", false);
    parameters.push_back(std::move(eqOnR));
    auto eqSelectR = std::make_unique<juce::AudioParameterChoice>("EQSELECTR", "eq select R", eqSettings, 0);
    parameters.push_back(std::move(eqSelectR));
    
    auto eqHPR = std::make_unique<juce::AudioParameterBool>("EQHPR", "eq Hp R", false);
    parameters.push_back(std::move(eqHPR));
    auto eqBPR = std::make_unique<juce::AudioParameterBool>("EQBPR", "eq Bp R", false);
    parameters.push_back(std::move(eqBPR));
    auto eqLPR = std::make_unique<juce::AudioParameterBool>("EQLPR", "eq Lp R", true);
    parameters.push_back(std::move(eqLPR));
    
    auto eqQR = std::make_unique<juce::AudioParameterFloat>("QR", "Q factor R", 0.1f, 2.0f, 1/sqrt(2));
    parameters.push_back(std::move(eqQR));
    //auto freqEqR = std::make_unique<juce::AudioParameterFloat>("FREQR", "CutOff freq R", 20.0f, 20000.0f, 100.0f);
    //parameters.push_back(std::move(freqEqR));

    auto freqEqR = std::make_unique<juce::AudioParameterFloat>("FREQR", "CutOff freq R", 0.0f, 1.0f, 0.5f);
    parameters.push_back(std::move(freqEqR));
    
    const char* distTypeList[3] = { "half_wave", "expQuasiSim","linearMaPoco" };
    const juce::StringArray distTypeStr(distTypeList, 3);
    auto distTypeSel = std::make_unique<juce::AudioParameterChoice>("DISTTYPE", "distType", distTypeStr, 0);
    parameters.push_back(std::move(distTypeSel));

    auto destroyGain = std::make_unique<juce::AudioParameterFloat>("DESTROYGAIN", "Gain of Destroy Section", 0.0f, 1.0f, 0.f);
    parameters.push_back(std::move(destroyGain));

    auto toneFreqL = std::make_unique<juce::AudioParameterFloat>("TONEL", "Tone frequency L", 0.0f, 1.0f, 1.0f);
    parameters.push_back(std::move(toneFreqL));

    auto toneFreqR = std::make_unique<juce::AudioParameterFloat>("TONER", "Tone frequency R", 0.0f, 1.0f, 1.0f);
    parameters.push_back(std::move(toneFreqR));
    
    return { parameters.begin(),parameters.end() };
}


void ADHDAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue) {
    if (parameterID == "GAINL") {
        gain[0] = newValue;
    }else if (parameterID == "GAINR") {
        gain[1] = newValue;
    }else if (parameterID == "LINK") {
        link = (bool)newValue;
    }
    else if (parameterID == "DRYWETL") {
        dryWet[0] = newValue;
    }
    else if (parameterID == "DRYWETR") {
        dryWet[1] = newValue;
    }
    else if (parameterID == "VOLUMEL") {
        volume[0] = newValue;
    }
    else if (parameterID == "VOLUMER") {
        volume[1] = newValue;
    }
    else if (parameterID == "MIDSIDE") {
        isMidSide=(bool)newValue;
    }
    else if (parameterID == "LEFTRIGHT") {
        isMidSide=!(bool)newValue;
    }
    else if (parameterID == "DESTROY") {
        destroy = (bool)newValue;
    }
    else if (parameterID == "CHANNELONL") {
        channelOnL = (bool)newValue;
    } else if (parameterID == "CHANNELONR") {
        channelOnR = (bool)newValue;
    }
    else if (parameterID == "EQONL") {
        eqOn[0] = (bool)newValue;
    }
    else if (parameterID == "EQONR") {
        eqOn[1] = (bool)newValue;
    }
    else if (parameterID == "QL") {
        eqQ[0] = newValue;
        updateFilters(0, eqSelect[0], eqQ[0], eqFreq[0]);
    }
    else if (parameterID == "QR") {
        eqQ[1] = newValue;
        updateFilters(1, eqSelect[1], eqQ[1], eqFreq[1]);
    }
    else if (parameterID == "FREQL") {
        //eqFreq[0] = newValue;
        eqFreq[0] = juce::mapToLog10(newValue, 10.0f, 16000.0f);
        updateFilters(0, eqSelect[0], eqQ[0], eqFreq[0]);
    }
    else if (parameterID == "FREQR") {
        //eqFreq[1] = newValue;
        eqFreq[1] = juce::mapToLog10(newValue, 10.0f, 16000.0f);
        updateFilters(1, eqSelect[1], eqQ[1], eqFreq[1]);
    }
    else if (parameterID == "EQSELECTL") {
        eqSelect[0] = newValue;
        updateFilters(0, eqSelect[0], eqQ[0], eqFreq[0]);

    } else if(parameterID == "EQLPL") {
        eqSelect[0] = 0;
        updateFilters(0, eqSelect[0], eqQ[0], eqFreq[0]);
        
    }else if(parameterID == "EQBPL") {
        eqSelect[0] = 1;
        updateFilters(0, eqSelect[0], eqQ[0], eqFreq[0]);
        
    }else if(parameterID == "EQHPL") {
        eqSelect[0] = 2;
        updateFilters(0, eqSelect[0], eqQ[0], eqFreq[0]);
        
    }
    else if (parameterID == "EQSELECTR") {
        eqSelect[1] = newValue;
        updateFilters(1, eqSelect[1], eqQ[1], eqFreq[1]);
    }
    else if(parameterID == "EQLPR") {
        eqSelect[1] = 0;
        updateFilters(1, eqSelect[1], eqQ[1], eqFreq[1]);
        
    }else if(parameterID == "EQBPR") {
        eqSelect[1] = 1;
        updateFilters(1, eqSelect[1], eqQ[1], eqFreq[1]);
        
    }
    else if (parameterID == "EQHPR") {
        eqSelect[1] = 2;
        updateFilters(1, eqSelect[1], eqQ[1], eqFreq[1]);

    }
    else if (parameterID == "DESTROYGAIN") {
        destroyGain = juce::jmap(newValue, 1.0f, 5.0f);
    }
    else if (parameterID == "TONEL") {
        toneFreq[0] = juce::mapToLog10(newValue, 100.0f, 15000.0f);
        toneFilterL.setCutoffFrequency(toneFreq[0]);
    }
    else if (parameterID == "TONER") {
        toneFreq[1] = juce::mapToLog10(newValue, 100.0f, 15000.0f);
        toneFilterR.setCutoffFrequency(toneFreq[1]);
    }
    else if (parameterID == "DISTTYPE") {
        distType = newValue;
    }
}


float ADHDAudioProcessor::getRMSValue(int inOut,int channel) {
    jassert(inOut==0 || inOut == 1);
    jassert(channel==0 || channel == 1);
    if(inOut==0){
        if(channel==0 ) {
            return rmsLevelInLeft.getCurrentValue();
        } else if(channel == 1) {
            return rmsLevelInRight.getCurrentValue();
        } else {
            return 0.0f;
        }
    } else if(inOut==1){
        if(channel==0 ) {
            return rmsLevelOutLeft.getCurrentValue();
        } else if(channel == 1) {
            return rmsLevelOutRight.getCurrentValue();
        } else {
            return 0.0f;
        }
    } else
        return 0.0f;
}

float ADHDAudioProcessor::getMAGValue(int inOut,int channel) {
    jassert(inOut==0 || inOut == 1);
    jassert(channel==0 || channel == 1);
    if(inOut==0){
        if(channel==0 ) {
            return magLevelInLeft.getCurrentValue();
        } else if(channel == 1) {
            return magLevelInRight.getCurrentValue();
        } else {
            return 0.0f;
        }
    } else if(inOut==1){
        if(channel==0 ) {
            return magLevelOutLeft.getCurrentValue();
        } else if(channel == 1) {
            return magLevelOutRight.getCurrentValue();
        } else {
            return 0.0f;
        }
    } else
        return 0.0f;
}


const auto ADHDAudioProcessor::getTreeState(){
    return treeState.copyState();
}
/*
ADHDAudioProcessor::ADHDAudioProcessor(ADHDAudioProcessor & ADHDCopy):
    ADHDAudioProcessor::ADHDAudioProcessor()
{
    const auto treeCopy = ADHDCopy.getTreeState();
    treeState.replaceState(treeCopy);
}
 */
