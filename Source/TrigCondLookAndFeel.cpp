/*
  ==============================================================================

    TrigCondLookAndFeel.cpp
    Created: 6 Sep 2022 1:33:03pm
    Author:  Alfred Kristal-Ern

  ==============================================================================
*/

#include "TrigCondLookAndFeel.h"

void TrigCondLookAndFeel::drawRotarySlider (juce::Graphics& g,
                                            int x, int y,
                                            int width, int height,
                                            float sliderPosProportional,
                                            float rotaryStartAngle,
                                            float rotaryEndAngle, juce::Slider &)
{
    auto radius { (float) juce::jmin(width / 2, height / 2) - 4.f };
    auto centreX { (float) x + (float) width * 0.5f };
    auto centreY { (float) y + (float) height * 0.5f };
    auto rx { centreX - radius };
    auto ry { centreY - radius };
    auto rw { radius * 2.0f };
    auto angle { rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle) };
    
    juce::Path path;
    path.addEllipse(rx, ry, rw, rw);
    
    juce::DropShadow shadow {
        juce::Colour((uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 60),
        10,
        juce::Point<int>{ 0, 5 }};
    shadow.drawForPath(g, path);
    
    g.setColour(juce::Colours::darkgrey);
    g.fillPath(path);
    
    g.setColour(juce::Colours::black);
    g.drawLine(centreX + (radius / 2) * cos(angle - juce::MathConstants<float>::halfPi),
               centreY + (radius / 2) * sin(angle - juce::MathConstants<float>::halfPi),
               centreX + radius * cos(angle - juce::MathConstants<float>::halfPi),
               centreY + radius * sin(angle - juce::MathConstants<float>::halfPi),
               5.f);
}

void TrigCondLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.setColour(juce::Colours::white);
    g.drawText(label.getText(), 0, 0,
               label.getWidth(), label.getHeight(),
               juce::Justification::centred);
}


void TrigCondLookAndFeel::drawToggleButton (juce::Graphics& g,
                                            juce::ToggleButton& button,
                                            bool shouldDrawButtonAsHighlighted,
                                            bool shouldDrawButtonAsDown)
{
    auto inset { 10 };
    
    juce::Rectangle<int> background { inset / 2, inset / 2,
                                    button.getWidth() - inset,
                                    button.getHeight() - inset
    };
    
    juce::Colour backgroundColor { button.getToggleState() ?
            juce::Colour{(uint8_t) 30, (uint8_t) 210, (uint8_t) 67, (uint8_t) 255}
        :   juce::Colour{(uint8_t) 150, (uint8_t) 150, (uint8_t) 150, (uint8_t) 255}
    };
    
    juce::DropShadow outlineShadow { juce::Colour{ (uint8_t) 0, 0, 0, (uint8_t) 100 },
        3,
        juce::Point<int>{ 2, 2 }
    };
    outlineShadow.drawForRectangle(g, background);
    
    g.setColour(backgroundColor);
    g.fillRect(background);
    
    
    juce::Colour textColour { (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 255 };
    const std::string buttonText { button.getToggleState() ? "ON" : "OFF" };
    
    g.setColour(textColour);
    g.drawText(buttonText, inset / 2, inset / 2,
               button.getWidth() - inset,
               button.getHeight() - inset, juce::Justification::centred);
}
