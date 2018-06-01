/*
  ==============================================================================

    Proc.h
    Created: 18 Jan 2018 1:19:46pm
    Author:  Macbook

  ==============================================================================
*/

#pragma once

#include "Wavetable.h"
#include "Ramp.h"
#include "Filter.h"
#include "Limiter.h"

class Flanger
{
public:
    
    enum ChannelSet
    {
        mono,
        stereo
    };
    
    enum WtType
    {
        simpleWt,
        complexWt
    };
    
    Flanger()  : sampleRate (1), channelSet (mono), wtType (complexWt), pingPong (false), resourcesReleased(false), minDelayInMs(0), maxDelayInMs (39), delayBuffer (0, 0), circularBufferSize (0), delayCounter(0), delayInSamples(0), prevDelayedLeft(0), prevDelayedRight(0), feedbackGain(0), prevSampleGain(0), dryGain(0), wetGain(0), dryWetPropotion(1), lfoCounter(0)
    {
        //initialise smart pointers with objects
        wavetable = new Oscilator (0.01);
        
        //set initial values
        setDryWetMix (1);
        delayRange.start = 6;
        hpFilter.frequency = 70;
        hpFilter.quality = 0.7;
        
        //set wt parameters
        wavetable->setAllParameters (0.1, 1, 0);
        
        //function pointers' default values
        
        //set default mono behaviour
        setToMono();
    }
    
    virtual ~Flanger()
    {
    }
    
    //Prepare for play function
    void prepare (double newSampleRate, int bufferSize)
    {
        //update sample rate
        sampleRate = newSampleRate;
        
        //prepare ramps
        //set time & update interval
        dryWetRamp.setTime (7, sampleRate);
        dryWetRamp.updateInterval (dryWetPropotion);
        
        //prepare filters
        //count coefficients
        hpFilter.countCoefficients (sampleRate);
        
        //prepare ranges: set start, end, interval, skew
        setMinDelayTime (minDelayInMs);
        delayRange.end = Utility::msToSamples ((double) maxDelayInMs, sampleRate);
        
        //prepare wavetables
        //set sample rate, range, mono/stereo & count wtt
        wavetable->setSampleRate (sampleRate);
        wavetable->countWavetable();
        
        //prepare copy buffers
        //set size according to buffer size & clear the buffers
        circularBufferSize = Utility::msToSamples (500, sampleRate);
        delayBuffer.setSize (2, circularBufferSize);
        delayBuffer.clear();
        delayCounter = 0;
        
    }
    
    void releaseResources()
    {
        if (!resourcesReleased)
        {
            //clear copy buffers
            delayBuffer.clear();
            
            //clear filters' buffers
            hpFilter.clearBuffers();
            
            //reset wavetables
            wavetable->resetWavetable();
            
            //reset counters
            delayCounter = 0;
            
            //set resourcesReleased to true
            resourcesReleased = true;
        }
    }
    
    void processSampleMono (const float input,
                            float& output,
                            const float* delayR,
                            float* delayW);
    
    void processSampleStereo (const float inputLeft, const float inputRight,
                              float& outputLeft, float& outputRight,
                              const float* leftDelayR,
                              const float* rightDelayR,
                              float* leftDelayW,
                              float* rightDelayW);
    
    //functions for changing number of the proceesor's channels
    void setToMono()
    {
        //set channelSet
        channelSet = mono;
        
        //set wavetavle to mono
        wavetable->setStereoOrMono (false, 0);
    }
    
    void setToStereo()
    {
        //set channelSet
        channelSet = stereo;
        
        //set wavetavle to mono
        wavetable->setStereoOrMono (false, 0);
    }
    
    //setters
    void setMaxDelayTime (float newMaxDelayMs)
    {
        maxDelayInMs = newMaxDelayMs;
        delayRange.end = Utility::msToSamples (maxDelayInMs, sampleRate);
        
        if (maxDelayInMs >= 15)
            wtType = complexWt;
        else
            wtType = simpleWt;
    }
    
    void setMinDelayTime (float newMinDelayMs)
    {
        minDelayInMs = newMinDelayMs;
        float minDelaySamp = std::max (4, Utility::msToSamples (newMinDelayMs, sampleRate));
        delayRange.start = minDelaySamp;
    }
    
    void setSweepWidth (float sweepWidthMs)
    {
        sweepWidthMs = std::min (sweepWidthMs, maxDelayInMs);
        setMinDelayTime (maxDelayInMs - sweepWidthMs);
    }
    
    void setDepth (float depthFrom0to1)
    {
        setSweepWidth (maxDelayInMs * depthFrom0to1);
    }
    
    void setFeedbackGain (float newFeedbackGain)
    {
        feedbackGain = std::min (0.99f, newFeedbackGain);
    }
    
    void setFeedbackPolarity (bool isPositive)
    {
        if (isPositive && feedbackGain < 0)
        {
            feedbackGain *= -1;
            prevSampleGain *= -1;
        }
        
        else if (!isPositive && feedbackGain > 0)
        {
            feedbackGain *= -1;
            prevSampleGain *= -1;
        }
    }
    
    void setPrevSampleGain (float newPrevSampGain)
    {
        prevSampleGain = newPrevSampGain;
    }
    
    void setDryWetMix (float newDryWetPropotion)
    {
        dryWetRamp.setRange (dryWetPropotion, newDryWetPropotion);
    }
    
    //filters' setters
    void setLowCutFrequency (float newFrequency)
    {
        hpFilter.frequency = newFrequency;
        hpFilter.countCoefficients (sampleRate);
    }
    
    //getters
    //get pointers
    const float* getReadPointerToDelayBuffer (int channel)
    {
        return delayBuffer.getReadPointer (channel);
    }
    
    float* getWritePointerToDelayBuffer (int channel)
    {
        return delayBuffer.getWritePointer (channel);
    }
    
    ScopedPointer<Oscilator> wavetable;
    
private:
    double sampleRate;
    ChannelSet channelSet;
    WtType wtType;
    
    bool pingPong;
    
    bool resourcesReleased;
    
    //filters
    HighPassFilter hpFilter;
    
    NormalisableRange <float> delayRange;
    float minDelayInMs, maxDelayInMs;
    
    AudioSampleBuffer delayBuffer;
    int circularBufferSize;
    int delayCounter;
    double delayInSamples;
    
    float prevDelayedLeft, prevDelayedRight;
    
    //gain values
    double feedbackGain;
    double prevSampleGain;
    double dryGain;
    double wetGain;
    double dryWetPropotion;
    
    
    
    //ramps
    Ramp dryWetRamp;
    
    int lfoCounter;
    
    //DAW transport state object
    AudioPlayHead::CurrentPositionInfo currentPositionInfo;
};


