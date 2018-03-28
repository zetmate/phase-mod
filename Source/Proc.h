/*
  ==============================================================================

    Proc.h
    Created: 18 Jan 2018 1:19:46pm
    Author:  Macbook

  ==============================================================================
*/

#pragma once

#include "Utility.h"
#include "Wavetable.h"
#include "Ramp.h"
#include "Filter.h"

class Proc
{
public:
    Proc()  : xFactor(4), sampleRate (44100), minDelayInMs (0), maxDelayInMs (37), delayBuffer (0, 0),
              delayCounter(0), delayInSamples(0), prevProcessedLeft(0), prevProcessedRight(0)
    {
        //set initial values
        lpFilter.frequency = 18000;
        
        //set wt parameters
        wavetable.setAllParameters (3.0f / (float) xFactor, 1, 0);
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
        lpFilter.frequency = sampleRate / 2.4;
        lpFilter.countCoefficients (sampleRate * xFactor);
        lpFilter1.frequency = sampleRate / 2.4;
        lpFilter1.countCoefficients (sampleRate);
        
        //prepare ranges: set start, end, interval, skew
        delayRange.start = 4;
        delayRange.end = Utility::msToSamples ((double) maxDelayInMs, sampleRate * xFactor);
        
        //prepare wavetables
        //set sample rate, range & count wtt
        wavetable.setSampleRate (sampleRate);
        wavetable.setRange (delayRange);
        wavetable.countWavetable();
        
        //prepare copy buffers
        //set size according to buffer size & clear the buffers
        delayBuffer.setSize (2, (int)delayRange.end * 2);
        delayBuffer.clear();
        delayCounter = 0;
    }
    
    Oscilator wavetable;
    int xFactor;
    
protected:
    double sampleRate;
    
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
        bool transportIsAvailable = playHead->getCurrentPosition(currentPositionInfo);
        
        //process only if the transport state = is playing or not available
        if (currentPositionInfo.isPlaying || !(transportIsAvailable))
        //if (true)
        {
            //constants
            const int numSamples = buffer.getNumSamples();
            const int circularBufferSize = delayBuffer.getNumSamples();
            const int xBufferSize = numSamples * xFactor;
            
            //apply oversampling
            AudioSampleBuffer xBuffer;
            Utility::oversample (buffer, xBuffer, xFactor);
            
            //get pointers to buffer
            const float* leftBufferR = xBuffer.getReadPointer(0);
            const float* rightBufferR = xBuffer.getReadPointer(1);
            float* leftBufferW = buffer.getWritePointer (0);
            float* rightBufferW = buffer.getWritePointer (1);
            
            //get pointers to delay buffer
            const float* leftDelayR = delayBuffer.getReadPointer(0);
            const float* rightDelayR = delayBuffer.getReadPointer(1);
            float* leftDelayW = delayBuffer.getWritePointer (0);
            float* rightDelayW = delayBuffer.getWritePointer (1);
            
            for (int sample = 0; sample < xBufferSize; ++sample)
            {
                //get input signal
                const float inputLeft = leftBufferR[sample];
                const float inputRight = rightBufferR[sample];
                const float averegeChannelLevel = (Utility::magnitude (inputLeft) +
                                                   Utility::magnitude (inputRight)) / 2;
                
                //get current delay time
                delayInSamples = wavetable.applyWavetable (averegeChannelLevel);
                int delayInSamplesInt = (int) ceilf (delayInSamples);
                float fraction = delayInSamples - floorf (delayInSamples);
                
                //check counter
                if (delayCounter >= circularBufferSize)
                    delayCounter = 0;
                
                //read delayed signal
                int indexPrev = 0;
                int indexN = 0;
                int indexN1 = 0;
                int indexN2 = 0;
                
                //count indexes
                
                if (delayCounter >= delayInSamplesInt)
                    indexN = delayCounter - delayInSamplesInt;
                else
                    indexN = (circularBufferSize) - (delayInSamplesInt - delayCounter);
                
                if (indexN == 0)
                    indexPrev = circularBufferSize - 1;
                else
                    indexPrev = indexN - 1;
                
                if (indexN == circularBufferSize - 1)
                    indexN1 = 0;
                else
                    indexN1 = indexN + 1;
                
                if (indexN1 == circularBufferSize - 1)
                    indexN2 = 0;
                else
                    indexN2 = indexN1 + 1;
                
                
                //intepolate samples
                
                float interpolatedLeft = Utility::cubicInterpolation (fraction,
                                                                      leftDelayR [indexPrev],
                                                                      leftDelayR [indexN],
                                                                      leftDelayR [indexN1],
                                                                      leftDelayR [indexN2]);
                
                float interpolatedRight = Utility::cubicInterpolation (fraction,
                                                                       rightDelayR [indexPrev],
                                                                       rightDelayR [indexN],
                                                                       rightDelayR [indexN1],
                                                                       rightDelayR [indexN2]);
                //filter interpolated samples
                float processedLeft = lpFilter.filterSignal (interpolatedLeft, 0);
                float processedRight = lpFilter.filterSignal (interpolatedRight, 1);
                
                //output signal
                if (sample % xFactor == 0)
                {
                    leftBufferW [sample / xFactor] = lpFilter1.filterSignal (processedLeft, 0);
                    rightBufferW [sample / xFactor] = lpFilter1.filterSignal (processedRight, 1);
                }
                    
                //store input signal in the delay buffer
                leftDelayW [delayCounter] = inputLeft + 0.9 * processedLeft;
                rightDelayW [delayCounter] = inputRight + 0.9 * processedRight;
                
                //store previous values
                prevProcessedLeft = processedLeft;
                prevProcessedRight = processedRight;
                
                //increase counter
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

