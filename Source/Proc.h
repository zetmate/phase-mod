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
    
    Proc()  : sampleRate (1), resourcesReleased (false),
                dryWetPropotion(1),
                channelSet (mono),
                processorType (separate)
    {
        //initialise smart pointers with objects
        
        //set initial values
        voiceClose.setMaxDelayTime (25.0f);
        voiceMid.setMaxDelayTime (39.0f);
        voiceFar.setMaxDelayTime (50.0f);
        voiceEcho.setMaxDelayTime (60.0f);
        
        //set wt parameters
        
        //function pointers' default values
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
        //set time & update interval
        dryWetRamp.setTime (7, sampleRate);
        dryWetRamp.updateInterval (dryWetPropotion);
        
        //prepare compressors & limiters
        limiterLeft.prepare (sampleRate);
        limiterRight.prepare (sampleRate);
        
        //prepare filters
        //count coefficients
        
        //prepare ranges: set start, end, interval, skew
        
        //prepare wavetables
        //set sample rate, range, mono/stereo & count wtt
        
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
        
        //set wavetavle to mono
        
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
        
        //set wavetavle to stereo
        
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
        voiceClose.setMaxDelayTime (newMaxDelayMs * 0.25);
        voiceMid.setMaxDelayTime (newMaxDelayMs * 0.5);
        voiceFar.setMaxDelayTime (newMaxDelayMs * 0.75);
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
        voiceClose.wavetable->setFrequency (frequency);
        voiceMid.wavetable->setFrequency (frequency);
    }
    
    void setFreq2 (float frequency)
    {
        voiceFar.wavetable->setFrequency (frequency);
        voiceEcho.wavetable->setFrequency (frequency);
    }
    
    //PROCESSING FUNCTION
    void processBlock (AudioSampleBuffer& buffer, AudioPlayHead* playHead)
    {
        //prepare transport state info
        bool transportIsAvailable = playHead->getCurrentPosition(currentPositionInfo);
        
        //process only if the transport state = is playing or not available
        if (currentPositionInfo.isPlaying || !(transportIsAvailable))
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

    double dryWetPropotion;
    Ramp dryWetRamp;

    //processor info
    ChannelSet channelSet;
    ProcessorType processorType;

    //limiter
    Compressor limiterLeft;
    Compressor limiterRight;
    Compressor limiter1Left;
    Compressor limiter1Right;

    //DAW transport state object
    AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    
};
