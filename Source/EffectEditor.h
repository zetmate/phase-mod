/*
  ==============================================================================

    EffectEditor.h
    Created: 2 Jun 2018 11:14:18am
    Author:  Macbook

  ==============================================================================
*/

#pragma once
#include "MyLookAndFeel.h"
#include "ParameterControl.h"
//==============================================================================
/*
*/
class EffectEditor    : public Component,
                        public Slider::Listener,
                        public Button::Listener
{
public:
    EffectEditor (Vibrato2AudioProcessor& p, int width, int height) : proc (p.proc), p (p)
    {
        setSize (width, height);
        
        Utility::addSlider (&depthSlider, &depthLabel, "DEPTH", 0, 100, 1, 50,
                            "%", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 30, this, this, true);
        depthSlider.setLookAndFeel (&myLookAndFeel);
        
        Utility::addSlider (&delaySlider, &delayLabel, "DELAY", 2, 500, 0.1, 40,
                            " ms", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 60, this, this, true);
        delaySlider.setLookAndFeel (&myLookAndFeel);
        
        Utility::addSlider (&feedbackSlider, &feedbackLabel, "FEEDBACK", -99, 99, 1, 0,
                            "%", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, -70, this, this, true);
        feedbackSlider.setLookAndFeel (&myLookAndFeel);
        
        Utility::addSlider (&lfo3AmpSlider, &lfo3AmpLabel, "Feedback LFO depth", 15, 99, 1, 50,
                            "%", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 80, this, this, false);
        lfo3AmpSlider.setLookAndFeel (&myLookAndFeel);
        
        //set feedback range
        feedbackSeparateRange.start = -99;
        feedbackSeparateRange.end = 99;
        feedbackCascadeRange.start = -65;
        feedbackCascadeRange.end = 65;
        
        Utility::addTextButton (&processingTypeButton, "separate processing",
                                true, true, true, this, this);
        
        Utility::addTextButton (&feedbackTypeButton, "single feedback",
                                false, true, true, this, this);
        
        //======================================================================================
        //                                  MASTER EDITOR
        Utility::addSlider (&inputGainSlider, &inputGainLabel, "MASTER", -24, 16, 0.1, 0,
                            "dB", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 0, this, this, true);
        
        inputGainSlider.setDoubleClickReturnValue (true, 0.0);
        
        Utility::addSlider (&voice1MixSlider, &voice1MixLabel, "VOICE 1 MIX", 0, 200, 1, 100,
                            "%", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 100, this, this, true);
        
        Utility::addSlider (&voice2MixSlider, &voice2MixLabel, "VOICE 2 MIX", 0, 200, 1, 100,
                            "%", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 100, this, this, true);
        
        voice1MixSlider.setDoubleClickReturnValue (true, 100);
        voice2MixSlider.setDoubleClickReturnValue (true, 100);
        
        //global dryWet
        Utility::addSlider (&dryWetSlider, &dryWetLabel, "MIX", 0, 100, 1, 50,
                            "%", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 50, this, this, true);
        
        //slider attachments
        depthSliderAttach = new AudioProcessorValueTreeState::SliderAttachment (p.parameters,
                                                                                p.depthId,
                                                                                depthSlider);
        
        delaySliderAttach = new AudioProcessorValueTreeState::SliderAttachment (p.parameters,
                                                                                p.delayId,
                                                                                delaySlider);
        
        feedbackSliderAttach = new AudioProcessorValueTreeState::SliderAttachment (p.parameters,
                                                                                   p.feedbackId,
                                                                                   feedbackSlider);
        
        fbLfoAmpSliderAttach = new AudioProcessorValueTreeState::SliderAttachment (p.parameters,
                                                                                   p.fbLfoAmpId,
                                                                                   lfo3AmpSlider);
        
        mixSliderAttach = new AudioProcessorValueTreeState::SliderAttachment (p.parameters,
                                                                              p.mixId,
                                                                              dryWetSlider);
        
        voice1MixSliderAttach = new AudioProcessorValueTreeState::SliderAttachment (p.parameters,
                                                                                    p.voice1MixId,
                                                                                    voice1MixSlider);
        
        voice2MixSliderAttach = new AudioProcessorValueTreeState::SliderAttachment (p.parameters,
                                                                                    p.voice2MixId,
                                                                                    voice2MixSlider);
        
        masterSliderAttach = new AudioProcessorValueTreeState::SliderAttachment (p.parameters,
                                                                                 p.masterId,
                                                                                 inputGainSlider);
        
        procTypeButtonAttach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                                   p.procTypeId,
                                                                                   processingTypeButton);
        
        fbTypeButtonAttach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                                 p.fbTypeId,
                                                                                 feedbackTypeButton);
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
            if (doubleFeedback)
            {
            }
            else
            {
               proc.setFeedbackGain (value / 100.0);
            }
        }
        else if (slider == &lfo3AmpSlider)
        {
            proc.setLfo3Amp (value / 100.0);
        }
        //=====================================
        //MASTER
        else if (slider == &dryWetSlider)
        {
            proc.setDryWet (value / 100.0);
        }
        else if (slider == &inputGainSlider)
        {
            proc.setInputGain (Utility::fromDb (value));
        }
        else if (slider == &voice1MixSlider)
        {
            if (processingTypeButton.getToggleState())
                proc.setGain1 (value / 100.0);
            else
                proc.setVoice1Mix (value / 100.0);
        }
        else if (slider == &voice2MixSlider)
        {
            if (processingTypeButton.getToggleState())
                proc.setGain2 (value / 100.0);
            else
                proc.setVoice2Mix (value / 100.0);
        }
    }
    
    void buttonClicked (Button* button) override
    {
        bool isOn = button->getToggleState();
        
        if (button == &processingTypeButton)
        {
            if (isOn)
            {
                button->setButtonText ("separate processing");
                proc.setSeparateProcessing();
                
                //set feedback ranges
                feedbackSlider.setRange (-99, 99, 1);
                lfo3AmpSlider.setRange (-99, 99, 1);
                
                //set appropriate feedback value
                double cascadeFbValue = feedbackCascadeRange.convertTo0to1 (feedbackSlider.getValue());
                double separateFbValue = feedbackSeparateRange.convertFrom0to1 (cascadeFbValue);
                feedbackSlider.setValue (separateFbValue);
                
                //enable double feedback
                feedbackTypeButton.setEnabled (true);
                
                //update voice gain sliders' values
                proc.setGain1 (voice1MixSlider.getValue() / 100.0);
                proc.setGain2 (voice2MixSlider.getValue() / 100.0);
                
                //set dry wet of voices to 100%
                proc.setVoice1Mix (1.0);
                proc.setVoice2Mix (1.0);
            }
            else
            {
                button->setButtonText ("cascade processing");
                proc.setCascadeProcessing();
                
                //set feedback ranges
                feedbackSlider.setRange (-65, 65, 1);
                lfo3AmpSlider.setRange (-65, 65, 1);
                
                //set aproptiate feedback value
                double separateFbValue = feedbackSeparateRange.convertTo0to1 (feedbackSlider.getValue());
                double cascadeFbValue = feedbackCascadeRange.convertFrom0to1 (separateFbValue);
                feedbackSlider.setValue (cascadeFbValue);
                
                //disable double feedback
                feedbackTypeButton.setToggleState (false, sendNotification);
                feedbackTypeButton.setEnabled (false);
                
                //update voice mix sliders' values
                double value1 = voice1MixSlider.getValue();
                double value2 = voice2MixSlider.getValue();
                proc.setVoice1Mix (value1 / 100.0);
                proc.setVoice2Mix (value2 / 100.0);
            }
        }
        else if (button == &feedbackTypeButton)
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
                feedbackTypeButton.setButtonText ("double feedback");
                doubleFeedback = true;
            }
            else
            {
                Utility::setSliderEnabled (&feedbackSlider, &feedbackLabel, true);
                proc.setPrevSampleGain (0);
                proc.setFeedbackGain (feedbackSlider.getValue() / 100.0);
                feedbackTypeButton.setButtonText ("single feedback");
                doubleFeedback = false;
            }
        }
       
    }
    
    void setLfo3AmpMode (bool lfo3AmpMode)
    {
        if (lfo3AmpMode)
        {
            double value = lfo3AmpSlider.getValue();
            feedbackSlider.setVisible (false);
            lfo3AmpSlider.setVisible (true);
            lfo3AmpSlider.setValue (value);
            
            feedbackTypeButton.setToggleState (false, sendNotification);
            feedbackTypeButton.setEnabled (false);
        }
        else
        {
            lfo3AmpSlider.setVisible (false);
            feedbackSlider.setVisible (true);
            feedbackTypeButton.setEnabled (true);
            proc.setFeedbackGain (feedbackSlider.getValue() / 100);
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
        processingTypeButton.setBounds (25, 175, 225, 50);
        feedbackTypeButton.setBounds (275, 175, 100, 50);
        
        //================================================
        //MASTER EDITOR
        dryWetSlider.setBounds (425, 50, 100, 100);
        inputGainSlider.setBounds (425, 195, 100, 100);
        voice1MixSlider.setBounds (425, 340, 100, 100);
        voice2MixSlider.setBounds (425, 485, 100, 100);
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
    
    NormalisableRange<double> feedbackSeparateRange, feedbackCascadeRange;
    
    TextButton processingTypeButton, feedbackTypeButton;
    
    bool doubleFeedback = false;
    
    //================================================
    //MASTER EDITOR
    Slider inputGainSlider;
    Label inputGainLabel;
    
    Slider voice1MixSlider, voice2MixSlider;
    Label voice1MixLabel, voice2MixLabel;
    
    Slider dryWetSlider;
    Label dryWetLabel;
    
    MyLookAndFeel myLookAndFeel;
    //==============================================================
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment>   depthSliderAttach,
                                                                    delaySliderAttach,
                                                                    feedbackSliderAttach,
                                                                    fbLfoAmpSliderAttach,
                                                                    mixSliderAttach,
                                                                    voice1MixSliderAttach,
                                                                    voice2MixSliderAttach,
                                                                    masterSliderAttach;
    
    ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment>   procTypeButtonAttach,
                                                                    fbTypeButtonAttach;
    
    Proc& proc;
    Vibrato2AudioProcessor& p;
    //==============================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EffectEditor)
};
