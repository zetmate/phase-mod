/*
  ==============================================================================

    LookAndFeel.h
    Created: 27 Jun 2020 9:53:07pm
    Author:  Ekaterina Zakimatova

  ==============================================================================
*/

#pragma once
#include "UIThemes.h"

class LF    : public LookAndFeel_V4
{
public:
    LF (Theme colorTheme)
    :   theme (colorTheme)
    {
        setColour (Slider::thumbColourId, theme.main);
        setColour (Slider::rotarySliderFillColourId, PALETTE.opaqueWhite);
        setColour (TextButton::buttonOnColourId, theme.main);
    }
    ~LF()
    {
    }
    
    void drawButtonBackground (Graphics& g,
                               Button& button,
                               const Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;
    
    void drawComboBox (Graphics&, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox&) override;
    
    Theme theme;
};

//==============================================================================
class LFEffect    : public LF
{
public:
    LFEffect ()
    :   LF (Theme (PALETTE.blue, PALETTE.asphalt))
    {
    }

    ~LFEffect()
    {
    }
};

//==============================================================================
class LFMod    : public LF
{
public:
    LFMod ()
    :   LF (Theme (PALETTE.red, PALETTE.darkBlue))
    {
    }

    ~LFMod()
    {
    }
};

//==============================================================================
class LFMaster    : public LF
{
public:
    LFMaster ()
    :   LF (Theme (PALETTE.mint, PALETTE.darkPurple))
    {
    }

    ~LFMaster()
    {
    }
};
