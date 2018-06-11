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
    ModEditor (Proc& p, EffectEditor& ee, int width, int height)  : proc(p), effectEditor(ee)
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
        mod1ShapeMenu.addItem ("triangle", 2);
        mod1ShapeMenu.addItem ("saw", 3);
        mod1ShapeMenu.addItem ("square", 4);
        mod1ShapeMenu.addItem ("random", 5);
        mod1ShapeMenu.addItem ("noise", 6);
        mod1ShapeMenu.addListener (this);
        mod1ShapeMenu.setJustificationType (Justification::centred);
        mod1ShapeMenu.setSelectedId (1);
        addAndMakeVisible (mod1ShapeMenu);
        
        //mod 2
        mod2ShapeMenu.addItem ("sin", 1);
        mod2ShapeMenu.addItem ("triangle", 2);
        mod2ShapeMenu.addItem ("saw", 3);
        mod2ShapeMenu.addItem ("square", 4);
        mod2ShapeMenu.addItem ("random", 5);
        mod2ShapeMenu.addItem ("noise", 6);
        mod2ShapeMenu.addListener (this);
        mod2ShapeMenu.setJustificationType (Justification::centred);
        mod2ShapeMenu.setSelectedId (1);
        addAndMakeVisible (mod2ShapeMenu);
        
        //mod 3
        mod3ShapeMenu.addItem ("sin", 1);
        mod3ShapeMenu.addItem ("triangle", 2);
        mod3ShapeMenu.addItem ("saw", 3);
        mod3ShapeMenu.addItem ("square", 4);
        mod3ShapeMenu.addItem ("random", 5);
        mod3ShapeMenu.addListener (this);
        mod3ShapeMenu.setJustificationType (Justification::centred);
        mod3ShapeMenu.setSelectedId (1);
        addAndMakeVisible (mod3ShapeMenu);
        
        Utility::addTextButton (&sync1Button, "TEMPO SYNC", false, true, this, this);
        Utility::addTextButton (&sync2Button, "TEMPO SYNC", false, true, this, this);
        Utility::addTextButton (&sync3Button, "TEMPO SYNC", false, true, this, this);
        
        Utility::addTextButton (&lfo1onButton, "ON", true, true, this, this);
        Utility::addTextButton (&lfo2onButton, "ON", true, true, this, this);
        Utility::addTextButton (&lfo3onButton, "OFF", false, true, this, this);
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
        
        else if (slider == &freq3Slider)
            proc.setFreq3 (value);
    }
    
    void buttonClicked (Button* button) override
    {
        bool isOn = button->getToggleState();
        
        if (button == &lfo1onButton)
        {
            if (isOn)
            {
                button->setToggleState (false, dontSendNotification);
                button->setButtonText ("OFF");
                proc.setLfo1on (false);
            }
            else
            {
                button->setToggleState (true, dontSendNotification);
                button->setButtonText ("ON");
                proc.setLfo1on (true);
            }
        }
        else if (button == &lfo2onButton)
        {
            if (isOn)
            {
                button->setToggleState (false, dontSendNotification);
                button->setButtonText ("OFF");
                proc.setLfo2on (false);
            }
            else
            {
                button->setToggleState (true, dontSendNotification);
                button->setButtonText ("ON");
                proc.setLfo2on (true);
            }
        }
        else if (button == &lfo3onButton)
        {
            if (isOn)
            {
                button->setToggleState (false, dontSendNotification);
                button->setButtonText ("OFF");
                proc.setLfo3on (false);
                proc.setFeedbackGain (effectEditor.feedbackSlider.getValue() / 100.0);
            }
            else
            {
                button->setToggleState (true, dontSendNotification);
                button->setButtonText ("ON");
                proc.setLfo3on (true);
            }
        }
    }
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override
    {
        int i = comboBoxThatHasChanged->getSelectedId();
        
        if (comboBoxThatHasChanged == &mod1ShapeMenu)
        {
            switch (i)
            {
                case 1:
                    proc.setlfoShape1 (Flanger::LfoShape::sin);
                    break;
                    
                case 2:
                    proc.setlfoShape1 (Flanger::LfoShape::triangle);
                    break;
                
                case 3:
                    proc.setlfoShape1 (Flanger::LfoShape::saw);
                    break;
                
                case 4:
                    proc.setlfoShape1 (Flanger::LfoShape::square);
                    break;
                
                case 5:
                    proc.setlfoShape1 (Flanger::LfoShape::random);
                    break;
                
                case 6:
                    proc.setlfoShape1 (Flanger::LfoShape::noise);
                    break;
                    
                default:
                    break;
            }
        }
        else if (comboBoxThatHasChanged == &mod2ShapeMenu)
        {
            switch (i)
            {
                case 1:
                    proc.setlfoShape2 (Flanger::LfoShape::sin);
                    break;
                    
                case 2:
                    proc.setlfoShape2 (Flanger::LfoShape::triangle);
                    break;
                    
                case 3:
                    proc.setlfoShape2 (Flanger::LfoShape::saw);
                    break;
                    
                case 4:
                    proc.setlfoShape2 (Flanger::LfoShape::square);
                    break;
                    
                case 5:
                    proc.setlfoShape2 (Flanger::LfoShape::random);
                    break;
                    
                case 6:
                    proc.setlfoShape2 (Flanger::LfoShape::noise);
                    break;
                    
                default:
                    break;
            }
        }
        else if (comboBoxThatHasChanged == &mod3ShapeMenu)
        {
            switch (i)
            {
                case 1:
                    proc.setlfoShape3 (Flanger::LfoShape::sin);
                    break;
                    
                case 2:
                    proc.setlfoShape3 (Flanger::LfoShape::triangle);
                    break;
                    
                case 3:
                    proc.setlfoShape3 (Flanger::LfoShape::saw);
                    break;
                    
                case 4:
                    proc.setlfoShape3 (Flanger::LfoShape::square);
                    break;
                    
                case 5:
                    proc.setlfoShape3 (Flanger::LfoShape::random);
                    break;
                    
                default:
                    break;
            }
        }
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
        sync1Button.setBounds (25, 210, 100, 30);
        sync2Button.setBounds (150, 210, 100, 30);
        sync3Button.setBounds (275, 210, 100, 30);
        
        lfo1onButton.setBounds (50, 255, 50, 30);
        lfo2onButton.setBounds (175, 255, 50, 30);
        lfo3onButton.setBounds (300, 255, 50, 30);
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
    TextButton lfo1onButton, lfo2onButton, lfo3onButton;
    //==============================================================
    Proc& proc;
    EffectEditor& effectEditor;
    //==============================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModEditor)
};
