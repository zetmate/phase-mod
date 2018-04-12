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
    setSize (600, 400);
    
    Utility::addSlider (&frequencySlider, &frequencyLabel, "Frequency", 0.1, 19, 0.01, 5000,
                        "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 0.1, this, this, true);
    
    Utility::addSlider (&feedbackSlider, &feedbackLabel, "Feedback", -100, 100, 1, 50,
                        "%", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 70, this, this, true);
    
    Utility::addSlider (&delaySlider, &delayLabel, "Delay", 3, 40, 0.1, 15,
                        "ms", Slider::SliderStyle::RotaryVerticalDrag,
                        Slider::TextEntryBoxPosition::TextBoxBelow, 7, this, this, true);
    
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
        processor.proc->wavetable->valueHasChanged();
    }
    else if (slider == &delaySlider)
    {
        processor.proc->changeMaxDelayTime (delaySlider.getValue());
    }
}

//==============================================================================
void Vibrato2AudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void Vibrato2AudioProcessorEditor::resized()
{
    frequencySlider.setBounds (100, 50, 100, 100);
    delaySlider.setBounds (350, 50, 100, 100);
    
}
