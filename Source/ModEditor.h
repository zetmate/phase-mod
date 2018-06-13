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
        
        Utility::addSlider (&freq1Slider, &freq1Label, "Voice 1 LFO", 0.01, 17, 0.01, 3,
                            "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 0.01, this, this, true);
        
        Utility::addSlider (&freq2Slider, &freq2Label, "Voice 2 LFO", 0.01, 17, 0.01, 3,
                            "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 0.01, this, this, true);
        
        Utility::addSlider (&freq3Slider, &freq3Label, "Feedback LFO", 0.01, 17, 0.01, 3,
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
        
        Utility::addTextButton (&sync1Button, "TEMPO SYNC", false, true, true, this, this);
        Utility::addTextButton (&sync2Button, "TEMPO SYNC", false, true, true, this, this);
        Utility::addTextButton (&sync3Button, "TEMPO SYNC", false, true, true, this, this);
        
        Utility::addTextButton (&lfo1onButton, "ON", true, true, true, this, this);
        Utility::addTextButton (&lfo2onButton, "ON", true, true, true, this, this);
        Utility::addTextButton (&lfo3onButton, "OFF", false, true, true, this, this);
        
        Utility::addTextButton (&sync2to1Button, "sync 1&2", false, true, true, this, this);
        Utility::addTextButton (&syncAllButton, "sync all", false, true, true, this, this);
        
        phase0.setRadioGroupId (1);
        phase90.setRadioGroupId (1);
        phase180.setRadioGroupId (1);
        phase0.addListener (this);
        phase90.addListener (this);
        phase180.addListener (this);
        phase0.setButtonText ("0");
        phase90.setButtonText ("90");
        phase180.setButtonText ("180");
        addChildComponent (phase0);
        addChildComponent (phase90);
        addChildComponent (phase180);
        
        Utility::setSliderEnabled (&freq3Slider, &freq3Label, false);
    }

    ~ModEditor()
    {
    }
    //===================================================================================
    void sliderValueChanged (Slider* slider) override
    {
        double value = slider->getValue();
        
        if (slider == &freq1Slider)
        {
            proc.setFreq1 (value);
            if (synced2to1)
            {
                freq2Slider.setValue (value);
                if (syncedAll)
                    freq3Slider.setValue (value);
            }
        }
        
        else if (slider == &freq2Slider)
        {
            proc.setFreq2 (value);
            if (synced2to1)
            {
                freq1Slider.setValue (value);
                if (syncedAll)
                    proc.setFreq3 (value);
            }
        }
        
        else if (slider == &freq3Slider)
        {
            proc.setFreq3 (value);
            if (syncedAll)
            {
                freq1Slider.setValue (value);
            }
        }
    }
    
    void buttonClicked (Button* button) override
    {
        bool isOn = button->getToggleState();
        
        if (button == &lfo1onButton)
        {
            if (isOn)
            {
                button->setButtonText ("ON");
                proc.setLfo1on (true);
                Utility::setSliderEnabled (&freq1Slider, &freq1Label, true);
            }
            else
            {
                button->setButtonText ("OFF");
                proc.setLfo1on (false);
                Utility::setSliderEnabled (&freq1Slider, &freq1Label, false);
            }
        }
        else if (button == &lfo2onButton)
        {
            if (isOn)
            {
                button->setButtonText ("ON");
                proc.setLfo2on (true);
                Utility::setSliderEnabled (&freq2Slider, &freq2Label, true);
            }
            else
            {
                button->setButtonText ("OFF");
                proc.setLfo2on (false);
                Utility::setSliderEnabled (&freq2Slider, &freq2Label, false);
            }
        }
        else if (button == &lfo3onButton)
        {
            if (isOn)
            {
                button->setButtonText ("ON");
                proc.setLfo3on (true);
                Utility::setSliderEnabled (&freq3Slider, &freq3Label, true);
                Utility::setSliderEnabled (&effectEditor.feedbackSlider,
                                           &effectEditor.feedbackLabel, false);
                //effectEditor.feedbackSlider.setVisible (false);
                //effectEditor.lfo3AmpSlider.setVisible (true);
                effectEditor.doubleFeedbackButton.setToggleState (false, sendNotification);
                effectEditor.doubleFeedbackButton.setEnabled (false);
                //proc.setLfo3Amp (effectEditor.lfo3AmpSlider.getValue() / 100.0);
            }
            else
            {
                button->setButtonText ("OFF");
                proc.setLfo3on (false);
                proc.setFeedbackGain (effectEditor.feedbackSlider.getValue() / 100.0);
                Utility::setSliderEnabled (&freq3Slider, &freq3Label, false);
                Utility::setSliderEnabled (&effectEditor.feedbackSlider,
                                           &effectEditor.feedbackLabel, true);
//                effectEditor.lfo3AmpSlider.setVisible (false);
//                effectEditor.feedbackSlider.setVisible (true);
                effectEditor.doubleFeedbackButton.setEnabled (true);
                proc.setFeedbackGain (effectEditor.feedbackSlider.getValue() / 100.0);
            }
        }
        else if (button == &sync2to1Button)
        {
            if (isOn)
            {
                synced2to1 = true;
                freq2Slider.setValue (freq1Slider.getValue());
                phase0.setVisible (true);
                phase90.setVisible (true);
                phase180.setVisible (true);
                phase0.setToggleState (true, dontSendNotification);
            }
            else
            {
                synced2to1 = false;
                phase0.setVisible (false);
                phase90.setVisible (false);
                phase180.setVisible (false);
                proc.setPhaseShift (Proc::PhaseShift::deg0);
            }
        }
        else if (button == &syncAllButton)
        {
            if (isOn)
            {
                syncedAll = true;
                freq3Slider.setValue (freq1Slider.getValue());
                freq2Slider.setValue (freq1Slider.getValue());
                sync2to1Button.setToggleState (true, sendNotification);
            }
            else
            {
                syncedAll = false;
            }
        }
        else if (button == &phase0 && isOn)
        {
            proc.setPhaseShift (Proc::PhaseShift::deg0);
        }
        else if (button == &phase90 && isOn)
        {
            proc.setPhaseShift (Proc::PhaseShift::deg90);
        }
        else if (button == &phase180 && isOn)
        {
            proc.setPhaseShift (Proc::PhaseShift::deg180);
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

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::black);
        
        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);
    }

    void resized() override
    {
        int row1y = 50 + 50;
        int row2y = 175 + 50;
        int row3y = 210 + 50;
        int row4y = 255 + 50;
        
        int cell1x = 25;
        int cell2x = 150;
        int cell3x = 275;
        
        //addrow
        lfo1onButton.setBounds (50, 25, 50, 30);
        lfo2onButton.setBounds (175, 25, 50, 30);
        lfo3onButton.setBounds (300, 25, 50, 30);
        
        
        //first row
        freq1Slider.setBounds (cell1x, row1y, 100, 100);
        freq2Slider.setBounds (cell2x, row1y, 100, 100);
        freq3Slider.setBounds (cell3x, row1y, 100, 100);
        
        //second row
        mod1ShapeMenu.setBounds (cell1x, row2y, 100, 25);
        mod2ShapeMenu.setBounds (cell2x, row2y, 100, 25);
        mod3ShapeMenu.setBounds (cell3x, row2y, 100, 25);
        
        //third row
        sync1Button.setBounds (cell1x, row3y, 100, 30);
        sync2Button.setBounds (cell2x, row3y, 100, 30);
        sync3Button.setBounds (cell3x, row3y, 100, 30);
        
        //foutrh row
        sync2to1Button.setBounds (cell1x + 15, row4y, 70, 25);
        phase0.setBounds (cell2x, row4y, 25, 25);
        phase90.setBounds (cell2x + 37, row4y, 25, 25);
        phase180.setBounds (cell2x + 75, row4y, 25, 25);
        syncAllButton.setBounds (cell3x + 15, row4y, 70, 25);
        
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
    
    bool synced2to1 = false, syncedAll = false;
    TextButton sync2to1Button, syncAllButton;
    TextButton lfo1onButton, lfo2onButton, lfo3onButton;
    ToggleButton phase0, phase90, phase180;
    //==============================================================
    Proc& proc;
    EffectEditor& effectEditor;
    //==============================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModEditor)
};
