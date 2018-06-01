/*
  ==============================================================================

    VoiceEditor.h
    Created: 17 Apr 2018 8:19:21pm
    Author:  Macbook

  ==============================================================================
*/

#pragma once

#include "Flanger.h"

//==============================================================================
/*
*/
class VoiceEditor    : public Component,
                              Slider::Listener,
                              Button::Listener
{
public:
    VoiceEditor (Flanger& f, int width, int height) : voiceProc (f)
    {
        setSize (width, height);
    }
    
    ~VoiceEditor()
    {
    }
    
    void sliderValueChanged (Slider* slider) override
    {
    }
    
    void buttonClicked (Button* button) override
    {
    }
    
    void paint (Graphics& g) override
    {
        g.fillAll (Colours::black);
        
        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);
    }

    void resized() override
    {
    }

private:
    
    //voice processor reference
    Flanger& voiceProc;
//=============================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoiceEditor)
};

//==================================================================
//==================================================================
class VoiceCloseEditor    : public VoiceEditor
{
public:
    VoiceCloseEditor (Flanger& f, int width, int height) : VoiceEditor (f, width, height)
    {
    }
    
    ~VoiceCloseEditor()
    {
    }
    
private:
    //========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoiceCloseEditor)
};

//============================================================================
class VoiceMidEditor    : public VoiceEditor
{
public:
    VoiceMidEditor (Flanger& f, int width, int height) : VoiceEditor (f, width, height)
    {
    }
    
    ~VoiceMidEditor()
    {
    }
    
private:
    //========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoiceMidEditor)
};

//============================================================================
class VoiceFarEditor    : public VoiceEditor
{
public:
    VoiceFarEditor (Flanger& f, int width, int height) : VoiceEditor (f, width, height)
    {
    }
    
    ~VoiceFarEditor()
    {
    }
    
private:
    //========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoiceFarEditor)
};

//============================================================================
class VoiceEchoEditor    : public VoiceEditor
{
public:
    VoiceEchoEditor (Flanger& f, int width, int height) : VoiceEditor (f, width, height)
    {
    }
    
    ~VoiceEchoEditor()
    {
    }
    
private:
    //========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoiceEchoEditor)
};
