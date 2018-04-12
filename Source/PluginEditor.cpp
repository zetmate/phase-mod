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
    
    Utility::addSlider (&feedbackSlider, &feedbackLabel, "Feedback", -100, 100, 1, 50,
                        "%", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 70, this, this, true);
    
    Utility::addSlider (&maxDelaySlider, &maxDelayLabel, "Max Delay", 25, 60, 0.01, 39,
                        "ms", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 39, this, this, true);
    
    Utility::addSlider (&sweepWidthSlider, &sweepWidthLabel, "Sweep Width", 0.4, 60, 0.01, 10,
                        "ms", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 39, this, this, true);
    
    Utility::addSlider (&smoothSlider, &smoothLabel, "Smooth", 0, 0.9, 0.01, 3,
                        " ", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 0.1, this, this, true);
    
    Utility::addSlider (&dryWetSlider, &dryWetLabel, "Mix", 0, 100, 1, 20,
                        "%", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 30, this, this, true);
}

Vibrato2AudioProcessorEditor::~Vibrato2AudioProcessorEditor()
{
}

//==============================================================================
void Vibrato2AudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if (slider == &frequencySlider)
    {
        processor.proc->wavetable->setFrequency ((float) slider->getValue());
    }
    else if (slider == &maxDelaySlider)
    {
        processor.proc->changeMaxDelayTime (slider->getValue());
    }
    else if (slider == &sweepWidthSlider)
    {
        processor.proc->changeSweepWidth (slider->getValue());
    }
}

//==============================================================================
void Vibrato2AudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void Vibrato2AudioProcessorEditor::resized()
{
    frequencySlider.setBounds (25, 50, 100, 100);
    maxDelaySlider.setBounds (150, 50, 100, 100);
    sweepWidthSlider.setBounds (275, 50, 100, 100);
}

