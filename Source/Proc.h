/*
  ==============================================================================

    Proc.h
    Created: 18 Jan 2018 1:19:46pm
    Author:  Macbook

  ==============================================================================
*/

#pragma once

#include "Flanger.h"

class Proc
{
public:
    
    enum ChannelSet
    {
        mono,
        stereo
    };
    
    enum ProcessorType
    {
        separate,
        cascade
    };
    
    enum PhaseShift
    {
        deg0 = 0,
        deg90 = 90,
        deg180 = 180
    };
    
    Proc()  : sampleRate (1), resourcesReleased (false),
                dryWetPropotion(0.5),
                channelSet (mono),
                processorType (separate)
    {
        //initialise smart pointers with objects
        
        //set initial values
        setMaxDelayTime (60);
        
        voiceMid.setDelayLfoFhase (0.5);
        voiceEcho.setDelayLfoFhase (0.5);
        voiceMid.setFeedbackLfoFhase (0.5);
        voiceEcho.setFeedbackLfoFhase (0.5);
        
        //set default mono behaviour
        setToMono();
    }
    
    virtual ~Proc()
    {
    }
    
    //Prepare for play function
    void prepare (double newSampleRate, int bufferSize)
    {
        //update sample rate
        sampleRate = newSampleRate;
        
        //prepare internal processors
        voiceClose.prepare (sampleRate, bufferSize);
        voiceMid.prepare (sampleRate, bufferSize);
        voiceFar.prepare (sampleRate, bufferSize);
        voiceEcho.prepare (sampleRate, bufferSize);
        
        //prepare ramps
        //set time
        dryWetRamp.setTime (7, sampleRate);
        inputGainRamp.setTime (5, sampleRate);
        outputGainRamp.setTime (5, sampleRate);
        effectGainRamp.setTime (5, sampleRate);
        gain1Ramp.setTime (5, sampleRate);
        gain2Ramp.setTime (5, sampleRate);
        
        //prepare compressors & limiters
        limiterLeft.prepare (sampleRate);
        limiterRight.prepare (sampleRate);
        limiter1Left.prepare (sampleRate);
        limiter1Right.prepare (sampleRate);
        
        //prepare filters
        //count coefficients
        
        //prepare ranges: set start, end, interval, skew
        
        //prepare copy buffers
        //set size according to buffer size & clear the buffers
    }
    
    void releaseResources()
    {
        if (!resourcesReleased)
        {
            //clear copy buffers
            
            //clear filter buffers
            
            //reset wavetables
            
            //reset compressors
            limiterLeft.reset();
            limiterRight.reset();
            limiter1Left.reset();
            limiter1Right.reset();
            
            //reset counters
            
            //release resources of the internal processors
            voiceClose.releaseResources();
            voiceMid.releaseResources();
            voiceFar.releaseResources();
            voiceEcho.releaseResources();
            
            //set resourcesReleased to true
            resourcesReleased = true;
        }
    }
    
    //functions for changing number of the proceesor's channels
    void setToMono()
    {
        //set internal processors to mono behaviour
        voiceClose.setToMono();
        voiceMid.setToMono();
        voiceFar.setToMono();
        voiceEcho.setToMono();
        
        //change channel set
        channelSet = mono;
    }
    
    void setToStereo()
    {
        //set internal processors to stereo behaviour
        voiceClose.setToStereo();
        voiceMid.setToStereo();
        voiceFar.setToStereo();
        voiceEcho.setToStereo();
        
        //set channel set
        channelSet = stereo;
    }
    
    //functions for connection with GUI
    void setSeparateProcessing()
    {
        processorType = separate;
    }
    
    void setCascadeProcessing()
    {
        processorType = cascade;
    }
    
    void setDryWet (float newDryWetPropotion)
    {
        dryWetRamp.setRange (dryWetPropotion, newDryWetPropotion);
    }
    
    void setVoice1Mix (float newDryWetPropotion)
    {
        voiceClose.setDryWetMix (newDryWetPropotion);
        voiceMid.setDryWetMix (newDryWetPropotion);
    }
    void setVoice2Mix (float newDryWetPropotion)
    {
        voiceFar.setDryWetMix (newDryWetPropotion);
        voiceEcho.setDryWetMix (newDryWetPropotion);
    }
    
