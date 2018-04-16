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

        Utility::addSlider (&feedbackSlider, &feedbackLabel, "Feedback", 0, 0.99, 0.01, 0.5,
                            "", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::NoTextBox, 0, this, this, true);
        
        Utility::addSlider (&dryWetSlider, &dryWetLabel, "Mix", 0, 1, 0.01, 0.5,
                            "", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::NoTextBox, 1, this, this, true);
        
        separateProcessingButton.setToggleState (true, dontSendNotification);
        separateProcessingButton.setButtonText ("separate processing");
        separateProcessingButton.addListener (this);
        addAndMakeVisible (separateProcessingButton);
    }

    ~MasterEditor()
    {
    }
    
    //===================================================================================
    void sliderValueChanged (Slider* slider) override
    {
        if (slider == &feedbackSlider)
        {
            proc.setGlobalFeedbackGain (slider->getValue());
        }
        else if (slider == &dryWetSlider)
        {
            proc.setGlobalDryWet (slider->getValue());
        }
    }
    
    void buttonClicked (Button* button) override
    {
        if (button == &separateProcessingButton)
        {
            bool isOn = button->getToggleState();
            
            if (isOn)
            {
                button->setToggleState (false, dontSendNotification);
                button->setButtonText ("cascade processing");
                proc.setCascadeProcessing();
            }
            else
            {
                button->setToggleState (true, dontSendNotification);
                button->setButtonText ("separate processing");
                proc.setSeparateProcessing();
            }
        }
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
        feedbackSlider.setBounds (25, 225, 100, 100);
        dryWetSlider.setBounds (150, 225, 100, 100);
        
        separateProcessingButton.setBounds ((getWidth() - 100) * 0.5,
                                            350, 100, 30);
    }

private:
    
    Slider feedbackSlider;
    Label feedbackLabel;
    
    Slider dryWetSlider;
    Label dryWetLabel;
    
    TextButton separateProcessingButton;
    
    Proc& proc;
//==============================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterEditor)
};
