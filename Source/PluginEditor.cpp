/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Vibrato2AudioProcessorEditor::Vibrato2AudioProcessorEditor (Vibrato2AudioProcessor& p)
    : AudioProcessorEditor (&p), proc (p.proc)
{
    setSize (525, 550);
    
    //add sliders
    Utility::addSlider (&freq1Slider, &freq1Label, "Frequency 1", 0.01, 17, 0.001, 3,
                        "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 0.01, this, this, true);
    
    Utility::addSlider (&freq2Slider, &freq2Label, "Frequency 2", 0.01, 17, 0.001, 3,
                        "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 0.01, this, this, true);
    
    Utility::addSlider (&lowCutSlider, &lowCutLabel, "Low cut 1", 20, 20000, 1, 3000,
                        "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 70, this, this, true);
    
    Utility::addSlider (&depthSlider, &depthLabel, "Depth", 0, 100, 1, 50,
                        "%", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 30, this, this, true);
    
    Utility::addSlider (&feedbackSlider, &feedbackLabel, "Feedback", 0, 99, 1, 50,
                        "%", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 30, this, this, true);
    
    Utility::addSlider (&dryWetSlider, &dryWetLabel, "Mix", 0, 100, 1, 50,
                        "%", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 50, this, this, true);
    
    //add buttons
    Utility::addTextButton (&separateProcessingButton, "separate processing",
                            true, true, this, this);
    
//    Utility::addTextButton (&doubleFeedbackButton, "single feedback",
//                            false, true, this, this);
}

Vibrato2AudioProcessorEditor::~Vibrato2AudioProcessorEditor()
{
}
//==============================================================================
void Vibrato2AudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    double value = slider->getValue();
    
    if (slider == &freq1Slider)
        proc.setFreq1 (value);
    
    else if (slider == &freq2Slider)
        proc.setFreq2 (value);
    
    else if (slider == &lowCutSlider)
        proc.setLowCut (value);
    
    else if (slider == &depthSlider)
        proc.setDepth (value / 100.0);
    
    else if (slider == &feedbackSlider)
        proc.setFeedbackGain (value / 100.0);
    
    else if (slider == &dryWetSlider)
        proc.setDryWet (value / 100.0);
}

void Vibrato2AudioProcessorEditor::buttonClicked(juce::Button* button)
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

//==============================================================================//23.75
void Vibrato2AudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void Vibrato2AudioProcessorEditor::resized()
{
//first row
    //25 - 150 - 275 - 400
}

