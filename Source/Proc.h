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

class Proc
{
public:
    Proc()  : frequency (0.1), xFactor(1), sampleRate (44100), lfoSample(0), minDelayInMs (0), maxDelayInMs (37), delayBuffer (0, 0),
              delayCounter(0), delayInSamples(0), prevProcessedLeft(0), prevProcessedRight(0)
    {
        //initialise smart pointers with objects
        wavetable = new Oscilator();
        
        //set initial values
        
        //set wt parameters
        wavetable->setAllParameters (0.1f, 1, 0);
        wavetable->setStereoOrMono (true, 180);
    }
    
    virtual ~Proc()
    {
    }
    
    void virtual processBlock (AudioSampleBuffer& buffer, AudioPlayHead* playHead) = 0;
    
    void prepare (double newSampleRate, int bufferSize)
    {
        //update sample rate
        sampleRate = newSampleRate;
        
        //prepare ramps
        //set time
        
        //prepare filters
        //count coefficients
        lpFilter.frequency = floor(sampleRate / 2.4);
        lpFilter.countCoefficients (sampleRate);
        lpFilter1.frequency = lpFilter.frequency;
        lpFilter1.countCoefficients (sampleRate);
        
        //prepare ranges: set start, end, interval, skew
        delayRange.start = 4;
        delayRange.end = Utility::msToSamples ((double) maxDelayInMs, sampleRate * xFactor);
        
        //prepare wavetables
        //set sample rate, range, mono/stereo & count wtt
        wavetable->setSampleRate (sampleRate);
        wavetable->setRange (delayRange);
        wavetable->countWavetable();
        
        //prepare copy buffers
        //set size according to buffer size & clear the buffers
        delayBuffer.setSize (2, (int)delayRange.end * 2);
        delayBuffer.clear();
        delayCounter = 0;
    }
    
    void changeMaxDelayTime (float newMaxDelayMs)
    {
        delayRange.end = Utility::msToSamples (maxDelayInMs, sampleRate);
        wavetable->setRange (delayRange);
        wavetable->valueHasChanged();
    }
    
    ScopedPointer<Oscilator> wavetable;
    float frequency;
    int xFactor;
    
protected:
    double sampleRate;
    
    int lfoSample;
    
    AntiAliasingFilter lpFilter;
    AntiAliasingFilter lpFilter1;
    
    NormalisableRange <float> delayRange;
    float minDelayInMs, maxDelayInMs;
    
    AudioSampleBuffer delayBuffer;
    int delayCounter;
    double delayInSamples;
    
    float prevProcessedLeft, prevProcessedRight;
    
    AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    
private:
    
};

class MonoProc  : public Proc
{
public:
    MonoProc()
    {
    }
    
    ~MonoProc()
    {
    }
    
    void processBlock (AudioSampleBuffer& buffer, AudioPlayHead* playHead) override
    {
        //constants
        const int numSamples = buffer.getNumSamples();
        
        //get pointers to buffer
        const float* bufferR = buffer.getReadPointer(0);
        float* bufferW = buffer.getWritePointer (0);
        
    };
};

class StereoProc  : public Proc
{
public:
    StereoProc()
    {
    }
    
    ~StereoProc()
    {
    }
    
    void processBlock (AudioSampleBuffer& buffer, AudioPlayHead* playHead) override
    {
        //prepare transpor state info
        //bool transportIsAvailable = playHead->getCurrentPosition(currentPositionInfo);
        
        //process only if the transport state = is playing or not available
        //if (currentPositionInfo.isPlaying || !(transportIsAvailable))
        if (true)
        {
            //constants
            const int numSamples = buffer.getNumSamples();
            const int circularBufferSize = delayBuffer.getNumSamples();
            
            //get pointers to buffer
            const float* leftBufferR = buffer.getReadPointer(0);
            const float* rightBufferR = buffer.getReadPointer(1);
            float* leftBufferW = buffer.getWritePointer (0);
            float* rightBufferW = buffer.getWritePointer (1);
            
            //get pointers to delay buffer
            const float* leftDelayR = delayBuffer.getReadPointer(0);
            const float* rightDelayR = delayBuffer.getReadPointer(1);
            float* leftDelayW = delayBuffer.getWritePointer (0);
            float* rightDelayW = delayBuffer.getWritePointer (1);
            
            for (int sample = 0; sample < numSamples; ++sample)
            {
                //get input signal
                const float inputLeft = leftBufferR[sample];
                const float inputRight = rightBufferR[sample];
                
                //check counter
                if (delayCounter >= circularBufferSize)
                    delayCounter = 0;
                
                //get current delay time
                float delayInSamplesLeft = wavetable->applyWavetable (inputLeft, 0);
                float delayInSamplesRight = wavetable->applyWavetable (inputRight, 1);
                
                //interpolate samples
                //========================================================================
                float interpolatedLeft = Utility::fractDelayCubicInt (leftDelayR,
                                                                      delayInSamplesLeft,
                                                                      delayCounter,
                                                                      circularBufferSize);
                
                float interpolatedRight = Utility::fractDelayCubicInt (rightDelayR,
                                                                       delayInSamplesRight,
                                                                       delayCounter,
                                                                       circularBufferSize);
                //=========================================================================
                
                //filter interpolated samples
                float processedLeft = lpFilter.filterSignal (interpolatedLeft, 0);
                float processedRight = lpFilter.filterSignal (interpolatedRight, 1);
                
                //output signal
                leftBufferW [sample] = lpFilter1.filterSignal (interpolatedLeft, 0);
                rightBufferW [sample] = lpFilter1.filterSignal (interpolatedRight, 1);
                    
                //store input signal in the delay buffer
                leftDelayW [delayCounter] = inputLeft + 0.9 * processedLeft;
                rightDelayW [delayCounter] = inputRight + 0.9 * processedRight;
                
                //store previous values
                prevProcessedLeft = processedLeft;
                prevProcessedRight = processedRight;
                
                //increase counters
                delayCounter++;
            }
        }
        else
        {
            delayCounter = 0;
            delayBuffer.clear();
            lpFilter.clearBuffers();
            lpFilter1.clearBuffers();
        }
    }
};

