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
        // Slider
        setColour (Slider::textBoxOutlineColourId, PALETTE.transperent);
        setColour (Slider::textBoxHighlightColourId, PALETTE.transperent);
        setColour (Slider::textBoxBackgroundColourId, theme.bg);
        setColour (Slider::thumbColourId, PALETTE.transperent);

        setColour (Slider::backgroundColourId, PALETTE.opaqueWhite);
        setColour (Slider::rotarySliderOutlineColourId, PALETTE.opaqueWhite);

        setColour (Slider::trackColourId, theme.main);
        setColour (Slider::rotarySliderFillColourId, theme.main);
        
        // Popup
        setColour (PopupMenu::backgroundColourId, theme.bg);
        setColour (PopupMenu::highlightedBackgroundColourId, theme.mainDimmed);

        // Button
        setColour (TextButton::buttonOnColourId, theme.bg);
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
