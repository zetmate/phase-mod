/*
  ==============================================================================

    ParameterControl.h
    Created: 20 Jun 2018 9:21:09pm
    Author:  Macbook

  ==============================================================================
*/

#pragma once
#include "Utility.h"
#include "PluginProcessor.h"

struct ParameterControl
{
public:
    static void updateParameter (Vibrato2AudioProcessor& p, const String parameterId, const float parameterValue)
    {
        if (parameterId == p.depthId)
            setDepth (p, parameterValue);
        
        else if (parameterId == p.delayId)
            setDelay (p, parameterValue);
        
        else if (parameterId == p.feedbackId)
            setFeedback (p, parameterValue);
        
        else if (parameterId == p.fbLfoAmpId)
            setFeedback (p, parameterValue);
        
        else if (parameterId == p.mixId)
            setMix (p, parameterValue);
        
        else if (parameterId == p.inputId)
            setInput (p, parameterValue);
            
        else if (parameterId == p.outputId)
            setOutput (p, parameterValue);
        
        else if (parameterId == p.voice1MixId)
            setVoice1Mix (p, parameterValue);
        
        else if (parameterId == p.voice2MixId)
            setVoice2Mix (p, parameterValue);
        
        else if (parameterId == p.procTypeId)
            setProcType (p, parameterValue);
        
        else if (parameterId == p.fbTypeId)
            setFbType (p, parameterValue);
        
        else if (parameterId == p.lfo1FreqId)
            setFreq1 (p, parameterValue);
        
        else if (parameterId == p.lfo2FreqId)
            setFreq2 (p, parameterValue);
        
        else if (parameterId == p.lfo3FreqId)
            setFreq3 (p, parameterValue);
        
        else if (parameterId == p.lfo1TFreqId)
            setTFreq1 (p, parameterValue);
        
        else if (parameterId == p.lfo2TFreqId)
            setTFreq2 (p, parameterValue);
        
        else if (parameterId == p.lfo3TFreqId)
            setTFreq3 (p, parameterValue);
        
        else if (parameterId == p.lfo1onId)
            setLfo1on (p, parameterValue);
        
        else if (parameterId == p.lfo2onId)
            setLfo2on (p, parameterValue);
        
        else if (parameterId == p.lfo3onId)
            setLfo3on (p, parameterValue);
        
        else if (parameterId == p.tempoSync1Id)
            setTempoSync1 (p, parameterValue);
        
        else if (parameterId == p.tempoSync2Id)
            setTempoSync2 (p, parameterValue);
        
        else if (parameterId == p.tempoSync3Id)
            setTempoSync3 (p, parameterValue);
        
        else if (parameterId == p.note1TypeId)
            setNote1Type (p, parameterValue);
        
        else if (parameterId == p.note2TypeId)
            setNote2Type (p, parameterValue);
        
        else if (parameterId == p.note3TypeId)
            setNote3Type (p, parameterValue);
        
        else if (parameterId == p.lfo1TypeId)
            setLfo1Shape (p, parameterValue);
        
        else if (parameterId == p.lfo2TypeId)
            setLfo2Shape (p, parameterValue);
        
        else if (parameterId == p.lfo3TypeId)
            setLfo3Shape (p, parameterValue);
        
        else if (parameterId == p.phaseId)
            setPhase (p, parameterValue);
    }
    
    static void setDepth (Vibrato2AudioProcessor& p, const float parameterValue)
    {
        p.proc.setDepth (parameterValue / 100.0);
    }
    
    static void setDelay (Vibrato2AudioProcessor& p, const float parameterValue)
    {
        p.proc.setMaxDelayTime (parameterValue);
    }
    
    static void setFeedback (Vibrato2AudioProcessor& p, const float parameterValue)
    {
        bool doubleFeedback = roundToInt (*p.parameters.getRawParameterValue (p.fbTypeId));
        bool cascProc = !getBoolFromParameter (p, p.procTypeId);
        if (cascProc)
            doubleFeedback = false;
        
        bool lfo3off = !getBoolFromParameter (p, p.lfo3onId);
            
        if (doubleFeedback == false && lfo3off)
        {
            bool sepProc = roundToInt (*p.parameters.getRawParameterValue (p.procTypeId));
            
            if (sepProc)
                p.proc.setFeedbackGain (parameterValue / 100.0);
            else
                p.proc.setFeedbackGain (0.65 * parameterValue / 100.0);
        }
    }
    
