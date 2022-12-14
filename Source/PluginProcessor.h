/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <random>

//==============================================================================
/**
*/
class TriggerConditionAudioProcessor  : public juce::AudioProcessor,
                                        public juce::AudioProcessorValueTreeState::Listener
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    TriggerConditionAudioProcessor();
    ~TriggerConditionAudioProcessor() override;

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
    
    virtual void parameterChanged (const juce::String &parameterID, float newValue) override;

private:
    int messageCount { 0 };
    
    juce::AudioProcessorValueTreeState parameters;
    
    std::atomic<float>* frequencyParameter { nullptr };
    int filteredNoteCount;
    
    void filterNote(juce::MidiBuffer& buffer, const juce::MidiMessageMetadata& messageMetadata, int passedMessagesPerPeriod, int periodLength);
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;
    std::function<int()> randomNumber { std::bind(distribution, generator) };
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TriggerConditionAudioProcessor)
};
