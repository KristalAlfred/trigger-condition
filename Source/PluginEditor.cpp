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
    
    frequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    probabilitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    
    probabilityButtonAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(aptvs, "probabilityMode", probabilityModeButton));
    frequencySliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(aptvs, "allowedMessageFrequency", frequencySlider));
    probabilitySliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(aptvs, "probability", probabilitySlider));
    
    probabilitySlider.setLookAndFeel(&customLnF);
    frequencySlider.setLookAndFeel(&customLnF);
    probabilityModeButton.setLookAndFeel(&customLnF);
    
    probabilityModeButton.onClick = [this] () {
        if (probabilityModeButton.getToggleState()) {
            this->addAndMakeVisible(&probabilitySlider);
            this->removeChildComponent(&frequencySlider);
            this->sliderTitle.setText("Probability", juce::dontSendNotification);
        } else {
            this->addAndMakeVisible(&frequencySlider);
            this->removeChildComponent(&probabilitySlider);
            this->sliderTitle.setText("Periodic", juce::dontSendNotification);
        }
    };
    
    if (probabilityModeButton.getToggleState()) addAndMakeVisible(&probabilitySlider);
    else                                        addAndMakeVisible(&frequencySlider);
    
    addAndMakeVisible(&probabilityModeButton);
    
    
    header.setText("TriggerCondition", juce::NotificationType::dontSendNotification);
    header.setFont(juce::Font("Avenir", 20.f, juce::Font::FontStyleFlags::bold));
    header.setEditable(false);
    addAndMakeVisible(&header);
    
    
    sliderTitle.setText(probabilityModeButton.getToggleState() ? "Probability" : "Periodic", juce::dontSendNotification);
    sliderTitle.setFont(juce::Font("Avenir", 20.f, 0));
    sliderTitle.setJustificationType(juce::Justification::centred);
    sliderTitle.setEditable(false);
    addAndMakeVisible(&sliderTitle);
    
    
    probabilityModeLabel.setText("Probability mode", juce::dontSendNotification);
    probabilityModeLabel.setFont(juce::Font("Avenir", 16, 0));
    probabilityModeLabel.setJustificationType(juce::Justification::centred);
    probabilityModeLabel.setEditable(false);
    addAndMakeVisible(&probabilityModeLabel);
    
    
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
    
    auto width { getWidth() };
    auto halfWidth { width / 2 };
    auto height { getHeight() };
    auto halfHeight { height / 2 };
    
    auto sliderWidth { 150 };
    auto sliderHeight { 150 };
    frequencySlider.setBounds(halfWidth - sliderWidth / 2,
                              halfHeight - sliderHeight / 2,
                              sliderWidth,
                              sliderHeight);
    
    auto buttonWidth { 60 };
    auto buttonHeight { 30 };
    probabilityModeButton.setBounds(halfWidth - buttonWidth / 2,
                                    (7 * getHeight() / 8) - buttonHeight / 2,
                                    buttonWidth,
                                    buttonHeight);
    
    probabilitySlider.setBounds(halfWidth - sliderWidth / 2,
                                halfHeight - sliderHeight / 2,
                                sliderWidth,
                                sliderHeight);
    
    auto headerWidth { 200 };
    auto headerHeight { 20 };
    header.setBounds(10, 10, headerWidth, headerHeight);
    
    sliderTitle.setBounds(halfWidth - headerWidth / 2, 50, headerWidth, headerHeight);
    
    probabilityModeLabel.setBounds(halfWidth - headerWidth / 2, height * 0.93, headerWidth, headerHeight);
}

void TriggerConditionAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