    static void setLfo3Amp (Vibrato2AudioProcessor& p, const float parameterValue)
    {
        p.proc.setLfo3Amp (parameterValue / 100.0);
    }
    
    static void setMix (Vibrato2AudioProcessor& p, const float parameterValue)
    {
        p.proc.setDryWet (parameterValue / 100.0);
    }
    
    static void setInput (Vibrato2AudioProcessor& p, const float parameterValue)
    {
        p.proc.setInputGain (Utility::fromDb (parameterValue));
    }
    
    static void setOutput (Vibrato2AudioProcessor& p, const float parameterValue)
    {
        p.proc.setOutputGain (Utility::fromDb (parameterValue));
    }
    
    static void setVoice1Mix (Vibrato2AudioProcessor& p, const float parameterValue)
    {
        bool separateProcessing = roundToInt (*p.parameters.getRawParameterValue (p.procTypeId));
            
        if (separateProcessing)
            p.proc.setGain1 (parameterValue / 100.0);
        else
            p.proc.setVoice1Mix (parameterValue / 100.0);
    }
    
    static void setVoice2Mix (Vibrato2AudioProcessor& p, const float parameterValue)
    {
        bool separateProcessing = roundToInt (*p.parameters.getRawParameterValue (p.procTypeId));
            
        if (separateProcessing)
            p.proc.setGain2 (parameterValue / 100.0);
        else
            p.proc.setVoice2Mix (parameterValue / 100.0);
    }
    
    static void setProcType (Vibrato2AudioProcessor& p, const float parameterValue)
    {
        bool sep = roundToInt (parameterValue);
        if (sep)
        {
            //set processor type
            p.proc.setSeparateProcessing();
            
            //set appropriate feedback gain
            float fb = *p.parameters.getRawParameterValue (p.feedbackId);
            float doubleFb = getBoolFromParameter (p, p.fbTypeId);
            float lfo3off = !getBoolFromParameter (p, p.lfo3onId);
            if (doubleFb && lfo3off)
            {
                if (fb > 0)
                {
                    p.proc.setFeedbackGain (0.7);
                    p.proc.setPrevSampleGain (0.25);
                }
                else
                {
                    p.proc.setFeedbackGain (-0.7);
                    p.proc.setPrevSampleGain (-0.25);
                }
            }
            else
            {
                p.proc.setFeedbackGain (fb / 100.0);
            }
            
            //set gain of voices
            float gain1 = *p.parameters.getRawParameterValue (p.voice1MixId);
            float gain2 = *p.parameters.getRawParameterValue (p.voice2MixId);
            p.proc.setGain1 (gain1 / 100.0);
            p.proc.setGain2 (gain2 / 100.0);
            
            //set mix of voices to 100%
            p.proc.setVoice1Mix (1.0);
            p.proc.setVoice2Mix (1.0);
        }
        else
        {
            //set processor type
            p.proc.setCascadeProcessing();
            
            //set appropriate feedback gain
            float fb = *p.parameters.getRawParameterValue (p.feedbackId);
            p.proc.setFeedbackGain (0.65 * fb / 100.0);
            
            //remove double feedback
            p.proc.setPrevSampleGain (0);
            
            //set mix values
            float mix1 = *p.parameters.getRawParameterValue (p.voice1MixId);
            float mix2 = *p.parameters.getRawParameterValue (p.voice2MixId);
            p.proc.setVoice1Mix (mix1 / 100.0);
            p.proc.setVoice2Mix (mix2 / 100.0);
        }
    }
    
    static void setFbType (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool sepProc = getBoolFromParameter (p, p.procTypeId);
        bool lfo3off = !getBoolFromParameter (p, p.lfo3onId);
        if (sepProc && lfo3off)
        {
            bool doubleFb = roundToInt (parameterValue);
            float fb = *p.parameters.getRawParameterValue (p.feedbackId);
            
            if (doubleFb)
            {
                if (fb > 0)
                {
                    p.proc.setFeedbackGain (0.7);
                    p.proc.setPrevSampleGain (0.25);
                }
                else
                {
                    p.proc.setFeedbackGain (-0.7);
                    p.proc.setPrevSampleGain (-0.25);
                }
            }
            else
            {
                p.proc.setPrevSampleGain (0);
                p.proc.setFeedbackGain (fb / 100.0);
            }
        }
        else
        {
            p.proc.setPrevSampleGain (0);
        }
    }
    
