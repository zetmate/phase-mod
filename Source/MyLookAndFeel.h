/*
  ==============================================================================

    MyLookAndFeel.h
    Created: 16 Jun 2018 9:59:49am
    Author:  Macbook

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class MyLookAndFeel    : public LookAndFeel_V4
{
public:
    MyLookAndFeel()
    {
        Colour myBlue09 (Colour::fromRGBA (14, 119, 225, 255 * 0.9));
        Colour myBlue05 (Colour::fromRGBA (14, 119, 225, 255 * 0.5));
        
//        setColour (Slider::thumbColourId, myBlue09);
//        setColour (Slider::rotarySliderFillColourId, myBlue07);
        setColour (TextButton::buttonOnColourId, Colour::fromFloatRGBA (14, 119, 225, 0.4));
    }
    ~MyLookAndFeel()
    {
    }
};

