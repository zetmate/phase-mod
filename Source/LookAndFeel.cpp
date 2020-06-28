/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 27 Jun 2020 9:53:07pm
    Author:  Ekaterina Zakimatova

  ==============================================================================
*/

#include "LookAndFeel.h"

void LF::drawButtonBackground (Graphics& g,
                                           Button& button,
                                           const Colour& backgroundColour,
                                           bool shouldDrawButtonAsHighlighted,
                                           bool shouldDrawButtonAsDown)
{
    auto cornerSize = 6.0f;
    auto bounds = button.getLocalBounds().toFloat().reduced (0.5f, 0.5f);

    auto baseColour = button.getToggleState() ? theme.mainDimmed : PALETTE.transperent;
    auto outlineColor = button.getToggleState() ? PALETTE.transperent : PALETTE.opaqueWhite;

    if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted) {
        baseColour = baseColour.contrasting (shouldDrawButtonAsDown ? 0.2f : 0.05f);
    }

    g.setColour (baseColour);

    auto flatOnLeft   = button.isConnectedOnLeft();
    auto flatOnRight  = button.isConnectedOnRight();
    auto flatOnTop    = button.isConnectedOnTop();
    auto flatOnBottom = button.isConnectedOnBottom();

    if (flatOnLeft || flatOnRight || flatOnTop || flatOnBottom)
    {
        Path path;
        path.addRoundedRectangle (bounds.getX(), bounds.getY(),
                                  bounds.getWidth(), bounds.getHeight(),
                                  cornerSize, cornerSize,
                                  ! (flatOnLeft  || flatOnTop),
                                  ! (flatOnRight || flatOnTop),
                                  ! (flatOnLeft  || flatOnBottom),
                                  ! (flatOnRight || flatOnBottom));

        g.fillPath (path);

        g.setColour (outlineColor);
        g.drawRoundedRectangle (bounds, cornerSize + 2, 2.0f);
    }
    else
    {
        g.fillRoundedRectangle (bounds, cornerSize);

        g.setColour (outlineColor);
        g.drawRoundedRectangle (bounds, cornerSize, 1.5f);
    }
}

//==============================================================================
void LF::drawComboBox (Graphics& g, int width, int height, bool,
                                   int, int, int, int, ComboBox& box)
{
    auto cornerSize = box.findParentComponentOfClass<ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
    Rectangle<int> boxBounds (0, 0, width, height);

    g.setColour (PALETTE.opaqueWhite);
    g.drawRoundedRectangle (boxBounds.toFloat(), cornerSize, 3.0f);

    Rectangle<int> arrowZone (width - 23, 12, 17, 6);
    Path path;
    path.startNewSubPath (arrowZone.getX() + 3.0f, arrowZone.getCentreY() - 2.0f);
    path.lineTo (static_cast<float> (arrowZone.getCentreX()), arrowZone.getCentreY() + 3.0f);
    path.lineTo (arrowZone.getRight() - 3.0f, arrowZone.getCentreY() - 2.0f);

    g.setColour (box.findColour (ComboBox::arrowColourId).withAlpha ((box.isEnabled() ? 0.9f : 0.2f)));
    g.fillPath(path);
}
