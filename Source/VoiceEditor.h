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
        
        Utility::addSlider (&frequencySlider, &frequencyLabel, "Frequency", 0.01, 17, 0.001, 3,
                            "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::NoTextBox, 0.01, this, this, true);
        
        Utility::addSlider (&lowCutSlider, &lowCutLabel, "Low cut", 20, 20000, 1, 3000,
                            "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::NoTextBox, 70, this, this, true);
        
        Utility::addSlider (&highCutSlider, &highCutLabel, "High cut", 20, 20000, 1, 3000,
                            "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::NoTextBox, 20000, this, this, true);
        
        Utility::addSlider (&resoSlider, &resoLabel, "Resonance", 0.7, 80, 0.01, 10,
                            "", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::NoTextBox, 0.7, this, this, true);
        
        Utility::addSlider (&dryWetSlider, &dryWetLabel, "Mix", 0, 1, 0.01, 0.5,
                            "", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::NoTextBox, 1, this, this, true);
    }
    
    ~VoiceEditor()
    {
    }
    
    void sliderValueChanged (Slider* slider) override
    {
        if (slider == &frequencySlider)
        {
            voiceProc.wavetable->setFrequency ((float) slider->getValue());
        }
        else if (slider == &dryWetSlider)
        {
            voiceProc.setDryWetMix (slider->getValue());
        }
        else if (slider == &lowCutSlider)
        {
            voiceProc.setLowCutFrequency (slider->getValue());
        }
        else if (slider == &highCutSlider)
        {
            voiceProc.setHighCutFrequency (slider->getValue());
        }
        else if (slider == &resoSlider)
        {
            voiceProc.setResonance (slider->getValue());
        }
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
        //first row
        lowCutSlider.setBounds (25, 50, 100, 100);
        highCutSlider.setBounds (150, 50, 100, 100);
        
        //second row
        resoSlider.setBounds (25, 175, 100, 100);
        frequencySlider.setBounds (150, 175, 100, 100);
    }

private:
    Slider frequencySlider;
    Label frequencyLabel;
    
    Slider lowCutSlider;
    Label lowCutLabel;
    
    Slider highCutSlider;
    Label highCutLabel;
    
    Slider resoSlider;
    Label resoLabel;
    
    Slider dryWetSlider;
    Label dryWetLabel;
    
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
