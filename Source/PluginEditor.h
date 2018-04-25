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
class Vibrato2AudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Vibrato2AudioProcessorEditor (Vibrato2AudioProcessor&);
    ~Vibrato2AudioProcessorEditor();
    
    void paint (Graphics&) override;
    void resized() override;

private:
    
    Vibrato2AudioProcessor& processor;
//=============================================
    //size values
    int voiceWidth;
    int voiceHeight;
    
    int width;
    int height;
//=============================================
    //child editors
    VoiceCloseEditor voiceCloseEditor;
    VoiceMidEditor voiceMidEditor;
    VoiceFarEditor voiceFarEditor;
    VoiceEchoEditor voiceEchoEditor;
    
    MasterEditor masterEditor;
//=============================================
    Slider frequencySlider;
    Label frequencyLabel;
    
    Slider feedbackSlider;
    Label feedbackLabel;
    
    Slider maxDelaySlider;
    Label maxDelayLabel;
    
    Slider sweepWidthSlider;
    Label sweepWidthLabel;
    
    Slider smoothSlider;
    Label smoothLabel;
    
    Slider dryWetSlider;
    Label dryWetLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Vibrato2AudioProcessorEditor)
};
