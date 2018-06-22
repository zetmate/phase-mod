/*
  ==============================================================================

    ModEditor.h
    Created: 2 Jun 2018 11:14:47am
    Author:  Macbook

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MyLookAndFeel.h"
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
                            Slider::TextEntryBoxPosition::TextBoxBelow, 0.01, this, this, true);
        
        Utility::addSlider (&freq2Slider, &freq2Label, "Voice 2 LFO", 0.01, 17, 0.01, 3,
                            "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 0.01, this, this, true);
        
        Utility::addSlider (&freq3Slider, &freq3Label, "Feedback LFO", 0.01, 17, 0.01, 3,
                            "Hz", Slider::SliderStyle::RotaryVerticalDrag,
                            Slider::TextEntryBoxPosition::TextBoxBelow, 2, this, this, true);
        
        Utility::addSlider (&tempo1Slider, &tempo1Label, "Voice 1 LFO", -10, -1, 1, -5,
                            "", Slider::SliderStyle::LinearHorizontal,
                            Slider::TextEntryBoxPosition::TextBoxBelow, -9, this, this, false);
        
        Utility::addSlider (&tempo2Slider, &tempo2Label, "Voice 2 LFO", -10, -1, 1, -5,
                            "", Slider::SliderStyle::LinearHorizontal,
                            Slider::TextEntryBoxPosition::TextBoxBelow, -9, this, this, false);
        
        Utility::addSlider (&tempo3Slider, &tempo3Label, "Feedback LFO", -10, -1, 1, -5,
                            "", Slider::SliderStyle::LinearHorizontal,
                            Slider::TextEntryBoxPosition::TextBoxBelow, -4, this, this, false);
        
        Utility::addTextButton (&triplet1Button, "T", false, true, true, this, this);
        Utility::addTextButton (&triplet2Button, "T", false, true, true, this, this);
        Utility::addTextButton (&triplet3Button, "T", false, true, true, this, this);
        Utility::addTextButton (&dotted1Button, "D", false, true, true, this, this);
        Utility::addTextButton (&dotted2Button, "D", false, true, true, this, this);
        Utility::addTextButton (&dotted3Button, "D", false, true, true, this, this);
        triplet1Button.setVisible (false);
        triplet2Button.setVisible (false);
        triplet3Button.setVisible (false);
        dotted1Button.setVisible (false);
        dotted2Button.setVisible (false);
        dotted3Button.setVisible (false);
        
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
        
        Utility::addTextButton (&tempoSync1Button, "TEMPO SYNC", false, true, true, this, this);
        Utility::addTextButton (&tempoSync2Button, "TEMPO SYNC", false, true, true, this, this);
        Utility::addTextButton (&tempoSync3Button, "TEMPO SYNC", false, true, true, this, this);
        
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
        phase0.setClickingTogglesState (true);
        phase90.setClickingTogglesState (true);
        phase180.setClickingTogglesState (true);
        phase90.setToggleState (false, dontSendNotification);
        phase180.setToggleState (false, dontSendNotification);
        phase0.setToggleState (true, sendNotification);
        addAndMakeVisible (phase0);
        addAndMakeVisible (phase90);
        addAndMakeVisible (phase180);
        
        setAllButtonsLookAndFeel (&myLookAndFeel);
        
        Utility::setSliderEnabled (&freq3Slider, &freq3Label, false);
        Utility::setSliderEnabled (&tempo3Slider, &tempo3Label, false);
        triplet3Button.setEnabled (false);
        dotted3Button.setEnabled (false);
        
        freq1 = freq1Slider.getValue();
        freq2 = freq2Slider.getValue();
        freq3 = freq3Slider.getValue();
        
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
        
        triplet1Attach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                             p.triplet1Id,
                                                                             triplet1Button);
        
        triplet2Attach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                             p.triplet2Id,
                                                                             triplet2Button);
        
        triplet3Attach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                             p.triplet3Id,
                                                                             triplet3Button);
        
        dotted1Attach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                            p.dotted1Id,
                                                                            dotted1Button);
        
        dotted2Attach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                            p.dotted2Id,
                                                                            dotted2Button);
        
        dotted3Attach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                            p.dotted3Id,
                                                                            dotted3Button);
        
        tempoSync1Attach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                               p.tempoSync1Id,
                                                                               tempoSync1Button);
        
        tempoSync2Attach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                               p.tempoSync2Id,
                                                                               tempoSync2Button);
        
        tempoSync3Attach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                               p.tempoSync3Id,
                                                                               tempoSync3Button);
        
        sync2to1Attach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                             p.sync2to1Id,
                                                                             sync2to1Button);
        
        syncAllAttach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                            p.syncAllId,
                                                                            syncAllButton);
        
        phase0Attach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                           p.phase0Id,
                                                                           phase0);
        
        phase90Attach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                            p.phase90Id,
                                                                            phase90);
        
        phase180Attach = new AudioProcessorValueTreeState::ButtonAttachment (p.parameters,
                                                                             p.phase180Id,
                                                                             phase180);
        
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
            freq1 = value;
        }
        
        else if (slider == &freq2Slider)
        {
            ParameterControl::setFreq2 (p, value);
            if (!synced2to1)
                freq2 = value;
        }
        
        else if (slider == &freq3Slider)
        {
            ParameterControl::setFreq3 (p, value);
            if (!syncedAll)
                freq3 = value;
        }
        
        else if (slider == &tempo1Slider)
        {
            freq1 = ParameterControl::setTFreq1 (p, value);
            if (synced2to1)
            {
                freq2 = freq1;
                if (syncedAll)
                    freq3 = freq1;
            }
        }
        else if (slider == &tempo2Slider)
        {
            freq2 = ParameterControl::setTFreq1 (p, value);
        }
        else if (slider == &tempo3Slider)
        {
            freq3 = ParameterControl::setTFreq1 (p, value);
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
                triplet1Button.setEnabled (true);
                dotted1Button.setEnabled (true);
            }
            else
            {
                button->setButtonText ("OFF");
                ParameterControl::setLfo1on (p, 0);
                Utility::setSliderEnabled (&freq1Slider, &freq1Label, false);
                Utility::setSliderEnabled (&tempo1Slider, &tempo1Label, false);
                triplet1Button.setEnabled (false);
                dotted1Button.setEnabled (false);
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
                triplet2Button.setEnabled (true);
                dotted2Button.setEnabled (true);
            }
            else
            {
                button->setButtonText ("OFF");
                ParameterControl::setLfo2on (p, 0);
                Utility::setSliderEnabled (&freq2Slider, &freq2Label, false);
                Utility::setSliderEnabled (&tempo2Slider, &tempo2Label, false);
                triplet2Button.setEnabled (false);
                dotted2Button.setEnabled (false);
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
                triplet3Button.setEnabled (true);
                dotted3Button.setEnabled (true);
                
                effectEditor.setLfo3AmpMode (true);
            }
            else
            {
                button->setButtonText ("OFF");
                ParameterControl::setLfo3on (p, 0);
                proc.setFeedbackGain (effectEditor.feedbackSlider.getValue() / 100.0);
                Utility::setSliderEnabled (&freq3Slider, &freq3Label, false);
                Utility::setSliderEnabled (&tempo3Slider, &tempo3Label, false);
                triplet3Button.setEnabled (false);
                dotted3Button.setEnabled (false);
                
                effectEditor.setLfo3AmpMode (false);
                
                proc.setFeedbackGain (effectEditor.feedbackSlider.getValue() / 100.0);
            }
        }
        else if (button == &sync2to1Button)
        {
            if (isOn)
            {
                synced2to1 = true;
                freq2Label.setText ("Synced with LFO1", dontSendNotification);
                freq2Slider.setEnabled (false);
            }
            else
            {
                synced2to1 = false;
                freq2Label.setText ("Voice 2 LFO", dontSendNotification);
                if (lfo2onButton.getToggleState())
                    freq2Slider.setEnabled (true);
                    
                syncAllButton.setToggleState (false, sendNotification);
            }
        }
        else if (button == &syncAllButton)
        {
            if (isOn)
            {
                syncedAll = true;
                
                freq2Label.setText ("Synced with LFO1", dontSendNotification);
                freq2Slider.setEnabled (false);
                
                freq3Label.setText ("Synced with LFO1", dontSendNotification);
                freq3Slider.setEnabled (false);
            }
            else
            {
                syncedAll = false;
                
                freq3Label.setText ("Voice 3 LFO", dontSendNotification);
                if (lfo3onButton.getToggleState())
                    freq3Slider.setEnabled (true);
            }
        }
        else if (button == &tempoSync1Button)
        {
            if (isOn)
            {
                tempoSync1 = true;

                freq1Slider.setVisible (false);
                tempo1Slider.setVisible (true);
                triplet1Button.setVisible (true);
                dotted1Button.setVisible (true);
                
                if (synced2to1)
                {
                    bool triplet = triplet1Button.getToggleState();
                    bool dotted = dotted1Button.getToggleState();
                    triplet2Button.setToggleState (triplet, sendNotification);
                    dotted2Button.setToggleState (dotted, sendNotification);
                    tempoSync2Button.setToggleState (true, sendNotification);
                    if (syncedAll)
                        tempoSync3Button.setToggleState (true, sendNotification);
                }
                
                double bpm = proc.getCurrentBpm();
                tempo1Slider.setValue (Utility::freqToTempoIndex (freq1, bpm) * -1);
            }
            else
            {
                tempoSync1 = false;
                
                freq1Slider.setVisible (true);
                tempo1Slider.setVisible (false);
                triplet1Button.setVisible (false);
                dotted1Button.setVisible (false);
                
                if (syncedAll)
                    tempoSync3Button.setToggleState (false, sendNotification);
                if (synced2to1)
                    tempoSync2Button.setToggleState (false, sendNotification);
                
                freq1Slider.setValue (freq1 * tmi1);
            }
        }
        else if (button == &tempoSync2Button)
        {
            if (isOn)
            {
                tempoSync2 = true;
                
                freq2Slider.setVisible (false);
                tempo2Slider.setVisible (true);
                triplet2Button.setVisible (true);
                dotted2Button.setVisible (true);
                
                if (synced2to1)
                {
                    bool triplet = triplet1Button.getToggleState();
                    bool dotted = dotted1Button.getToggleState();
                    triplet2Button.setToggleState (triplet, sendNotification);
                    dotted2Button.setToggleState (dotted, sendNotification);
                    tempoSync1Button.setToggleState (true, sendNotification);
                    
                    if (syncedAll)
                        tempoSync3Button.setToggleState (true, sendNotification);
                }
                
                double bpm = proc.getCurrentBpm();
                tempo2Slider.setValue (Utility::freqToTempoIndex (freq2, bpm) * -1);
            }
            else
            {
                tempoSync2 = false;
                
                freq2Slider.setVisible (true);
                tempo2Slider.setVisible (false);
                triplet2Button.setVisible (false);
                dotted2Button.setVisible (false);
                
                if (syncedAll)
                    tempoSync3Button.setToggleState (false, sendNotification);
                if (synced2to1)
                    tempoSync1Button.setToggleState (false, sendNotification);
                
                freq2Slider.setValue (freq2 * tmi2);
            }
        }
        else if (button == &tempoSync3Button)
        {
            if (isOn)
            {
                tempoSync3 = true;
                
                freq3Slider.setVisible (false);
                tempo3Slider.setVisible (true);
                triplet3Button.setVisible (true);
                dotted3Button.setVisible (true);
                
                if (syncedAll)
                    tempoSync1Button.setToggleState (true, sendNotification);
                
                double bpm = proc.getCurrentBpm();
                tempo3Slider.setValue (Utility::freqToTempoIndex (freq3, bpm) * -1);
            }
            else
            {
                tempoSync3 = false;
                
                freq3Slider.setVisible (true);
                tempo3Slider.setVisible (false);
                triplet3Button.setVisible (false);
                dotted3Button.setVisible (false);
                
                if (syncedAll)
                    tempoSync3Button.setToggleState (false, sendNotification);
                
                freq3Slider.setValue (freq3 * tmi3);
            }
        }
        else if (button == &triplet1Button)
        {
            if (isOn)
            {
                dotted1Button.setToggleState (false, dontSendNotification);
                tmi1 = 3.0 / 2.0;
            }
            else
            {
                tmi1 = 1;
            }
            proc.setFreq1 (freq1 * tmi1);
            
            if (synced2to1)
            {
                triplet2Button.setToggleState (isOn, sendNotification);
                if (syncedAll)
                    triplet3Button.setToggleState (isOn, sendNotification);
            }
        }
        else if (button == &triplet2Button)
        {
            if (isOn)
            {
                dotted2Button.setToggleState (false, dontSendNotification);
                tmi2 = 3.0 / 2.0;
            }
            else
            {
                tmi2 = 1;
            }
            proc.setFreq2 (freq2 * tmi2);
            
            if (synced2to1)
            {
                triplet1Button.setToggleState (isOn, sendNotification);
                if (syncedAll)
                    triplet3Button.setToggleState (isOn, sendNotification);
            }
        }
        else if (button == &triplet3Button)
        {
            if (isOn)
            {
                dotted3Button.setToggleState (false, dontSendNotification);
                tmi3 = 3.0 / 2.0;
            }
            else
            {
                tmi3 = 1;
            }
            proc.setFreq3 (freq3 * tmi3);
            
            if (syncedAll)
                triplet1Button.setToggleState (isOn, sendNotification);
        }
        else if (button == &dotted1Button)
        {
            if (isOn)
            {
                triplet1Button.setToggleState (false, dontSendNotification);
                tmi1 = 0.75;
            }
            else
            {
                tmi1 = 1;
            }
            proc.setFreq1 (freq1 * tmi1);
            
            if (synced2to1)
            {
                dotted2Button.setToggleState (isOn, sendNotification);
                if (syncedAll)
                    dotted3Button.setToggleState (isOn, sendNotification);
            }
        }
        else if (button == &dotted2Button)
        {
            if (isOn)
            {
                triplet2Button.setToggleState (false, dontSendNotification);
                tmi2 = 0.75;
            }
            else
            {
                tmi2 = 1;
            }
            proc.setFreq2 (freq2 * tmi2);
            
            if (synced2to1)
            {
                dotted1Button.setToggleState (isOn, sendNotification);
                if (syncedAll)
                    dotted3Button.setToggleState (isOn, sendNotification);
            }
        }
        else if (button == &dotted3Button)
        {
            if (isOn)
            {
                triplet3Button.setToggleState (false, dontSendNotification);
                tmi3 = 0.75;
            }
            else
            {
                tmi3 = 1;
            }
            proc.setFreq3 (freq3 * tmi3);
            
            if (syncedAll)
                dotted1Button.setToggleState (isOn, sendNotification);
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
//================================================================================================
    void setAllButtonsLookAndFeel (LookAndFeel* newLookAndFeel)
    {
        lfo1onButton.setLookAndFeel (newLookAndFeel);
        lfo2onButton.setLookAndFeel (newLookAndFeel);
        lfo3onButton.setLookAndFeel (newLookAndFeel);
        
        tempoSync1Button.setLookAndFeel (newLookAndFeel);
        tempoSync2Button.setLookAndFeel (newLookAndFeel);
        tempoSync3Button.setLookAndFeel (newLookAndFeel);
        
        phase0.setLookAndFeel (newLookAndFeel);
        phase90.setLookAndFeel (newLookAndFeel);
        phase180.setLookAndFeel (newLookAndFeel);
        
        triplet1Button.setLookAndFeel (newLookAndFeel);
        triplet2Button.setLookAndFeel (newLookAndFeel);
        triplet3Button.setLookAndFeel (newLookAndFeel);
        
        dotted1Button.setLookAndFeel (newLookAndFeel);
        dotted2Button.setLookAndFeel (newLookAndFeel);
        dotted3Button.setLookAndFeel (newLookAndFeel);
        
        sync2to1Button.setLookAndFeel (newLookAndFeel);
        syncAllButton.setLookAndFeel (newLookAndFeel);
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
        
        tempo1Slider.setBounds (cell1x, row1y - 10, 100, 50);
        tempo2Slider.setBounds (cell2x, row1y - 10, 100, 50);
        tempo3Slider.setBounds (cell3x, row1y - 10, 100, 50);
        
        triplet1Button.setBounds (cell1x, row1y + 75, 40, 25);
        dotted1Button.setBounds (cell1x + 60, row1y + 75, 40, 25);
        
        triplet2Button.setBounds (cell2x, row1y + 75, 40, 25);
        dotted2Button.setBounds (cell2x + 60, row1y + 75, 40, 25);
        
        triplet3Button.setBounds (cell3x, row1y + 75, 40, 25);
        dotted3Button.setBounds (cell3x + 60, row1y + 75, 40, 25);
        
        //second row
        mod1ShapeMenu.setBounds (cell1x, row2y, 100, 25);
        mod2ShapeMenu.setBounds (cell2x, row2y, 100, 25);
        mod3ShapeMenu.setBounds (cell3x, row2y, 100, 25);
        
        //third row
        tempoSync1Button.setBounds (cell1x, row3y, 100, 30);
        tempoSync2Button.setBounds (cell2x, row3y, 100, 30);
        tempoSync3Button.setBounds (cell3x, row3y, 100, 30);
        
        //foutrh row
        sync2to1Button.setBounds (cell1x + 15, row4y, 70, 25);
        phase0.setBounds (cell2x - 15, row4y, 35, 25);
        phase90.setBounds (cell2x + 33, row4y, 35, 25);
        phase180.setBounds (cell2x + 81, row4y, 35, 25);
        syncAllButton.setBounds (cell3x + 15, row4y, 70, 25);
        
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
    
    TextButton triplet1Button, triplet2Button, triplet3Button;
    TextButton dotted1Button, dotted2Button, dotted3Button;
    
    ComboBox mod1ShapeMenu, mod2ShapeMenu, mod3ShapeMenu;
    TextButton tempoSync1Button, tempoSync2Button, tempoSync3Button;
    
    bool tempoSync1 = false, tempoSync2 = false, tempoSync3 = false;
    
    bool synced2to1 = false, syncedAll = false;
    TextButton sync2to1Button, syncAllButton;
    TextButton lfo1onButton, lfo2onButton, lfo3onButton;
    TextButton phase0, phase90, phase180;
    
    //for tempo sync (tempo multiplication index used for counting tripplets and dotted)
    double tmi1 = 1, tmi2 = 1, tmi3 = 1;
    double freq1, freq2, freq3;
    
    MyLookAndFeel myLookAndFeel;
    //====================================================================================
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment>   lfo1FreqSliderAttach,
                                                                    lfo2FreqSliderAttach,
                                                                    lfo3FreqSliderAttach,
                                                                    lfo1TFreqSliderAttach,
                                                                    lfo2TFreqSliderAttach,
                                                                    lfo3TFreqSliderAttach;
    
    ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment>   lfo1onButtonAttach,
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
                                                                    sync2to1Attach,
                                                                    syncAllAttach,
                                                                    phase0Attach,
                                                                    phase90Attach,
                                                                    phase180Attach;
    
    ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment>  lfo1TypeAttach,
                                                                     lfo2TypeAttach,
                                                                     lfo3TypeAttach;
    //==============================================================
    Proc& proc;
    Vibrato2AudioProcessor& p;
    EffectEditor& effectEditor;
    //==============================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModEditor)
};