    void setInputGain (double newInputGain)
    {
        inputGainRamp.setRange (inputGain, newInputGain);
    }
    
    void setOutputGain (double newOutputGain)
    {
        outputGainRamp.setRange (outputGain, newOutputGain);
    }
    
    void setEffectGain (double newEffectGain)
    {
        effectGainRamp.setRange (effectGain, newEffectGain);
    }
    
    void setGain1 (double newGain1)
    {
        gain1Ramp.setRange (gain1, newGain1);
    }
    
    void setGain2 (double newGain2)
    {
        gain2Ramp.setRange (gain2, newGain2);
    }
    
    void setFeedbackGain (float feedbackGain)
    {
        voiceClose.setFeedbackGain (feedbackGain);
        voiceMid.setFeedbackGain (feedbackGain);
        voiceFar.setFeedbackGain (feedbackGain);
        voiceEcho.setFeedbackGain (feedbackGain);
    }
    
    void setDepth (float depth)
    {
        voiceClose.setDepth (depth);
        voiceMid.setDepth (depth);
        voiceFar.setDepth (depth);
        voiceEcho.setDepth (depth);
    }
    
    void setPrevSampleGain (float gain)
    {
        voiceClose.setPrevSampleGain (gain);
        voiceMid.setPrevSampleGain (gain);
        voiceFar.setPrevSampleGain (gain);
        voiceEcho.setPrevSampleGain (gain);
    }
    
    void setMaxDelayTime (float newMaxDelayMs)
    {
        voiceClose.setMaxDelayTime (newMaxDelayMs * 0.5);
        voiceMid.setMaxDelayTime (newMaxDelayMs * 0.625);
        voiceFar.setMaxDelayTime (newMaxDelayMs * 0.875);
        voiceEcho.setMaxDelayTime (newMaxDelayMs);
    }
    
    void setLowCut (float frequency)
    {
        voiceClose.setLowCutFrequency (frequency);
        voiceMid.setLowCutFrequency (frequency);
        voiceFar.setLowCutFrequency (frequency);
        voiceEcho.setLowCutFrequency (frequency);
    }
    
    void setFreq1 (float frequency)
    {
        voiceClose.setFrequencyForDelayLfo (frequency);
        voiceMid.setFrequencyForDelayLfo (frequency);
    }
    void setFreq2 (float frequency)
    {
        voiceFar.setFrequencyForDelayLfo (frequency);
        voiceEcho.setFrequencyForDelayLfo (frequency);
    }
    void setFreq3 (float frequency)
    {
        voiceClose.setFrequencyForFeedbackLfo (frequency);
        voiceMid.setFrequencyForFeedbackLfo (frequency);
        voiceFar.setFrequencyForFeedbackLfo (frequency);
        voiceEcho.setFrequencyForFeedbackLfo (frequency);
    }
    
    void setlfoShape1 (Flanger::LfoShape lfoShape)
    {
        voiceClose.setShapeForDelayLfo (lfoShape);
        voiceMid.setShapeForDelayLfo (lfoShape);
    }
    void setlfoShape2 (Flanger::LfoShape lfoShape)
    {
        voiceFar.setShapeForDelayLfo (lfoShape);
        voiceEcho.setShapeForDelayLfo (lfoShape);
    }
    void setlfoShape3 (Flanger::LfoShape lfoShape)
    {
        voiceClose.setShapeForFeedbackLfo (lfoShape);
        voiceMid.setShapeForFeedbackLfo (lfoShape);
        voiceFar.setShapeForFeedbackLfo (lfoShape);
        voiceEcho.setShapeForFeedbackLfo (lfoShape);
    }
    
    void setLfo3Amp (double newAmp)
    {
        newAmp = std::max (0.02, newAmp);
        voiceClose.setAmplitudeForFeedbackLfo (newAmp);
        voiceMid.setAmplitudeForFeedbackLfo (newAmp);
        voiceFar.setAmplitudeForFeedbackLfo (newAmp);
        voiceEcho.setAmplitudeForFeedbackLfo (newAmp);
    }
    
