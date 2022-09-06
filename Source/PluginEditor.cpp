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
    probabilitySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    
    chanceButtonAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(aptvs, "probabilityMode", probabilityModeButton));
    frequencySliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(aptvs, "allowedMessageFrequency", frequencySlider));
    chanceSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(aptvs, "probability", probabilitySlider));
    
    probabilitySlider.setLookAndFeel(&customLnF);
    frequencySlider.setLookAndFeel(&customLnF);
    probabilityModeButton.setLookAndFeel(&customLnF);
    
    addAndMakeVisible(&probabilityModeButton);
    addAndMakeVisible(&frequencySlider);
    addAndMakeVisible(&probabilitySlider);
    
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
    probabilityModeButton.setBounds(200, 50, getWidth() / 5, getHeight() / 5);
    probabilitySlider.setBounds(300, 50, getWidth() / 5, getHeight() / 5);
}

void TriggerConditionAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
