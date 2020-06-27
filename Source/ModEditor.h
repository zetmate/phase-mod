/*
  ==============================================================================

    ModEditor.h
    Created: 2 Jun 2018 11:14:47am
    Author:  Macbook

  ==============================================================================
*/

#pragma once

#include "UIThemes.h"
//==============================================================================
/*
*/
class ModEditor    :    public Component,
                        public Slider::Listener,
                        public ComboBox::Listener,
                        public Button::Listener
{
public:
    ModEditor (Vibrato2AudioProcessor& p, EffectEditor& ee, int width, int height)  : proc (p.proc), p (p), effectEditor(ee)
    {
        setSize (width, height);
        
        Utility::addSlider (&freq1Slider, &freq1Label, "Voice 1 LFO", 0.01, 17, 0.01, 3,
                            "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 0.01, this, this, true,
                            &lookAndFeel);
        
        Utility::addSlider (&freq2Slider, &freq2Label, "Voice 2 LFO", 0.01, 17, 0.01, 3,
                            "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 0.01, this, this, true,
                            &lookAndFeel);
        
        Utility::addSlider (&freq3Slider, &freq3Label, "Feedback LFO", 0.01, 17, 0.01, 3,
                            "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 2, this, this, true,
                            &lookAndFeel);
        
        Utility::addSlider (&tempo1Slider, &tempo1Label, "Voice 1 LFO", -10, -1, 1, -5,
                            "", Slider::SliderStyle::LinearHorizontal,
                            Slider::TextEntryBoxPosition::TextBoxBelow, -8, this, this, false,
                            &lookAndFeel);
        
        Utility::addSlider (&tempo2Slider, &tempo2Label, "Voice 2 LFO", -10, -1, 1, -5,
                            "", Slider::SliderStyle::LinearHorizontal,
                            Slider::TextEntryBoxPosition::TextBoxBelow, -8, this, this, false,
                            &lookAndFeel);
        
        Utility::addSlider (&tempo3Slider, &tempo3Label, "Feedback LFO", -10, -1, 1, -5,
                            "", Slider::SliderStyle::LinearHorizontal,
                            Slider::TextEntryBoxPosition::TextBoxBelow, -3, this, this, false,
                            &lookAndFeel);
        
        Utility::addSlider (&phaseSlider, &phaseLabel, "Phase shift", 0, 180, 90, 90,
                            "deg", Slider::SliderStyle::LinearHorizontal,
                            Slider::TextEntryBoxPosition::TextBoxRight, 180, this, this, true,
                            &lookAndFeel);
        phaseLabel.attachToComponent (&phaseSlider, true);
        
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
        mod1ShapeMenu.setLookAndFeel (&lookAndFeel);
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
        mod2ShapeMenu.setLookAndFeel (&lookAndFeel);
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
        mod3ShapeMenu.setLookAndFeel (&lookAndFeel);
        addAndMakeVisible (mod3ShapeMenu);
        
        //note 1
        note1TypeMenu.addItem ("normal", 1);
        note1TypeMenu.addItem ("triplet", 2);
        note1TypeMenu.addItem ("dotted", 3);
        note1TypeMenu.addListener (this);
        note1TypeMenu.setJustificationType (Justification::centred);
        note1TypeMenu.setSelectedId (1);
        note1TypeMenu.setLookAndFeel (&lookAndFeel);
        addChildComponent (note1TypeMenu);
        
        //note2
        note2TypeMenu.addItem ("normal", 1);
        note2TypeMenu.addItem ("triplet", 2);
        note2TypeMenu.addItem ("dotted", 3);
        note2TypeMenu.addListener (this);
        note2TypeMenu.setJustificationType (Justification::centred);
        note2TypeMenu.setSelectedId (1);
        note2TypeMenu.setLookAndFeel (&lookAndFeel);
        addChildComponent (note2TypeMenu);
        
        //note3
        note3TypeMenu.addItem ("normal", 1);
        note3TypeMenu.addItem ("triplet", 2);
        note3TypeMenu.addItem ("dotted", 3);
        note3TypeMenu.addListener (this);
        note3TypeMenu.setJustificationType (Justification::centred);
        note3TypeMenu.setSelectedId (1);
        note3TypeMenu.setLookAndFeel (&lookAndFeel);
        addChildComponent (note3TypeMenu);
        
        Utility::addTextButton (&tempoSync1Button, "TEMPO SYNC", false, true, true, this, this,
                                &lookAndFeel);
        Utility::addTextButton (&tempoSync2Button, "TEMPO SYNC", false, true, true, this, this,
                                &lookAndFeel);
        Utility::addTextButton (&tempoSync3Button, "TEMPO SYNC", false, true, true, this, this,
                                &lookAndFeel);
        
        Utility::addTextButton (&lfo1onButton, "ON", true, true, true, this, this,
                                &lookAndFeel);
        Utility::addTextButton (&lfo2onButton, "ON", true, true, true, this, this,
                                &lookAndFeel);
        Utility::addTextButton (&lfo3onButton, "OFF", false, true, true, this, this,
                                &lookAndFeel);
        
        note3TypeMenu.setEnabled (false);
        
        tempo1Slider.updateText();
        tempo2Slider.updateText();
        tempo3Slider.updateText();
        
        Utility::setSliderEnabled (&freq3Slider, &freq3Label, false);
        Utility::setSliderEnabled (&tempo3Slider, &tempo3Label, false);
        
        //ATTACHEMENTS
        //Sliders
        lfo1FreqSliderAttach = new AudioProcessorValueTreeState::SliderAttachment (p.parameters,
                                                                                   p.lfo1FreqId,
                                                                                   freq1Slider);
        
        lfo2FreqSliderAttach = new AudioProcessorValueTreeState::SliderAttachment (p.parameters,
                                                                                   p.lfo2FreqId,
                                                                                   freq2Slider);
        
        lfo3FreqSliderAttach = new AudioProcessorValueTreeState::SliderAttachment (p.parameters,
                                                                                   p.lfo3FreqId,
                                                                                   freq3Slider);
        
        lfo1TFreqSliderAttach = new AudioProcessorValueTreeState::SliderAttachment (p.parameters,
                                                                                    p.lfo1TFreqId,
                                                                                    tempo1Slider);
        
        lfo2TFreqSliderAttach = new AudioProcessorValueTreeState::SliderAttachment (p.parameters,
                                                                                    p.lfo2TFreqId,
                                                                                    tempo2Slider);
        
        lfo3TFreqSliderAttach = new AudioProcessorValueTreeState::SliderAttachment (p.parameters,
                                                                                    p.lfo3TFreqId,
                                                                                    tempo3Slider);
        
        phaseSliderAttach = new AudioProcessorValueTreeState::SliderAttachment (p.parameters,
                                                                                p.phaseId,
                                                                                phaseSlider);
        
        //Buttons
        lfo1onButtonAttach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                                 p.lfo1onId,
                                                                                 lfo1onButton);
        
        lfo2onButtonAttach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                                 p.lfo2onId,
                                                                                 lfo2onButton);
        
        lfo3onButtonAttach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                                 p.lfo3onId,
                                                                                 lfo3onButton);
        
        tempoSync1Attach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                               p.tempoSync1Id,
                                                                               tempoSync1Button);
        
        tempoSync2Attach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                               p.tempoSync2Id,
                                                                               tempoSync2Button);
        
        tempoSync3Attach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                               p.tempoSync3Id,
                                                                               tempoSync3Button);
        
        //Combo Boxes
        lfo1TypeAttach = new AudioProcessorValueTreeState::ComboBoxAttachment (p.parameters,
                                                                               p.lfo1TypeId,
                                                                               mod1ShapeMenu);
        
        lfo2TypeAttach = new AudioProcessorValueTreeState::ComboBoxAttachment (p.parameters,
                                                                               p.lfo2TypeId,
                                                                               mod2ShapeMenu);
        
        lfo3TypeAttach = new AudioProcessorValueTreeState::ComboBoxAttachment (p.parameters,
                                                                               p.lfo3TypeId,
                                                                               mod3ShapeMenu);
        
        note1TypeAttach = new AudioProcessorValueTreeState::ComboBoxAttachment (p.parameters,
                                                                                p.note1TypeId,
                                                                                note1TypeMenu);
        
        note2TypeAttach = new AudioProcessorValueTreeState::ComboBoxAttachment (p.parameters,
                                                                                p.note2TypeId,
                                                                                note2TypeMenu);
        
        note3TypeAttach = new AudioProcessorValueTreeState::ComboBoxAttachment (p.parameters,
                                                                                p.note3TypeId,
                                                                                note3TypeMenu);
        
        
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
            ParameterControl::setFreq1 (p, value);
        }
        else if (slider == &freq2Slider)
        {
            ParameterControl::setFreq2 (p, value);
        }
        
        else if (slider == &freq3Slider)
        {
            ParameterControl::setFreq3 (p, value);
        }
        
        else if (slider == &tempo1Slider)
        {
            ParameterControl::setTFreq1 (p, value);
        }
        else if (slider == &tempo2Slider)
        {
            ParameterControl::setTFreq2 (p, value);
        }
        else if (slider == &tempo3Slider)
        {
            ParameterControl::setTFreq3 (p, value);
        }
        else if (slider == &phaseSlider)
        {
            ParameterControl::setPhase (p, value);
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
                ParameterControl::setLfo1on (p, 1);
                Utility::setSliderEnabled (&freq1Slider, &freq1Label, true);
                Utility::setSliderEnabled (&tempo1Slider, &tempo1Label, true);
                note1TypeMenu.setEnabled (true);
            }
            else
            {
                button->setButtonText ("OFF");
                ParameterControl::setLfo1on (p, 0);
                Utility::setSliderEnabled (&freq1Slider, &freq1Label, false);
                Utility::setSliderEnabled (&tempo1Slider, &tempo1Label, false);
                note1TypeMenu.setEnabled (false);
            }
        }
        else if (button == &lfo2onButton)
        {
            if (isOn)
            {
                button->setButtonText ("ON");
                ParameterControl::setLfo2on (p, 1);
                Utility::setSliderEnabled (&freq2Slider, &freq2Label, true);
                Utility::setSliderEnabled (&tempo2Slider, &tempo2Label, true);
                note2TypeMenu.setEnabled (true);
            }
            else
            {
                button->setButtonText ("OFF");
                ParameterControl::setLfo2on (p, 0);
                Utility::setSliderEnabled (&freq2Slider, &freq2Label, false);
                Utility::setSliderEnabled (&tempo2Slider, &tempo2Label, false);
                note2TypeMenu.setEnabled (false);
            }
        }
        else if (button == &lfo3onButton)
        {
            if (isOn)
            {
                button->setButtonText ("ON");
                ParameterControl::setLfo3on (p, 1);
                Utility::setSliderEnabled (&freq3Slider, &freq3Label, true);
                Utility::setSliderEnabled (&tempo3Slider, &tempo3Label, true);
                note3TypeMenu.setEnabled (true);
                
                effectEditor.setLfo3AmpMode (true);
            }
            else
            {
                button->setButtonText ("OFF");
                ParameterControl::setLfo3on (p, 0);
                proc.setFeedbackGain (effectEditor.feedbackSlider.getValue() / 100.0);
                Utility::setSliderEnabled (&freq3Slider, &freq3Label, false);
                Utility::setSliderEnabled (&tempo3Slider, &tempo3Label, false);
                note3TypeMenu.setEnabled (false);
                
                effectEditor.setLfo3AmpMode (false);
                
                proc.setFeedbackGain (effectEditor.feedbackSlider.getValue() / 100.0);
            }
        }
        else if (button == &tempoSync1Button)
        {
            if (isOn)
            {
                tempoSync1 = true;
                ParameterControl::setTempoSync1 (p, 1);
                freq1Slider.setVisible (false);
                tempo1Slider.setVisible (true);
                note1TypeMenu.setVisible (true);
            }
            else
            {
                tempoSync1 = false;
                ParameterControl::setTempoSync1 (p, 0);
                freq1Slider.setVisible (true);
                tempo1Slider.setVisible (false);
                note1TypeMenu.setVisible (false);
            }
        }
        else if (button == &tempoSync2Button)
        {
            if (isOn)
            {
                tempoSync2 = true;
                ParameterControl::setTempoSync2 (p, 1);
                freq2Slider.setVisible (false);
                tempo2Slider.setVisible (true);
                note2TypeMenu.setVisible (true);
            }
            else
            {
                tempoSync2 = false;
                ParameterControl::setTempoSync2 (p, 0);
                freq2Slider.setVisible (true);
                tempo2Slider.setVisible (false);
                note2TypeMenu.setVisible (false);
            }
        }
        else if (button == &tempoSync3Button)
        {
            if (isOn)
            {
                tempoSync3 = true;
                ParameterControl::setTempoSync3 (p, 1);
                freq3Slider.setVisible (false);
                tempo3Slider.setVisible (true);
                note3TypeMenu.setVisible (true);
            }
            else
            {
                tempoSync3 = false;
                ParameterControl::setTempoSync3 (p, 0);
                freq3Slider.setVisible (true);
                tempo3Slider.setVisible (false);
                note3TypeMenu.setVisible (false);
            }
        }
    }
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override
    {
        int i = comboBoxThatHasChanged->getSelectedId();
        
        if (comboBoxThatHasChanged == &mod1ShapeMenu)
            ParameterControl::setLfo1Shape (p, i - 1);
        
        else if (comboBoxThatHasChanged == &mod2ShapeMenu)
            ParameterControl::setLfo2Shape (p, i - 1);
        
        else if (comboBoxThatHasChanged == &mod3ShapeMenu)
            ParameterControl::setLfo3Shape (p, i - 1);
            
        else if (comboBoxThatHasChanged == &note1TypeMenu)
            ParameterControl::setNote1Type (p, i - 1);
        
        else if (comboBoxThatHasChanged == &note2TypeMenu)
            ParameterControl::setNote2Type (p, i - 1);
        
        else if (comboBoxThatHasChanged == &note3TypeMenu)
            ParameterControl::setNote3Type (p, i - 1);
        
    }

