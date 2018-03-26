/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "Utility.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class Vibrato2AudioProcessorEditor  : public AudioProcessorEditor,
                                             Slider::Listener
{
public:
    Vibrato2AudioProcessorEditor (Vibrato2AudioProcessor&);
    ~Vibrato2AudioProcessorEditor();

    //==============================================================================
    void sliderValueChanged (Slider* slider) override;
    void paint (Graphics&) override;
    void resized() override;

private:
    Slider frequencySlider;
    Label frequencyLabel;
    
    Slider feedbackSlider;
    Label feedbackLabel;
    
    Slider delaySlider;
    Label delayLabel;
    
    Slider smoothSlider;
    Label smoothLabel;
    
    Slider dryWetSlider;
    Label dryWetLabel;
    
    TextButton polarityButton;
    
//=============================================
    Vibrato2AudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Vibrato2AudioProcessorEditor)
};
