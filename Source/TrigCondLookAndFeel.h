/*
  ==============================================================================

    TrigCondLookAndFeel.h
    Created: 6 Sep 2022 1:33:03pm
    Author:  Alfred Kristal-Ern

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class TrigCondLookAndFeel : public juce::LookAndFeel_V4 {
public:
    virtual void drawRotarySlider (juce::Graphics &, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &) override;
    
    virtual void drawLabel (juce::Graphics&, juce::Label&) override;
};
