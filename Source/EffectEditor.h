/*
  ==============================================================================

    EffectEditor.h
    Created: 2 Jun 2018 11:14:18am
    Author:  Macbook

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class EffectEditor    : public Component,
                        public Slider::Listener,
                        public Button::Listener
{
public:
    EffectEditor (Proc& p, int width, int height)  : proc (p)
    {
        setSize (width, height);
        
        Utility::addSlider (&depthSlider, &depthLabel, "Depth", 0, 100, 1, 50,
                            "%", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 30, this, this, true);
        
        Utility::addSlider (&delaySlider, &delayLabel, "Delay", 2, 500, 0.1, 40,
                            " ms", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 60, this, this, true);
        
        Utility::addSlider (&feedbackSlider, &feedbackLabel, "Feedback", -99, 99, 1, 0,
                            "%", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 30, this, this, true);
        
        Utility::addTextButton (&separateProcessingButton, "separate processing",
                                true, true, this, this);
        
        Utility::addTextButton (&doubleFeedbackButton, "single feedback",
                                false, true, this, this);
    }

    ~EffectEditor()
    {
    }
    
    //===================================================================================
    void sliderValueChanged (Slider* slider) override
    {
        double value = slider->getValue();
        
        if (slider == &depthSlider)
            proc.setDepth (value / 100.0);
        
        else if (slider == &delaySlider)
            proc.setMaxDelayTime (value);
        
        else if (slider == &feedbackSlider)
            proc.setFeedbackGain (value / 100.0);
    }
    
    void buttonClicked (Button* button) override
    {
        bool isOn = button->getToggleState();
        
        if (button == &separateProcessingButton)
        {
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
        //    else if (button == &doubleFeedbackButton)
        //    {
        //        bool isOn = button->getToggleState();
        //
        //        if (isOn)
        //        {
        //            button->setToggleState (false, dontSendNotification);
        //            button->setButtonText ("single feedback");
        //            proc.setPrevSampleGain (0);
        //            proc.setFeedbackGain (feedbackSlider.getValue() / 100);
        //            feedbackSlider.setEnabled (true);
        //        }
        //        else
        //        {
        //            button->setToggleState (true, dontSendNotification);
        //            button->setButtonText ("double feedback");
        //            proc.setFeedbackGain (0.7);
        //            proc.setPrevSampleGain (0.3);
        //            feedbackSlider.setEnabled (false);
        //        }
        //    }
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
        //25 - 150 - 275
        //first row
        depthSlider.setBounds (25, 50, 100, 100);
        delaySlider.setBounds (150, 50, 100, 100);
        feedbackSlider.setBounds (275, 50, 100, 100);
        
        //second row
        separateProcessingButton.setBounds (25, 175, 225, 50);
        doubleFeedbackButton.setBounds (275, 175, 100, 50);
    }
    friend class ModEditor;
    
private:
    Slider depthSlider;
    Label depthLabel;
    
    Slider delaySlider;
    Label delayLabel;
    
    Slider feedbackSlider;
    Label feedbackLabel;
    
    TextButton separateProcessingButton, doubleFeedbackButton;
    //==============================================================
    Proc& proc;
    //==============================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EffectEditor)
};
