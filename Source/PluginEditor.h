/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "TrigCondLookAndFeel.h"

//==============================================================================
/**
*/
class TriggerConditionAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TriggerConditionAudioProcessorEditor (TriggerConditionAudioProcessor&, juce::AudioProcessorValueTreeState& aptvs);
    ~TriggerConditionAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    TrigCondLookAndFeel customLnF;
    juce::AudioProcessorValueTreeState& aptvs;
    juce::ToggleButton probabilityModeButton;
    juce::Slider frequencySlider;
    juce::Slider probabilitySlider;
    juce::Label header;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> probabilityButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> frequencySliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> probabilitySliderAttachment;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TriggerConditionAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TriggerConditionAudioProcessorEditor)
};
