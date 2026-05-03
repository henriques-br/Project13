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
class Project13AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Project13AudioProcessor();
    ~Project13AudioProcessor() override;

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

    //==============================================================================
    // AHENRIQU: Started programming for Project13
    // basic dsp (not prepared or used)
    enum class DSP_Option
    {
        Phase,
        Chorus,
        Overdrive,
        LadderFilter,
        END_OF_LIST
    };
    
private:
    //==============================================================================
    // AHENRIQU: Started programming for Project13
    // https://www.youtube.com/watch?v=fmrLo9OF8bE&list=PLL4Pwd7fD7npBIpgp6Hwzod1PkjsbI0dW&index=54
    // 25:08
    juce::dsp::Phaser<float> phaser;
    juce::dsp::Chorus<float> chorus;
    juce::dsp::LadderFilter<float> overdrive, ladderFilter;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Project13AudioProcessor)
};
