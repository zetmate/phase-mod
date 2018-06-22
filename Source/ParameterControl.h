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
        
        else if (parameterId == p.masterId)
            setMaster (p, parameterValue);
        
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
        
        else if (parameterId == p.sync2to1Id)
            setSync2to1 (p, parameterValue);
        
        else if (parameterId == p.syncAllId)
            setSyncAll (p, parameterValue);
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
            
        if (doubleFeedback == false)
        {
            bool sepProc = roundToInt (*p.parameters.getRawParameterValue (p.procTypeId));
            
            if (sepProc)
                p.proc.setFeedbackGain (parameterValue / 100.0);
            else
                p.proc.setFeedbackGain (0.7 * parameterValue / 100.0);
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
    
    static void setMaster (Vibrato2AudioProcessor& p, const float parameterValue)
    {
        p.proc.setInputGain (Utility::fromDb (parameterValue));
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
            
            if (getBoolFromParameter (p, p.fbTypeId))
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
            p.proc.setFeedbackGain (0.7 * fb / 100.0);
            
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
        if (sepProc)
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
    }
    
    static void setFreq1 (Vibrato2AudioProcessor& p, float parameterValue)
    {
        p.proc.setFreq1 (parameterValue);
            
        bool sync2to1 = roundToInt (*p.parameters.getRawParameterValue (p.sync2to1Id));
        if (sync2to1)
        {
            p.proc.setFreq2 (parameterValue);
            
            bool syncAll = roundToInt (*p.parameters.getRawParameterValue (p.syncAllId));
            if (syncAll)
                p.proc.setFreq3 (parameterValue);
        }
    }
    
    static void setFreq2 (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool sync2to1 = roundToInt (*p.parameters.getRawParameterValue (p.sync2to1Id));
        if (!sync2to1)
            p.proc.setFreq2 (parameterValue);
    }
    
    static void setFreq3 (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool syncAll = roundToInt (*p.parameters.getRawParameterValue (p.syncAllId));
        if (!syncAll)
            p.proc.setFreq3 (parameterValue);
    }
    
    static float setTFreq1 (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool tempoSync = getBoolFromParameter (p, p.tempoSync1Id);
        if (tempoSync)
        {
            double bpm = p.proc.getCurrentBpm();
            float freq = Utility::tempoToHz (roundToInt (parameterValue * -1), bpm);
            float tmi = 1.0f;
            
            bool triplet = roundToInt (*p.parameters.getRawParameterValue (p.triplet1Id));
            if (triplet)
            {
                tmi = 3.0 / 2.0;
            }
            else
            {
                bool dotted = roundToInt (*p.parameters.getRawParameterValue (p.dotted1Id));
                if (dotted)
                    tmi = 0.75;
            }
            
            p.proc.setFreq1 (freq * tmi);
            
            bool synced2to1 = getBoolFromParameter (p, p.sync2to1Id);
            if (synced2to1)
            {
                p.proc.setFreq2 (freq * tmi);
                
                bool syncedAll = getBoolFromParameter (p, p.syncAllId);
                if (syncedAll)
                    p.proc.setFreq3 (freq * tmi);
            }
            
            return freq * tmi;
        }
        return p.lfo1FreqDefault;
    }
    
    static float setTFreq2 (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool tempoSync = getBoolFromParameter (p, p.tempoSync2Id);
        if (tempoSync)
        {
            double bpm = p.proc.getCurrentBpm();
            float freq = Utility::tempoToHz (roundToInt (parameterValue * -1), bpm);
            float tmi = 1.0f;
            
            bool triplet = roundToInt (*p.parameters.getRawParameterValue (p.triplet2Id));
            if (triplet)
            {
                tmi = 3.0 / 2.0;
            }
            else
            {
                bool dotted = roundToInt (*p.parameters.getRawParameterValue (p.dotted2Id));
                if (dotted)
                    tmi = 0.75;
            }
            
            p.proc.setFreq2 (freq * tmi);
            
            return freq * tmi;
        }
        return p.lfo2FreqDefault;
    }
    
    static float setTFreq3 (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool tempoSync = getBoolFromParameter (p, p.tempoSync3Id);
        if (tempoSync)
        {
            double bpm = p.proc.getCurrentBpm();
            float freq = Utility::tempoToHz (roundToInt (parameterValue * -1), bpm);
            float tmi = 1.0f;
            
            bool triplet = roundToInt (*p.parameters.getRawParameterValue (p.triplet3Id));
            if (triplet)
            {
                tmi = 3.0 / 2.0;
            }
            else
            {
                bool dotted = roundToInt (*p.parameters.getRawParameterValue (p.dotted3Id));
                if (dotted)
                    tmi = 0.75;
            }
            
            p.proc.setFreq3 (freq * tmi);
            
            return freq * tmi;
        }
        return p.lfo3FreqDefault;
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
            p.proc.setLfo3on (true);
        else
            p.proc.setLfo3on (false);
    }
    
    static void setSync2to1 (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool isOn = roundToInt (parameterValue);
        if (isOn)
        {
            float freq = p.proc.getFreq1();
            p.proc.setFreq2 (freq);
        }
        else
        {
            //set freq2
            bool tempoSync = getBoolFromParameter (p, p.tempoSync2Id);
            if (tempoSync)
            {
                float tfreq = *p.parameters.getRawParameterValue (p.lfo2TFreqId);
                setTFreq2 (p, tfreq);
            }
            else
            {
                float freq = *p.parameters.getRawParameterValue (p.lfo2FreqId);
                setFreq2 (p, freq);
            }
        }
    }
    
    static void setSyncAll (Vibrato2AudioProcessor& p, float parameterValue)
    {
        bool isOn = roundToInt (parameterValue);
        if (isOn)
        {
            float freq = p.proc.getFreq1();
            p.proc.setFreq2 (freq);
            p.proc.setFreq3 (freq);
        }
        else
        {
            //set freq2
            bool tempoSync2 = getBoolFromParameter (p, p.tempoSync2Id);
            if (tempoSync2)
            {
                float tfreq = *p.parameters.getRawParameterValue (p.lfo2TFreqId);
                setTFreq2 (p, tfreq);
            }
            else
            {
                float freq = *p.parameters.getRawParameterValue (p.lfo2FreqId);
                setFreq2 (p, freq);
            }
            
            //set freq3
            bool tempoSync3 = getBoolFromParameter (p, p.tempoSync3Id);
            if (tempoSync3)
            {
                float tfreq = *p.parameters.getRawParameterValue (p.lfo3TFreqId);
                setTFreq3 (p, tfreq);
            }
            else
            {
                float freq = *p.parameters.getRawParameterValue (p.lfo3FreqId);
                setFreq3 (p, freq);
            }
        }
    }
    
private:
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
