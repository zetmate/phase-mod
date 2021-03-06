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
        effectEditor (p, editorWidth + masterEditorWidth,
                      effectEditorHeight + modEditorHeight),
        modEditor (p, effectEditor, editorWidth, modEditorHeight),
        proc (p.proc), p (p)
{
    setSize (editorWidth + masterEditorWidth, effectEditorHeight + modEditorHeight);
    
    addAndMakeVisible (effectEditor);
    addAndMakeVisible (modEditor);
    
}

Vibrato2AudioProcessorEditor::~Vibrato2AudioProcessorEditor()
{
}

void Vibrato2AudioProcessorEditor::paint (Graphics& g)
{
    ColourGradient BGradient;
    g.setGradientFill(BGradient);
}

void Vibrato2AudioProcessorEditor::resized()
{
    effectEditor.setBounds (0, 0,
                            effectEditor.getWidth(),
                            effectEditor.getHeight());
    
    modEditor.setBounds (0, effectEditorHeight,
                         modEditor.getWidth(),
                         modEditor.getHeight());
}

