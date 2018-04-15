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
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (400, 300);
    
    Utility::addSlider (&frequencySlider, &frequencyLabel, "Frequency", 0.1, 20, 0.01, 1,
                        "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 0.1, this, this, true);
    
    Utility::addSlider (&feedbackSlider, &feedbackLabel, "Feedback", -100, 100, 1, 0,
                        "%", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 70, this, this, true);
    
    Utility::addSlider (&maxDelaySlider, &maxDelayLabel, "Max Delay", 3, 60, 0.001, 39,
                        "ms", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 39, this, this, true);
    
    Utility::addSlider (&sweepWidthSlider, &sweepWidthLabel, "Sweep Width", 0.4, 60, 0.001, 10,
                        "ms", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 39, this, this, true);
    
    Utility::addSlider (&smoothSlider, &smoothLabel, "Smooth", -0.1, 0.1, 0.01, 0,
                        " ", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 0, this, this, true);
    
    Utility::addSlider (&dryWetSlider, &dryWetLabel, "Mix", 0, 100, 1, 50,
                        "%", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 100, this, this, true);
}

Vibrato2AudioProcessorEditor::~Vibrato2AudioProcessorEditor()
{
}

//==============================================================================
void Vibrato2AudioProcessorEditor::sliderValueChanged (Slider* slider)
{
//    if (slider == &frequencySlider)
//    {
//        processor.proc.wavetable->setFrequency ((float) slider->getValue());
//    }
//    else if (slider == &maxDelaySlider)
//    {
//        processor.proc.changeMaxDelayTime (slider->getValue());
//    }
//    else if (slider == &sweepWidthSlider)
//    {
//        processor.proc.changeSweepWidth (slider->getValue());
//    }
//    else if (slider == &feedbackSlider)
//    {
//        processor.proc.setFeedbackGain (slider->getValue() / 100);
//    }
//    else if (slider == &smoothSlider)
//    {
//        processor.proc.setPrevSampleGain (slider->getValue());
//    }
//    else if (slider == &dryWetSlider)
//    {
//        processor.proc.setDryWetMix (slider->getValue() / 100);
//    }
}

//==============================================================================
void Vibrato2AudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void Vibrato2AudioProcessorEditor::resized()
{
    //first row
    frequencySlider.setBounds (25, 50, 100, 100);
    maxDelaySlider.setBounds (150, 50, 100, 100);
    sweepWidthSlider.setBounds (275, 50, 100, 100);
    
    //second row
    feedbackSlider.setBounds (25, 175, 100, 100);
    smoothSlider.setBounds (150, 175, 100, 100);
    dryWetSlider.setBounds (275, 175, 100, 100);
}

