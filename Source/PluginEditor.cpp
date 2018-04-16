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
    : AudioProcessorEditor (&p), processor (p),
        voiceWidth (275), voiceHeight (350),
        width (voiceWidth * 3), height (voiceHeight * 2),
        voiceCloseEditor (p.proc.voiceClose, voiceWidth, voiceHeight),
        voiceMidEditor (p.proc.voiceMid, voiceWidth, voiceHeight),
        voiceFarEditor (p.proc.voiceFar, voiceWidth, voiceHeight),
        voiceEchoEditor (p.proc.voiceEcho, voiceWidth, voiceHeight),
        masterEditor (p.proc, width, height)
{
    setSize (width, height);
    
    addAndMakeVisible (voiceCloseEditor);
    addAndMakeVisible (voiceMidEditor);
    addAndMakeVisible (voiceFarEditor);
    addAndMakeVisible (voiceEchoEditor);
    
    addAndMakeVisible (masterEditor);
}

Vibrato2AudioProcessorEditor::~Vibrato2AudioProcessorEditor()
{
}

//==============================================================================
void Vibrato2AudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void Vibrato2AudioProcessorEditor::resized()
{
//voices section
    
    //first row
    voiceCloseEditor.setBounds (0, 0, voiceWidth, voiceHeight);
    voiceMidEditor.setBounds (voiceWidth, 0, voiceWidth, voiceHeight);
    
    //second row
    voiceFarEditor.setBounds (0, voiceHeight, voiceWidth, voiceHeight);
    voiceEchoEditor.setBounds (voiceWidth, voiceHeight, voiceWidth, voiceHeight);
    
//master section
    masterEditor.setBounds (voiceWidth * 2, 0, voiceWidth, getHeight());
    
}

