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

        //add sliders
        Utility::addSlider (&depthSlider, &depthLabel, "Depth", 0, 1, 0.001, 0.5,
                            "", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::NoTextBox, 0.3, this, this, true);
        
        Utility::addSlider (&feedbackSlider, &feedbackLabel, "Feedback", 0, 0.99, 0.01, 0.5,
                            "", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::NoTextBox, 0, this, this, true);
        
        Utility::addSlider (&dryWetSlider, &dryWetLabel, "Mix", 0, 1, 0.01, 0.3,
                            "", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::NoTextBox, 0.5, this, this, true);
        
        //add buttons
        Utility::addTextButton (&separateProcessingButton, "separate processing",
                                true, true, this, this);
        
        Utility::addTextButton (&feedbackPolarityButton, "negative",
                                false, true, this, this);
        
        Utility::addTextButton (&doubleFeedbackButton, "single feedback",
                                false, true, this, this);
    }

    ~MasterEditor()
    {
    }
    
    //===================================================================================
    void sliderValueChanged (Slider* slider) override
    {
        if (slider == &dryWetSlider)
        {
            proc.setGlobalDryWet (slider->getValue());
        }
        else if (slider == &depthSlider)
        {
            proc.setDepth (slider->getValue());
        }
        else if (slider == &feedbackSlider)
        {
            proc.setFeedbackGain (slider->getValue());
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
        else if (button == &feedbackPolarityButton)
        {
            bool isOn = button->getToggleState();
            
            if (isOn)
            {
                button->setToggleState (false, dontSendNotification);
                button->setButtonText ("negative");
                proc.setFeedbackPolarity (false);
            }
            else
            {
                button->setToggleState (true, dontSendNotification);
                button->setButtonText ("positive");
                proc.setFeedbackPolarity (true);
            }
        }
        else if (button == &doubleFeedbackButton)
        {
            bool isOn = button->getToggleState();
            
            if (isOn)
            {
                button->setToggleState (false, dontSendNotification);
                button->setButtonText ("single feedback");
                proc.setDoubleFeedback (false, 0);           }
            else
            {
                button->setToggleState (true, dontSendNotification);
                button->setButtonText ("double feedback");
                proc.setDoubleFeedback (true, feedbackSlider.getValue());
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
        dryWetSlider.setBounds (87.5, 50, 100, 100);
        
        feedbackSlider.setBounds (25, 225, 100, 100);
        depthSlider.setBounds (150, 225, 100, 100);
        
        separateProcessingButton.setBounds ((getWidth() - 100) * 0.5,
                                            350, 100, 35);
        
        feedbackPolarityButton.setBounds ((getWidth() - 100) * 0.5,
                                          400, 100, 35);
        
        doubleFeedbackButton.setBounds ((getWidth() - 100) * 0.5,
                                        450, 100, 35);
    }

private:
    
    Slider feedbackSlider;
    Label feedbackLabel;
    
    Slider depthSlider;
    Label depthLabel;
    
    Slider dryWetSlider;
    Label dryWetLabel;
    
    TextButton separateProcessingButton;
    TextButton feedbackPolarityButton;
    TextButton doubleFeedbackButton;
    
    Proc& proc;
//==============================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterEditor)
};
