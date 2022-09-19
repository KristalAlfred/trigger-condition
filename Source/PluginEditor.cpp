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
    frequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    
    frequencySliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(aptvs, "frequency", frequencySlider));
    frequencySlider.textFromValueFunction = [this] (double value) {
        if (value <= 100) return juce::String(value) + "%";
        return choices[value-101];
    };
    
    // The initial value isn't generated using the lambda above for some reason, so this line is used to display the correct text.
    frequencySlider.setTextValueSuffix(" ");
    
    frequencySlider.setLookAndFeel(&customLnF);
    
    addAndMakeVisible(&frequencySlider);
    
    header.setText("TriggerCondition", juce::NotificationType::dontSendNotification);
    header.setFont(juce::Font("Avenir", 20.f, juce::Font::FontStyleFlags::bold));
    header.setEditable(false);
    addAndMakeVisible(&header);
    
    
    sliderTitle.setText("Condition", juce::dontSendNotification);
    sliderTitle.setFont(juce::Font("Avenir", 20.f, 0));
    sliderTitle.setJustificationType(juce::Justification::centred);
    sliderTitle.setEditable(false);
    addAndMakeVisible(&sliderTitle);
    
    setSize (400, 300);
}

TriggerConditionAudioProcessorEditor::~TriggerConditionAudioProcessorEditor()
{
}

//==============================================================================
void TriggerConditionAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(171, 44, 59));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    
    auto width { getWidth() };
    auto halfWidth { width / 2 };
    auto height { getHeight() };
    auto halfHeight { height / 2 };
    
    auto sliderWidth { 150 };
    auto sliderHeight { 150 };
    frequencySlider.setBounds(halfWidth - (sliderWidth / 2),
                              halfHeight - (sliderHeight / 2) + 25,
                              sliderWidth,
                              sliderHeight);

    auto headerWidth { 200 };
    auto headerHeight { 20 };
    header.setBounds(10, 10, headerWidth, headerHeight);
    
    sliderTitle.setBounds(halfWidth - headerWidth / 2, 75, headerWidth, headerHeight);
}

void TriggerConditionAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