    static void setFreq1 (Vibrato2AudioProcessor& p, float parameterValue)
    {
        p.proc.setFreq1 (parameterValue);
    }
    
    static void setFreq2 (Vibrato2AudioProcessor& p, float parameterValue)
    {
        p.proc.setFreq2 (parameterValue);
    }
    
    static void setFreq3 (Vibrato2AudioProcessor& p, float parameterValue)
    {
        p.proc.setFreq3 (parameterValue);
    }
    
    static void setTFreq1 (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool tempoSync = getBoolFromParameter (p, p.tempoSync1Id);
        if (tempoSync)
        {
            double bpm = p.proc.getCurrentBpm();
            float freq = Utility::tempoToHz (roundToInt (parameterValue * -1), bpm);
            
            int noteType = roundToInt (*p.parameters.getRawParameterValue (p.note1TypeId));
            float tmi = 1.0f;
            switch (noteType)
            {
                case 1:
                    tmi = 3.0 / 2.0;
                    break;
                    
                case 2:
                    tmi = 0.75;
                    break;
                    
                default:
                    break;
            }
            
            p.proc.setFreq1 (freq * tmi);
        }
    }
    
    static void setTFreq2 (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool tempoSync = getBoolFromParameter (p, p.tempoSync2Id);
        if (tempoSync)
        {
            double bpm = p.proc.getCurrentBpm();
            float freq = Utility::tempoToHz (roundToInt (parameterValue * -1), bpm);
            
            int noteType = roundToInt (*p.parameters.getRawParameterValue (p.note2TypeId));
            float tmi = 1.0f;
            switch (noteType)
            {
                case 1:
                    tmi = 3.0 / 2.0;
                    break;
                    
                case 2:
                    tmi = 0.75;
                    break;
                    
                default:
                    break;
            }
            
            p.proc.setFreq2 (freq * tmi);
        }
    }
    
    static void setTFreq3 (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool tempoSync = getBoolFromParameter (p, p.tempoSync3Id);
        if (tempoSync)
        {
            double bpm = p.proc.getCurrentBpm();
            float freq = Utility::tempoToHz (roundToInt (parameterValue * -1), bpm);
            
            int noteType = roundToInt (*p.parameters.getRawParameterValue (p.note3TypeId));
            float tmi = 1.0f;
            switch (noteType)
            {
                case 1:
                    tmi = 3.0 / 2.0;
                    break;
                    
                case 2:
                    tmi = 0.75;
                    break;
                    
                default:
                    break;
            }
            
            p.proc.setFreq3 (freq * tmi);
        }
    }
    
    static void setLfo1on (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool isOn = roundToInt (parameterValue);
        if (isOn)
            p.proc.setLfo1on (true);
        else
            p.proc.setLfo1on (false);
    }
    
    static void setLfo2on (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool isOn = roundToInt (parameterValue);
        if (isOn)
            p.proc.setLfo2on (true);
        else
            p.proc.setLfo2on (false);
    }
    
    static void setLfo3on (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool isOn = roundToInt (parameterValue);
        if (isOn)
        {
            p.proc.setLfo3on (true);
            p.proc.setPrevSampleGain (0);
        }
        else
        {
            p.proc.setLfo3on (false);
            bool doublefb = getBoolFromParameter (p, p.fbTypeId);
            bool sepProc = getBoolFromParameter (p, p.procTypeId);
            if (doublefb && sepProc)
            {
                float fb = getParameterValue (p, p.feedbackId);
                if (fb > 0)
                {
                    p.proc.setFeedbackGain (0.7);
                    p.proc.setPrevSampleGain (0.25);
                }
                else
                {
                    p.proc.setFeedbackGain (-0.7);
                    p.proc.setPrevSampleGain (-0.25);
                }
            }
        }
    }
    
    static void setTempoSync1 (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool tempoSync = getBoolFromParameter (p, p.tempoSync1Id);
        if (tempoSync)
        {
            float tempoParamValue = getParameterValue (p, p.lfo1TFreqId);
            setTFreq1 (p, tempoParamValue);
        }
        else
        {
            float freq = getParameterValue (p, p.lfo1FreqId);
            setFreq1 (p, freq);
        }
    }
    