    void setLfo1on (bool shouldBeOn)
    {
        voiceClose.setDelayLfoOn (shouldBeOn);
        voiceMid.setDelayLfoOn (shouldBeOn);
    }
    void setLfo2on (bool shouldBeOn)
    {
        voiceFar.setDelayLfoOn (shouldBeOn);
        voiceEcho.setDelayLfoOn (shouldBeOn);
    }
    void setLfo3on (bool shouldBeOn)
    {
        voiceClose.setFbLfoOn (shouldBeOn);
        voiceMid.setFbLfoOn (shouldBeOn);
        voiceFar.setFbLfoOn (shouldBeOn);
        voiceEcho.setFbLfoOn (shouldBeOn);
    }
    
    //phaseShift
    void setPhaseShift (PhaseShift newPhaseShift)
    {
        phaseShift = newPhaseShift;
        if (phaseShift == deg180)
        {
            voiceMid.setDelayLfoFhase (0.5);
            voiceEcho.setDelayLfoFhase (0.5);
            voiceMid.setFeedbackLfoFhase (0.5);
            voiceEcho.setFeedbackLfoFhase (0.5);
        }
        else if (phaseShift == deg90)
        {
            voiceMid.setDelayLfoFhase (0.25);
            voiceEcho.setDelayLfoFhase (0.25);
            voiceMid.setFeedbackLfoFhase (0.25);
            voiceEcho.setFeedbackLfoFhase (0.25);
        }
        else
        {
            voiceMid.setDelayLfoFhase (0);
            voiceEcho.setDelayLfoFhase (0);
            voiceMid.setFeedbackLfoFhase (0);
            voiceEcho.setFeedbackLfoFhase (0);
        }
    }
    
    //getters
    double getCurrentBpm() const
    {
        return currentBpm;
    }
    
    float getFreq1() const
    {
        return voiceClose.lfoFreq_delay;
    }
    
    float getFreq2() const
    {
        return voiceFar.lfoFreq_delay;
    }
    
    float getFreq3() const
    {
        return voiceClose.lfoFreq_feedback;
    }
    
    //PROCESSING FUNCTION
    void processBlock (AudioSampleBuffer& buffer, AudioPlayHead* playHead)
    {
        //prepare transport state info
        transportIsAvailable = playHead->getCurrentPosition(currentPositionInfo);
       
        //process only if the transport state = is playing or not available
        if (currentPositionInfo.isPlaying || !(transportIsAvailable))
        //if (true)
        {
            if (channelSet == stereo)
            {
                if (processorType == separate)
                    processBlockStereoSeparate (buffer);
                else
                    processBlockStereoCascade (buffer);
            }
            else
            {
                if (processorType == separate)
                    processBlockMonoSeparate (buffer);
                else
                    processBlockMonoCascade (buffer);
            }
        }
        else
        {
            releaseResources();
        }
    }

    //Mono processing functions
    void processBlockMonoSeparate (AudioSampleBuffer& buffer);
    void processBlockMonoCascade (AudioSampleBuffer& buffer);
    
    //Stereo processing functions
    void processBlockStereoSeparate (AudioSampleBuffer& buffer);
    void processBlockStereoCascade (AudioSampleBuffer& buffer);

    //internal processors
    Flanger voiceClose,
            voiceMid,
            voiceFar,
            voiceEcho;
    
protected:
    double sampleRate;
    bool resourcesReleased;

    //dry wet stuff
    double dryWetPropotion;
    Ramp dryWetRamp;
    
    //gains
    double inputGain = 1.0, outputGain = 1.0;
    double effectGain = 1.0;
    double gain1 = 1.0;
    double gain2 = 1.0f;
    //gain ramps
    Ramp inputGainRamp, outputGainRamp, effectGainRamp, gain1Ramp, gain2Ramp;

    //processor info
    ChannelSet channelSet;
    ProcessorType processorType;
    PhaseShift phaseShift = deg180;

    //limiter
    Compressor limiterLeft;
    Compressor limiterRight;
    Compressor limiter1Left;
    Compressor limiter1Right;

    //DAW transport state object
    AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    double currentBpm = 120;
    bool transportIsAvailable = false;
};
