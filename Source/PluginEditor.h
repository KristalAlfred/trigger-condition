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
    juce::Slider frequencySlider;
    juce::Label header;
    juce::Label sliderTitle;
    
    juce::StringArray choices { "1:1", "1:2", "1:3", "2:3", "1:4", "3:4", "1:5", "2:5", "3:5", "4:5", "1:6", "5:6", "1:7", "2:7", "3:7", "4:7", "5:7", "6:7", "1:8", "3:8", "5:8", "7:8", "1:9", "2:9", "4:9", "5:9", "7:9", "8:9", "1:10", "3:10", "7:10", "9:10", "1:11", "2:11", "3:11", "4:11", "5:11", "6:11", "7:11", "8:11", "9:11", "10:11", "1:12", "5:12", "7:12", "11:12", "1:13", "2:13", "3:13", "4:13", "5:13", "6:13", "7:13", "8:13", "9:13", "10:13", "11:13", "12:13", "1:14", "3:14", "5:14", "9:14", "11:14", "13:14", "1:15", "2:15", "4:15", "7:15", "8:15", "11:15", "13:15", "14:15", "1:16", "3:16", "5:16", "7:16", "9:16", "11:16", "13:16", "15:16" };
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> frequencySliderAttachment;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TriggerConditionAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TriggerConditionAudioProcessorEditor)
};