//================================================================================================
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
        
        tempo1Slider.setBounds (cell1x, row1y, 100, 50);
        tempo2Slider.setBounds (cell2x, row1y, 100, 50);
        tempo3Slider.setBounds (cell3x, row1y, 100, 50);
        
        note1TypeMenu.setBounds (cell1x, row1y + 75, 100, 25);
        note2TypeMenu.setBounds (cell2x, row1y + 75, 100, 25);
        note3TypeMenu.setBounds (cell3x, row1y + 75, 100, 25);
        
        //second row
        mod1ShapeMenu.setBounds (cell1x, row2y, 100, 25);
        mod2ShapeMenu.setBounds (cell2x, row2y, 100, 25);
        mod3ShapeMenu.setBounds (cell3x, row2y, 100, 25);
        
        //third row
        tempoSync1Button.setBounds (cell1x, row3y, 100, 30);
        tempoSync2Button.setBounds (cell2x, row3y, 100, 30);
        tempoSync3Button.setBounds (cell3x, row3y, 100, 30);
        
        //foutrh row
        phaseSlider.setBounds (cell2x, row4y, 175, 30);
    }
private:
    Slider freq1Slider;
    Label freq1Label;
    
    Slider freq2Slider;
    Label freq2Label;
    
    Slider freq3Slider;
    Label freq3Label;
    
    Slider tempo1Slider;
    Label tempo1Label;
    
    Slider tempo2Slider;
    Label tempo2Label;
    
    Slider tempo3Slider;
    Label tempo3Label;
    
    Slider phaseSlider;
    Label phaseLabel;
    
    ComboBox mod1ShapeMenu, mod2ShapeMenu, mod3ShapeMenu;
    ComboBox note1TypeMenu, note2TypeMenu, note3TypeMenu;
    TextButton tempoSync1Button, tempoSync2Button, tempoSync3Button;
    
    bool tempoSync1 = false, tempoSync2 = false, tempoSync3 = false;
    
    TextButton lfo1onButton, lfo2onButton, lfo3onButton;
    
    //for tempo sync (tempo multiplication index used for counting tripplets and dotted)
    double tmi1 = 1, tmi2 = 1, tmi3 = 1;
    
    LFMod lookAndFeel;
    //====================================================================================
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment>      lfo1FreqSliderAttach,
                                                                        lfo2FreqSliderAttach,
                                                                        lfo3FreqSliderAttach,
                                                                        lfo1TFreqSliderAttach,
                                                                        lfo2TFreqSliderAttach,
                                                                        lfo3TFreqSliderAttach,
                                                                        phaseSliderAttach;
    
    ScopedPointer <AudioProcessorValueTreeState::ButtonAttachment>      lfo1onButtonAttach,
                                                                        lfo2onButtonAttach,
                                                                        lfo3onButtonAttach,
                                                                        triplet1Attach,
                                                                        triplet2Attach,
                                                                        triplet3Attach,
                                                                        dotted1Attach,
                                                                        dotted2Attach,
                                                                        dotted3Attach,
                                                                        tempoSync1Attach,
                                                                        tempoSync2Attach,
                                                                        tempoSync3Attach,
                                                                        sync2to1Attach;
    
    ScopedPointer <AudioProcessorValueTreeState::ComboBoxAttachment>    lfo1TypeAttach,
                                                                        lfo2TypeAttach,
                                                                        lfo3TypeAttach,
                                                                        note1TypeAttach,
                                                                        note2TypeAttach,
                                                                        note3TypeAttach;
    //==============================================================
    Proc& proc;
    Vibrato2AudioProcessor& p;
    EffectEditor& effectEditor;
    //==============================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModEditor)
};
