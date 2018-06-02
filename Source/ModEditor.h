/*
  ==============================================================================

    ModEditor.h
    Created: 2 Jun 2018 11:14:47am
    Author:  Macbook

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ModEditor    :    public Component,
                        public Slider::Listener,
                        public ComboBox::Listener,
                        public Button::Listener
{
public:
    ModEditor (Proc& p, int width, int height)  : proc(p)
    {
        setSize (width, height);
        
        Utility::addSlider (&freq1Slider, &freq1Label, "Frequency 1", 0.01, 17, 0.001, 3,
                            "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 0.01, this, this, true);
        
        Utility::addSlider (&freq2Slider, &freq2Label, "Frequency 2", 0.01, 17, 0.001, 3,
                            "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 0.01, this, this, true);
        
        Utility::addSlider (&freq3Slider, &freq3Label, "Frequency 3", 0.01, 17, 0.001, 3,
                            "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 0.01, this, this, true);
        
        //mod 1
        mod1ShapeMenu.addItem ("sin", 1);
        mod1ShapeMenu.addItem ("sawtooth", 2);
        mod1ShapeMenu.addItem ("square", 3);
        mod1ShapeMenu.addItem ("random", 4);
        mod1ShapeMenu.addListener (this);
        addAndMakeVisible (mod1ShapeMenu);
        
        //mod 2
        mod2ShapeMenu.addItem ("sin", 1);
        mod2ShapeMenu.addItem ("sawtooth", 2);
        mod2ShapeMenu.addItem ("square", 3);
        mod2ShapeMenu.addItem ("random", 4);
        mod2ShapeMenu.addListener (this);
        addAndMakeVisible (mod2ShapeMenu);
        
        //mod 3
        mod3ShapeMenu.addItem ("sin", 1);
        mod3ShapeMenu.addItem ("sawtooth", 2);
        mod3ShapeMenu.addItem ("square", 3);
        mod3ShapeMenu.addItem ("random", 4);
        mod3ShapeMenu.addListener (this);
        addAndMakeVisible (mod3ShapeMenu);
        
        Utility::addTextButton (&sync1Button, "SYNC", false, true, this, this);
        Utility::addTextButton (&sync2Button, "SYNC", false, true, this, this);
        Utility::addTextButton (&sync3Button, "SYNC", false, true, this, this);
    }

    ~ModEditor()
    {
    }
    
    //===================================================================================
    void sliderValueChanged (Slider* slider) override
    {
        double value = slider->getValue();
        
        if (slider == &freq1Slider)
            proc.setFreq1 (value);
        
        else if (slider == &freq2Slider)
            proc.setFreq2 (value);
    }
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override
    {
    }
    
    void buttonClicked (Button* button) override
    {
    }
    //===================================================================================

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::black);
        
        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);
    }

    void resized() override
    {
        //first row
        freq1Slider.setBounds (25, 50, 100, 100);
        freq2Slider.setBounds (150, 50, 100, 100);
        freq3Slider.setBounds (275, 50, 100, 100);
        
        //second row
        mod1ShapeMenu.setBounds (25, 175, 100, 25);
        mod2ShapeMenu.setBounds (150, 175, 100, 25);
        mod3ShapeMenu.setBounds (275, 175, 100, 25);
        
        //third row
        sync1Button.setBounds (50, 210, 50, 30);
        sync2Button.setBounds (175, 210, 50, 30);
        sync3Button.setBounds (300, 210, 50, 30);
    }

private:
    Slider freq1Slider;
    Label freq1Label;
    
    Slider freq2Slider;
    Label freq2Label;
    
    Slider freq3Slider;
    Label freq3Label;
    
    ComboBox mod1ShapeMenu, mod2ShapeMenu, mod3ShapeMenu;
    TextButton sync1Button, sync2Button, sync3Button;
    //==============================================================
    Proc& proc;
    //==============================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModEditor)
};
