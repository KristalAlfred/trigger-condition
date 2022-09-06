/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

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
    juce::AudioProcessorValueTreeState& aptvs;
    juce::ToggleButton chanceModeButton;
    juce::Slider frequencySlider;
    juce::Slider chanceSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> chanceButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> frequencySliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chanceSliderAttachment;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TriggerConditionAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TriggerConditionAudioProcessorEditor)
};
