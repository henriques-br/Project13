/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <Fifo.h>

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
        GeneralFilter,
        END_OF_LIST
    };
    
    using DSP_Order = std::array<DSP_Option, static_cast<size_t>(DSP_Option::END_OF_LIST)>;
    SimpleMBComp::Fifo<DSP_Order> dspOrderFifo;
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Settings", createParameterLayout() };
    
    /*
     Phaser
     Rate: hz
     Depth: 0 to 1
     Center freq: hz
     Feedback: -1 to +1
     Mix: 0 to 1
     */
    juce::AudioParameterFloat* phaserRateHz = nullptr;
    juce::AudioParameterFloat* phaserDepthPercent = nullptr;
    juce::AudioParameterFloat* phaserCenterFreqHz = nullptr;
    juce::AudioParameterFloat* phaserFeedbackPercent = nullptr;
    juce::AudioParameterFloat* phaserMixPercent = nullptr;
    juce::AudioParameterBool*  phaserBypass = nullptr;
    
    /*
     Chorus
     Rate: hz
     Depth: 0 to 1
     Center delay: ms (1 to 100)
     Feedback: -1 to +1
     Mix: 0 to 1
     */
    juce::AudioParameterFloat* chorusRateHz = nullptr;
    juce::AudioParameterFloat* chorusDepthPercent = nullptr;
    juce::AudioParameterFloat* chorusCenterDelayMs = nullptr;
    juce::AudioParameterFloat* chorusFeedbackPercent = nullptr;
    juce::AudioParameterFloat* chorusMixPercent = nullptr;
    juce::AudioParameterBool*  chorusBypass = nullptr;
    
    /*
     Overdrive
     Drive: 1 to 100
     */
    juce::AudioParameterFloat* overdriveSaturation = nullptr;
    juce::AudioParameterBool* overdriveBypass = nullptr;
    
    /*
     Ladder filter
     mode: LadderFilterMode enum (int)
     cutoff: hz
     resonance: 0 to 1
     drive: 1 - 100
     */
    juce::AudioParameterChoice* ladderFilterMode = nullptr;
    juce::AudioParameterFloat* ladderFilterCutoffHz = nullptr;
    juce::AudioParameterFloat* ladderFilterResonance = nullptr;
    juce::AudioParameterFloat* ladderFilterDrive = nullptr;
    juce::AudioParameterBool*  ladderFilterBypass = nullptr;
    
    /*
     general filter: https://docs.juce.com/develop/structdsp_1_1IIR_1_1Coefficients.html
     Mode: Peak, bandpass, notch, allpass,
     freq: 20hz - 20,000hz in 1hz steps
     Q: 0.1 - 10 in 0.5 steps
     gain: -24db to +24db in 0.5db increments
     */
    juce::AudioParameterChoice* generalFilterMode = nullptr;
    juce::AudioParameterFloat*  generalFilterFreqHz = nullptr;
    juce::AudioParameterFloat*  generalFilterQuality = nullptr;
    juce::AudioParameterFloat*  generalFilterGain = nullptr;
    juce::AudioParameterBool*  generalFilterBypass = nullptr;

private:
    //==============================================================================
    // AHENRIQU: Started programming for Project13
    // https://www.youtube.com/watch?v=fmrLo9OF8bE
    // contine from https://www.youtube.com/watch?v=fmrLo9OF8bE&t=4483s
    // 8. Overdrive
    DSP_Order dspOrder;
    
    template<typename DSP>
    struct DSP_Choice : juce::dsp::ProcessorBase
    {
        void prepare(const juce::dsp::ProcessSpec& spec) override
        {
            dsp.prepare(spec);
        };
        
        void process (const juce::dsp::ProcessContextReplacing<float>& context) override
        {
            dsp.process(context);
        };
        
        void reset() override
        {
            dsp.reset();
        };
        
        DSP dsp;
    };
    
    DSP_Choice<juce::dsp::DelayLine<float>> delay;
    DSP_Choice<juce::dsp::Phaser<float>> phaser;
    DSP_Choice<juce::dsp::Chorus<float>> chorus;
    DSP_Choice<juce::dsp::LadderFilter<float>> overdrive, ladderFilter;
    DSP_Choice<juce::dsp::IIR::Filter<float>> generalFilter;
    
    struct ProcessState
    {
        juce::dsp::ProcessorBase* processor = nullptr;
        bool bypassed = false;
    };
    
    using DSP_Pointers = std::array<ProcessState,
        static_cast<size_t>(DSP_Option::END_OF_LIST)>;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Project13AudioProcessor)
};
