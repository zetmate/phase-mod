/*
  ==============================================================================

    MasterEditor.h
    Created: 16 Apr 2018 11:43:44am
    Author:  Macbook

  ==============================================================================
*/

#pragma once

#include "Proc.h"

//==============================================================================
/*
*/
class MasterEditor    : public Component,
                        Slider::Listener,
                        Button::Listener
{
public:
    MasterEditor (Proc& p, int width, int height)  : proc (p)
    {
        setSize (width, height);
        
//        Utility::addSlider (&doubleFbSlider, &doubleFbLabel, "double fb", -0.5, 0.5, 0.01, 0,
//                            "", Slider::SliderStyle::RotaryVerticalDrag,
//                            Slider::TextEntryBoxPosition::TextBoxBelow, 0, this, this, true);
        
        
    }

    ~MasterEditor()
    {
    }
    
    //===================================================================================
    void sliderValueChanged (Slider* slider) override
    {
        double value = slider->getValue();
        
        
    }
    
    void buttonClicked (Button* button) override
    {
    }
    //===================================================================================

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::black);
        
        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);
    }

    void resized() override
    {
        
    }

private:
//==============================================================
    Proc& proc;
//==============================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterEditor)
};
