/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TriggerConditionAudioProcessorEditor::TriggerConditionAudioProcessorEditor (TriggerConditionAudioProcessor& p, juce::AudioProcessorValueTreeState& aptvs)
    : AudioProcessorEditor (&p), aptvs(aptvs), audioProcessor (p)
{
    
    frequencySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    chanceSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    chanceButtonAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(aptvs, "chanceMode", chanceModeButton));
    frequencySliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(aptvs, "allowedMessageFrequency", frequencySlider));
    chanceSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(aptvs, "percentage", chanceSlider));
    
    addAndMakeVisible(&chanceModeButton);
    addAndMakeVisible(&frequencySlider);
    addAndMakeVisible(&chanceSlider);
    
    setSize (400, 300);
}

TriggerConditionAudioProcessorEditor::~TriggerConditionAudioProcessorEditor()
{
}

//==============================================================================
void TriggerConditionAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    frequencySlider.setBounds(50, 50, getWidth() / 5, getHeight() / 5);
    chanceModeButton.setBounds(200, 50, getWidth() / 5, getHeight() / 5);
    chanceSlider.setBounds(300, 50, getWidth() / 5, getHeight() / 5);
}

void TriggerConditionAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
