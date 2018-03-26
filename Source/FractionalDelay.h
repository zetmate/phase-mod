/*
  ==============================================================================

    FractionalDelay.h
    Created: 23 Feb 2018 11:44:47am
    Author:  Macbook

  ==============================================================================
*/

#pragma once
#include "Utility.h"

class FractionalDelay
{
public:
    FractionalDelay()  : buffer (0, 0), counter (0), sampleRate (44100), delayInSamples(0),
                         delayInMs (0)
    {
    }
    
    ~FractionalDelay()
    {
    }
    
//    float writeLeft (float valueToWrite)
//    {
//    }
    
    void cleanBufferAndResetCounter()
    {
        buffer.clear();
        counter = 0;
    }
    
    void updateSampleRateRelatedValues (double newSampleRate, float maxDelayInMs, int numChannels)
    {
        sampleRate = newSampleRate;
        delayInSamples = Utility::msToSamplesF (delayInMs, sampleRate);
        buffer.setSize (numChannels, Utility::msToSamples (maxDelayInMs, sampleRate));
    }
    
    void setDelayInMs (float ms)
    {
        delayInMs = ms;
        delayInSamples = Utility::msToSamplesF (ms, sampleRate);
    }
    
    void setDelayInSamples (float newDelayInSamples)
    {
        delayInMs = Utility::samplesToMs (newDelayInSamples, sampleRate);
        delayInSamples = newDelayInSamples;
    }
    
private:
    AudioSampleBuffer buffer;
    int counter;
    double sampleRate;
    float delayInSamples, delayInMs;
    float* leftW, rightW;
    //const float* leftR, rightR;
    
};