/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "Utility.h"
#include "PluginProcessor.h"
#include "VoiceEditor.h"
#include "MasterEditor.h"


//==============================================================================
/**
*/
class Vibrato2AudioProcessorEditor  :   public AudioProcessorEditor,
                                        public Slider::Listener,
                                        public Button::Listener
{
public:
    Vibrato2AudioProcessorEditor (Vibrato2AudioProcessor&);
    ~Vibrato2AudioProcessorEditor();
    
    void paint (Graphics&) override;
    void sliderValueChanged (Slider* slider) override;
    void buttonClicked (Button*) override;
    void resized() override;

private:
    
    Proc& proc;
//==============================================================================
    Slider lowCutSlider;
    Label lowCutLabel;
    
    Slider freq1Slider;
    Label freq1Label;
    
    Slider freq2Slider;
    Label freq2Label;
    
    Slider depthSlider;
    Label depthLabel;
    
    Slider feedbackSlider;
    Label feedbackLabel;
    
    Slider dryWetSlider;
    Label dryWetLabel;
    
    TextButton separateProcessingButton;
    
//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Vibrato2AudioProcessorEditor)
};
