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
    : AudioProcessorEditor (&p),
        effectEditor (p.proc, editorWidth, effectEditorHeight),
        modEditor (p.proc, editorWidth, modEditorHeight),
        masterEditor (p.proc, masterEditorWidth, effectEditorHeight + modEditorHeight),
        proc (p.proc)
{
    setSize (editorWidth + masterEditorWidth, effectEditorHeight + modEditorHeight);
    
    addAndMakeVisible (effectEditor);
    addAndMakeVisible (modEditor);
    addAndMakeVisible (masterEditor);
}

Vibrato2AudioProcessorEditor::~Vibrato2AudioProcessorEditor()
{
}

void Vibrato2AudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void Vibrato2AudioProcessorEditor::resized()
{
    effectEditor.setBounds (0, 0,
                            effectEditor.getWidth(),
                            effectEditor.getHeight());
    
    modEditor.setBounds (0, effectEditorHeight,
                         modEditor.getWidth(),
                         modEditor.getHeight());
    
    masterEditor.setBounds (editorWidth, 0,
                            masterEditor.getWidth(),
                            masterEditor.getHeight());
}

