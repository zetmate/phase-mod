/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "Utility.h"
#include "PluginProcessor.h"
#include "EffectEditor.h"
#include "ModEditor.h"
#include "MasterEditor.h"
#include "MyLookAndFeel.h"

//==============================================================================
/**
*/
class Vibrato2AudioProcessorEditor  :   public AudioProcessorEditor

{
public:
    Vibrato2AudioProcessorEditor (Vibrato2AudioProcessor&);
    ~Vibrato2AudioProcessorEditor();
    
    void paint (Graphics&) override;
    void resized() override;

private:
    int editorWidth = 400;
    int masterEditorWidth = 150;
    int effectEditorHeight = 250;
    int modEditorHeight = 310 + 50;
    
    MasterEditor masterEditor;
    EffectEditor effectEditor;
    ModEditor modEditor;
    
    MyLookAndFeel myLookAndFeel;
//==============================================================================
    Proc& proc;
//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Vibrato2AudioProcessorEditor)
};