    static void setTempoSync2 (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool tempoSync = getBoolFromParameter (p, p.tempoSync2Id);
        if (tempoSync)
        {
            float tempoParamValue = getParameterValue (p, p.lfo2TFreqId);
            setTFreq2 (p, tempoParamValue);
        }
        else
        {
            float freq = getParameterValue (p, p.lfo2FreqId);
            setFreq2 (p, freq);
        }
    }
    
    static void setTempoSync3 (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool tempoSync = getBoolFromParameter (p, p.tempoSync3Id);
        if (tempoSync)
        {
            float tempoParamValue = getParameterValue (p, p.lfo3TFreqId);
            setTFreq3 (p, tempoParamValue);
        }
        else
        {
            float freq = getParameterValue (p, p.lfo3FreqId);
            setFreq3 (p, freq);
        }
    }
    
    static void setNote1Type (Vibrato2AudioProcessor& p, float parameterValue)
    {
        float tempoFreq = getParameterValue (p, p.lfo1TFreqId);
        setTFreq1 (p, tempoFreq);
    }
    
    static void setNote2Type (Vibrato2AudioProcessor& p, float parameterValue)
    {
        float tempoFreq = getParameterValue (p, p.lfo2TFreqId);
        setTFreq2 (p, tempoFreq);
    }
    
    static void setNote3Type (Vibrato2AudioProcessor& p, float parameterValue)
    {
        float tempoFreq = getParameterValue (p, p.lfo3TFreqId);
        setTFreq3 (p, tempoFreq);
    }
    
    static void setLfo1Shape (Vibrato2AudioProcessor& p, float parameterValue)
    {
        int i = roundToInt (parameterValue) + 1;
        switch (i)
        {
            case 1:
                p.proc.setlfoShape1 (Flanger::LfoShape::sin);
                break;
                
            case 2:
                p.proc.setlfoShape1 (Flanger::LfoShape::triangle);
                break;
                
            case 3:
                p.proc.setlfoShape1 (Flanger::LfoShape::saw);
                break;
                
            case 4:
                p.proc.setlfoShape1 (Flanger::LfoShape::square);
                break;
                
            case 5:
                p.proc.setlfoShape1 (Flanger::LfoShape::random);
                break;
                
            case 6:
                p.proc.setlfoShape1 (Flanger::LfoShape::noise);
                break;
                
            default:
                break;
        }
    }
    
    static void setLfo2Shape (Vibrato2AudioProcessor& p, float parameterValue)
    {
        int i = roundToInt (parameterValue) + 1;
        
        switch (i)
        {
            case 1:
                p.proc.setlfoShape2 (Flanger::LfoShape::sin);
                break;
                
            case 2:
                p.proc.setlfoShape2 (Flanger::LfoShape::triangle);
                break;
                
            case 3:
                p.proc.setlfoShape2 (Flanger::LfoShape::saw);
                break;
                
            case 4:
                p.proc.setlfoShape2 (Flanger::LfoShape::square);
                break;
                
            case 5:
                p.proc.setlfoShape2 (Flanger::LfoShape::random);
                break;
                
            case 6:
                p.proc.setlfoShape2 (Flanger::LfoShape::noise);
                break;
                
            default:
                break;
        }
    }
    
    static void setLfo3Shape (Vibrato2AudioProcessor& p, float parameterValue)
    {
        int i = roundToInt (parameterValue) + 1;
        
        switch (i)
        {
            case 1:
                p.proc.setlfoShape3 (Flanger::LfoShape::sin);
                break;
                
            case 2:
                p.proc.setlfoShape3 (Flanger::LfoShape::triangle);
                break;
                
            case 3:
                p.proc.setlfoShape3 (Flanger::LfoShape::saw);
                break;
                
            case 4:
                p.proc.setlfoShape3 (Flanger::LfoShape::square);
                break;
                
            case 5:
                p.proc.setlfoShape3 (Flanger::LfoShape::random);
                break;
                
            default:
                break;
        }
    }
    
    static void setPhase (Vibrato2AudioProcessor& p, float parameterValue)
    {
        p.proc.setPhaseShift ((Proc::PhaseShift) parameterValue);
    }
    
    //GETTERS
    static bool getBoolFromParameter (Vibrato2AudioProcessor& p, juce::StringRef parameterID)
    {
        bool b = roundToInt (*p.parameters.getRawParameterValue (parameterID));
        return b;
    }
    
    static float getParameterValue (Vibrato2AudioProcessor& p, juce::StringRef parameterID)
    {
        return *p.parameters.getRawParameterValue (parameterID);
    }
};
