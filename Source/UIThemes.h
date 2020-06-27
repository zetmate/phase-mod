/*
  ==============================================================================

    MyLookAndFeel.h
    Created: 16 Jun 2018 9:59:49am
    Author:  Macbook

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

struct Palette
{
public:
    Colour blue = Colour::fromRGB (55, 163, 211);
    Colour mint = Colour::fromRGB (55, 211, 169);
    Colour red = Colour::fromRGB (211, 55, 55);

    Colour darkBlue = Colour::fromRGB (16, 12, 33);
    Colour asphalt = Colour::fromRGB (41, 38, 59);
    Colour darkPurple = Colour::fromRGB (60, 37, 70);
    
    Colour opaqueWhite = Colour::fromRGBA (238, 238, 238, getAlpha(73));
    Colour white = Colour::fromRGBA (238, 238, 238, getAlpha(93));
    
private:
    short int getAlpha (int percents)
    {
        return roundToInt ((float) percents / 100.0f * 255.0f);
    }
};

const Palette PALETTE;


//==============================================================================
struct Theme
{
    Theme (Colour mainColor, Colour bgColor, Colour textColor = PALETTE.white)
    :   main (mainColor),
        bg (bgColor),
        text (textColor)
    {
    }
    
    Colour main;
    Colour bg;
    Colour text;
};
