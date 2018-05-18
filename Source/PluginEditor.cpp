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
    
    Utility::addSlider (&lowCut1Slider, &lowCut1Label, "Low cut 1", 20, 20000, 1, 3000,
                        "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 70, this, this, true);
    
    Utility::addSlider (&lowCut2Slider, &lowCut2Label, "Low cut 2", 20, 20000, 1, 3000,
                        "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 70, this, this, true);
    
    Utility::addSlider (&highCut1Slider, &highCut1Label, "High cut 1", 20, 20000, 1, 3000,
                        "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 20000, this, this, true);
    
    Utility::addSlider (&highCut2Slider, &highCut2Label, "High cut 2", 20, 20000, 1, 3000,
                        "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 20000, this, this, true);
    
    Utility::addSlider (&resoSlider, &resoLabel, "Resonance", 0.7, 50, 0.01, 10,
                        "", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 0.7, this, this, true);
    
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
    
    Utility::addTextButton (&feedbackPolarityButton, "negative",
                            false, true, this, this);
    
    Utility::addTextButton (&doubleFeedbackButton, "single feedback",
                            false, true, this, this);
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
    
    else if (slider == &lowCut1Slider)
        proc.setLowCut1 (value);
    
    else if (slider == &lowCut2Slider)
        proc.setLowCut2 (value);
    
    else if (slider == &highCut1Slider)
        proc.setHighCut1 (value);
    
    else if (slider == &highCut2Slider)
        proc.setHighCut2 (value);
    
    else if (slider == &resoSlider)
        proc.setReso (value);
    
    else if (slider == &depthSlider)
        proc.setDepth (value / 100.0);
    
    else if (slider == &feedbackSlider)
        proc.setFeedbackGain (value / 100.0);
    
    else if (slider == &dryWetSlider)
        proc.setGlobalDryWet (value / 100.0);
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
            proc.setPrevSampleGain (0);
            proc.setFeedbackGain (feedbackSlider.getValue() / 100);
            feedbackSlider.setEnabled (true);
        }
        else
        {
            button->setToggleState (true, dontSendNotification);
            button->setButtonText ("double feedback");
            proc.setFeedbackGain (0.7);
            proc.setPrevSampleGain (0.3);
            feedbackSlider.setEnabled (false);
        }
    }
}

//==============================================================================//23.75
void Vibrato2AudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void Vibrato2AudioProcessorEditor::resized()
{
//first row
    lowCut1Slider.setBounds (25, 50, 100, 100);
    highCut1Slider.setBounds (150, 50, 100, 100);
    lowCut2Slider.setBounds (275, 50, 100, 100);
    highCut2Slider.setBounds (400, 50, 100, 100);
    
//second row
    freq1Slider.setBounds (25, 200, 100, 100);
    freq2Slider.setBounds (150, 200, 100, 100);
    depthSlider.setBounds (275, 200, 100, 100);
    resoSlider.setBounds (400, 200, 100, 100);
    
//third row
    feedbackSlider.setBounds (25, 350, 100, 100);
    separateProcessingButton.setBounds (215, 323.75, 100, 35);
    feedbackPolarityButton.setBounds (215, 381.75, 100, 35);
    doubleFeedbackButton.setBounds (215, 440.5, 100, 35);
    dryWetSlider.setBounds (400, 350, 100, 100);
}

