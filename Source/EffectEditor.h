/*
  ==============================================================================

    EffectEditor.h
    Created: 2 Jun 2018 11:14:18am
    Author:  Macbook

  ==============================================================================
*/

#pragma once

//==============================================================================
/*
*/
class EffectEditor    : public Component,
                        public Slider::Listener,
                        public Button::Listener
{
public:
    EffectEditor (Proc& p, int width, int height) : proc (p)
    {
        setSize (width, height);
        
        Utility::addSlider (&depthSlider, &depthLabel, "DEPTH", 0, 100, 1, 50,
                            "%", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 30, this, this, true);
        
        Utility::addSlider (&delaySlider, &delayLabel, "DELAY", 2, 500, 0.1, 40,
                            " ms", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 60, this, this, true);
        
        Utility::addSlider (&feedbackSlider, &feedbackLabel, "FEEDBACK", -99, 99, 1, 0,
                            "%", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, -70, this, this, true);
        
        Utility::addSlider (&lfo3AmpSlider, &lfo3AmpLabel, "Feedback LFO depth", 5, 99, 1, 50,
                            "%", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 80, this, this, false);
        
        Utility::addTextButton (&separateProcessingButton, "separate processing",
                                true, true, true, this, this);
        
        Utility::addTextButton (&doubleFeedbackButton, "single feedback",
                                false, true, true, this, this);
    }

    ~EffectEditor()
    {
    }
    
    //===================================================================================
    void sliderValueChanged (Slider* slider) override
    {
        double value = slider->getValue();
        
        if (slider == &depthSlider)
        {
            proc.setDepth (value / 100.0);
        }
        else if (slider == &delaySlider)
        {
            proc.setMaxDelayTime (value);
        }
        else if (slider == &feedbackSlider)
        {
            proc.setFeedbackGain (value / 100.0);
            
            if (value < 5 && value > -5)
                proc.setPrevSampleGain (0);
        }
        else if (slider == &lfo3AmpSlider)
        {
            proc.setLfo3Amp (value / 100.0);
        }
    }
    
    void buttonClicked (Button* button) override
    {
        bool isOn = button->getToggleState();
        
        if (button == &separateProcessingButton)
        {
            if (isOn)
            {
                button->setButtonText ("separate processing");
                proc.setSeparateProcessing();
                feedbackSlider.setRange (-99, 99, 1);
            }
            else
            {
                button->setButtonText ("cascade processing");
                proc.setCascadeProcessing();
                feedbackSlider.setRange (-65, 65, 1);
                
            }
        }
        else if (button == &doubleFeedbackButton)
        {
            if (isOn)
            {
                float currentFbGain = feedbackSlider.getValue() / 100.0;
                if (currentFbGain > 0)
                {
                    proc.setFeedbackGain (0.7);
                    proc.setPrevSampleGain (0.25);
                }
                else
                {
                    proc.setFeedbackGain (-0.7);
                    proc.setPrevSampleGain (-0.25);
                }
                Utility::setSliderEnabled (&feedbackSlider, &feedbackLabel, false);
                doubleFeedbackButton.setButtonText ("double feedback");
            }
            else
            {
                Utility::setSliderEnabled (&feedbackSlider, &feedbackLabel, true);
                proc.setPrevSampleGain (0);
                proc.setFeedbackGain (feedbackSlider.getValue() / 100.0);
                doubleFeedbackButton.setButtonText ("single feedback");
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
        //25 - 150 - 275
        //first row
        depthSlider.setBounds (25, 50, 100, 100);
        delaySlider.setBounds (150, 50, 100, 100);
        feedbackSlider.setBounds (275, 50, 100, 100);
        lfo3AmpSlider.setBounds (275, 50, 100, 100);
        
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
    
    Slider lfo3AmpSlider;
    Label lfo3AmpLabel;
    
    TextButton separateProcessingButton, doubleFeedbackButton;
    //==============================================================
    Proc& proc;
    //==============================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EffectEditor)
};
