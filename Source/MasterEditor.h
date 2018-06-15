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
        
        Utility::addSlider (&doubleFbSlider, &doubleFbLabel, "double fb", -0.5, 0.5, 0.01, 0,
                            "", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 0, this, this, true);
        
        Utility::addSlider (&inputGainSlider, &inputGainLabel, "Input gain", -24, 16, 0.1, 0,
                            "dB", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 0, this, this, true);
        
        Utility::addSlider (&effectGainSlider, &effectGainLabel, "Effect gain", -16, 16, 0.1, 0,
                            "dB", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 0, this, this, true);
        
        Utility::addSlider (&voice1GainSlider, &voice1GainLabel, "Voice 1 mix", 0, 100, 1, 50,
                            "%", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 100, this, this, true);
        
        Utility::addSlider (&voice2GainSlider, &voice2GainLabel, "Voice 2 mix", 0, 100, 1, 50,
                            "%", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 100, this, this, true);
        
        Utility::addSlider (&dryWetSlider, &dryWetLabel, "Mix", 0, 100, 1, 50,
                            "%", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 50, this, this, true);
    }

    ~MasterEditor()
    {
    }
    
    //===================================================================================
    void sliderValueChanged (Slider* slider) override
    {
        double value = slider->getValue();
        
        if (slider == &dryWetSlider)
            proc.setDryWet (value / 100.0);
        else if (slider == &doubleFbSlider)
            proc.setPrevSampleGain (value);
        else if (slider == &inputGainSlider)
            proc.setInputGain (Utility::fromDb (value));
        else if (slider == &voice1GainSlider)
            proc.setGain1 (value / 100.0);
        else if (slider == &voice2GainSlider)
            proc.setGain2 (value / 100.0);
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
        dryWetSlider.setBounds (25, 50, 100, 100);
        inputGainSlider.setBounds (25, 195, 100, 100);
        voice1GainSlider.setBounds (25, 340, 100, 100);
        voice2GainSlider.setBounds (25, 485, 100, 100);
    }

private:
    Slider doubleFbSlider;
    Label doubleFbLabel;
    
    Slider inputGainSlider, voice1GainSlider, voice2GainSlider, effectGainSlider;
    Label inputGainLabel, voice1GainLabel, voice2GainLabel, effectGainLabel;
    
    Slider dryWetSlider;
    Label dryWetLabel;
//==============================================================
    Proc& proc;
//==============================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterEditor)
};
