/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include "Proc.h"


//==============================================================================
/**
*/
class Vibrato2AudioProcessor  : public AudioProcessor,
                                public AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    Vibrato2AudioProcessor();
    ~Vibrato2AudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    //==============================================================================
    void parameterChanged (const String& parameterID, float newValue) override;
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    Proc proc;
    friend class EffectEditor;
    friend class ModEditor;
    friend struct ParameterControl;
private:
    //some gui related stuff
    AudioProcessorValueTreeState parameters;
    UndoManager undoManager;
    
    //Ranges
    NormalisableRange<float>    depthRange,
                                delayRange,
                                feedbackRange,
                                fbLfoAmpRange,
                                procTypeRange,
                                fbTypeRange;
    
    NormalisableRange<float>    mixRange,
                                voice1MixRange,
                                voice2MixRange,
                                masterRange;
    
    NormalisableRange<float>    lfo1onRange,
                                lfo2onRange,
                                lfo3onRange,
                                lfo1FreqRange,
                                lfo2FreqRange,
                                lfo3FreqRange,
                                lfo1TFreqRange,
                                lfo2TFreqRange,
                                lfo3TFreqRange,
                                lfo1TypeRange,
                                lfo2TypeRange,
                                lfo3TypeRange,
                                note1TypeRange,
                                note2TypeRange,
                                note3TypeRange,
                                tempoSync1Range,
                                tempoSync2Range,
                                tempoSync3Range;
    
    //MAIN PARAMETERS==========================================================
    String depthId = "depth", depthName = "Depth", depthLabelText = depthName;
    float depthDefault = 30;
    
    String delayId = "delay", delayName = "Delay", delayLabelText = delayName;
    float delayDefault = 60;
    
    String feedbackId = "feedback", feedbackName = "Feedback", feedbackLabelText = delayName;
    float feedbackDefault = -70;
    
    String fbLfoAmpId = "fbLfoAmp", fbLfoAmpName = "Feedback LFO Depth", fbLfoAmpLabelText = fbLfoAmpName;
    float fbLfoAmpDefault = 80;
    
    String procTypeId = "procType", procTypeName = "Processing Type", procTypeLabelText = procTypeName;
    float procTypeDefault = 1;
    
    String fbTypeId = "fbTypeId", fbTypeName = "Feedback Type", fbTypeLabelText = fbTypeName;
    float fbTypeDefault = 0;
    
    String mixId = "mix", mixName = "Mix", mixLabelText = mixName.toUpperCase();
    float mixDefault = 50;
    
    String voice1MixId = "voice1Mix", voice1MixName = "Voice 1 Mix", voice1MixLabelText = voice1MixName.toUpperCase();
    float voice1MixDefault = 100;
    
    String voice2MixId = "voice2Mix", voice2MixName = "Voice 2 Mix", voice2MixLabelText = voice2MixName.toUpperCase();
    float voice2MixDefault = 100;
    
    String masterId = "master", masterName = "Master", masterLabelText = masterName.toUpperCase();
    float masterDefault = 0;
    
    // MODULATION PARAMETERS
    //on / off buttons
    String lfo1onId = "lfo1on", lfo1onName = "LFO1 on/off", lfo1onLabelText = lfo1onName.toUpperCase();
    float lfo1onDefault = 1;
    
    String lfo2onId = "lfo2on", lfo2onName = "LFO2 on/off", lfo2onLabelText = lfo2onName.toUpperCase();
    float lfo2onDefault = 1;
    
    String lfo3onId = "lfo3on", lfo3onName = "LFO3 on/off", lfo3onLabelText = lfo3onName.toUpperCase();
    float lfo3onDefault = 0;
    
    //frequency
    // not synced
    String lfo1FreqId = "lfo1Freq", lfo1FreqName = "LFO1 frequency", lfo1FreqLabelText = lfo1FreqName.toUpperCase();
    float lfo1FreqDefault = 0.01;
    
    String lfo2FreqId = "lfo2Freq", lfo2FreqName = "LFO2 frequency", lfo2FreqLabelText = lfo2FreqName.toUpperCase();
    float lfo2FreqDefault = 0.01;
    
    String lfo3FreqId = "lfo3Freq", lfo3FreqName = "LFO3 frequency", lfo3FreqLabelText = lfo3FreqName.toUpperCase();
    float lfo3FreqDefault = 2;
    
    //tempo synced
    String lfo1TFreqId = "lfo1TFreq", lfo1TFreqName = "LFO1 tempo freq", lfo1TFreqLabelText = lfo1TFreqName.toUpperCase();
    float lfo1TFreqDefault = -9;
    
    String lfo2TFreqId = "lfo2TFreq", lfo2TFreqName = "LFO2 tempo freq", lfo2TFreqLabelText = lfo2TFreqName.toUpperCase();
    float lfo2TFreqDefault = -9;
    
    String lfo3TFreqId = "lfo3TFreq", lfo3TFreqName = "LFO3 tempo freq", lfo3TFreqLabelText = lfo3TFreqName.toUpperCase();
    float lfo3TFreqDefault = -4;
    
    //LFO SHAPE COMBO BOXES
    String lfo1TypeId = "lfo1Type", lfo1TypeName = "LFO1 Shape", lfo1TypeLabelText = lfo1TypeName;
    float lfo1TypeDefault = 0;
    
    String lfo2TypeId = "lfo2Type", lfo2TypeName = "LFO2 Shape", lfo2TypeLabelText = lfo2TypeName;
    float lfo2TypeDefault = 0;
    
    String lfo3TypeId = "lfo3Type", lfo3TypeName = "LFO3 Shape", lfo3TypeLabelText = lfo3TypeName;
    float lfo3TypeDefault = 0;
    
    //LFO NOTE TYPE COMBO BOXES
    String note1TypeId = "note1Type", note1TypeName = "LFO1 Note type", note1TypeLabelText = note1TypeName;
    float note1TypeDefault = 0;
    
    String note2TypeId = "note2Type", note2TypeName = "LFO2 Note type", note2TypeLabelText = note2TypeName;
    float note2TypeDefault = 0;
    
    String note3TypeId = "note3Type", note3TypeName = "LFO3 Note type", note3TypeLabelText = note3TypeName;
    float note3TypeDefault = 0;
    
    //TEMPO SYNC BUTTONS
    String tempoSync1Id = "tempoSync1", tempoSync1Name = "LFO1 Tempo Sync", tempoSync1LabelText = tempoSync1Name;
    float tempoSync1Default = 0;
    
    String tempoSync2Id = "tempoSync2", tempoSync2Name = "LFO2 Tempo Sync", tempoSync2LabelText = tempoSync2Name;
    float tempoSync2Default = 0;
    
    String tempoSync3Id = "tempoSync3", tempoSync3Name = "LFO3 Tempo Sync", tempoSync3LabelText = tempoSync3Name;
    float tempoSync3Default = 0;
    
    
    //TEXT TO VALUE AND VALUE TO TEXT FUNCS
    
    String lfoTFreqToText (float value)
    {
        int index = roundToInt (value) * -1;
        switch (index)
        {
            case Tempo::eight1:
                return "8 / 1";
                break;
                
            case Tempo::four1:
                return ("4 / 1");
                break;
                
            case Tempo::two1:
                return ("2 / 1");
                break;
                
            case Tempo::one1:
                return ("1 / 1");
                break;
                
            case Tempo::one2:
                return ("1 / 2");
                break;
                
            case Tempo::one4:
                return ("1 / 4");
                break;
                
            case Tempo::one8:
                return ("1 / 8");
                break;
                
            case Tempo::one16:
                return ("1 / 16");
                break;
                
            case Tempo::one32:
                return ("1 / 32");
                break;
                
            case Tempo::one64:
                return ("1 / 64");
                break;
                
            default:
                return "";
                break;
        }
    }
    
    float textToLfoTFreq (String string)
    {
        String s = string.getLastCharacters (3);
        int denum = s.getIntValue();
        if (denum == 0)
        {
            s = string.getLastCharacters (2);
            denum = s.getIntValue();
            if (denum == 0)
            {
                s = string.getLastCharacters (1);
                denum = s.getIntValue();
            }
        }
        denum = Utility::magnitude (denum);
        
        if (denum == 0)
            return -4;
        
        if (denum != 1)
        {
            if (denum < 3)
                return -6;
            else if (denum < 6)
                return -5;
            else if (denum < 12)
                return -4;
            else if (denum < 24)
                return -3;
            else if (denum < 48)
                return -2;
            else
                return -1;
        }
        else
        {
            int num = Utility::magnitude (string.getIntValue());
            
            if (num > 6)
                num = 8;
            else if (num >= 3)
                num = 4;
            else if (num <= 1)
                num = 1;
            else
                num = 2;
            
            switch (num)
            {
                case 1:
                    return -7;
                    break;
                    
                case 2:
                    return -8;
                    break;
                    
                case 4:
                    return -9;
                    break;
                    
                case 8:
                    return -10;
                    break;
                    
                default:
                    break;
            }
        }
    }
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Vibrato2AudioProcessor)
};
