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
    Flanger()  : sampleRate (0), resourcesReleased(false), minDelayInMs (0), maxDelayInMs (39), delayBuffer (0, 0), circularBufferSize (0), delayCounter(0), delayInSamples(0), prevDelayedLeft(0), prevDelayedRight(0), feedbackGain(0), prevSampleGain(0), dryGain(0), wetGain(0), dryWetPropotion(1)
    {
        //initialise smart pointers with objects
        wavetable = new Oscilator (0.1);
        
        //set initial values
        setDryWetMix (1);
        
        //set wt parameters
        wavetable->setAllParameters (0.1, 1, 0);
        wavetable->setStereoOrMono (true, 90);
        
        //function pointers' default values
        processBlock = &Flanger::processBlockMono;
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
        dryWetRamp.setTime (5, sampleRate);
        dryWetRamp.updateInterval();
        
        //prepare filters
        //count coefficients
        lpFilter.frequency = floor(sampleRate / 2.4);
        lpFilter.countCoefficients (sampleRate);
        lpFilter1.frequency = lpFilter.frequency;
        lpFilter1.countCoefficients (sampleRate);
        
        //prepare ranges: set start, end, interval, skew
        delayRange.start = 4;
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
            
            //clear filter buffers
            lpFilter.clearBuffers();
            lpFilter1.clearBuffers();
            
            //reset wavetables
            wavetable->resetWavetable();
            
            //reset counters
            delayCounter = 0;
            
            //set resourcesReleased to true
            resourcesReleased = true;
        }
    }
    
    //functions for changing number of the proceesor's channels
    void setToMono()
    {
        //processBlockFunc = &Proc::processBlockMono;
        processBlock = &Flanger::processBlockMono;
    }
    
    void setToStereo()
    {
        //processBlockFunc = &Proc::processBlockStereo;
        processBlock = &Flanger::processBlockStereo;
    }
    
    //setters
    void setMaxDelayTime (float newMaxDelayMs)
    {
        maxDelayInMs = newMaxDelayMs;
        delayRange.end = Utility::msToSamples (maxDelayInMs, sampleRate);
    }
    
    void setMinDelayTime (float newMinDelayMs)
    {
        float minDelaySamp = std::max (4, Utility::msToSamples (newMinDelayMs, sampleRate));
        delayRange.start = minDelaySamp;
    }
    
    void setSweepWidth (float sweepWidthMs)
    {
        sweepWidthMs = std::min (sweepWidthMs, maxDelayInMs);
        setMinDelayTime (maxDelayInMs - sweepWidthMs);
    }
    
    void setFeedbackGain (float newFeedbackGain)
    {
        feedbackGain = newFeedbackGain;
    }
    
    void setPrevSampleGain (float newPrevSampGain)
    {
        prevSampleGain = newPrevSampGain;
    }
    
    void setDryWetMix (float newDryWetPropotion)
    {
        dryWetRamp.setRange (dryWetPropotion, newDryWetPropotion);
    }
    
    //getters
    const float* getReadPointerToDelayBuffer (int channel)
    {
        delayBuffer.getReadPointer (channel);
    }
    
    float* getWritePointerToDelayBuffer (int channel)
    {
        delayBuffer.getWritePointer (channel);
    }
    
    ScopedPointer<Oscilator> wavetable;
    
    //function pointers
    std::function<void (Flanger&, AudioSampleBuffer& buffer, AudioPlayHead* playHead)>
                            processBlock = {};
    
    std::function<AudioSampleBuffer (Flanger&, const float leftInput, const float rightInput,
                                     const float* leftDelayR,
                                     const float* rightDelayR,
                                     float* leftDelayW,
                                     float* rightDelayW)>
                            processSample = {};
    
protected:
    double sampleRate;
    
    bool resourcesReleased;
    
    //filters
    AntiAliasingFilter lpFilter;
    AntiAliasingFilter lpFilter1;
    
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
    
    //DAW transport state object
    AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    
private:
//=====================================================================================
//    PROCESSING FUNCTIONS FOR THE BLOCK
//=====================================================================================
    //Mono processing function
    void processBlockMono (AudioSampleBuffer& buffer, AudioPlayHead* playHead)
    {
        //constants
        const int numSamples = buffer.getNumSamples();
        
        //get pointers to buffer
        const float* bufferR = buffer.getReadPointer(0);
        float* bufferW = buffer.getWritePointer (0);
        
    };
    
    //Stereo processing function
    void processBlockStereo (AudioSampleBuffer& buffer, AudioPlayHead* playHead)
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
                //========================================================================
                float delayInSamplesLeft;
                float delayInSamplesRight;
                
                {
                    //get current wt value
                    float wtValueLeft = wavetable->applyWavetable (inputLeft, 0);
                    float wtValueRight = wavetable->applyWavetable (inputRight, 1);
                    
                    //convert wt value
                    delayInSamplesLeft = delayRange.convertFrom0to1 (wtValueLeft);
                    delayInSamplesRight = delayRange.convertFrom0to1 (wtValueRight);
                }
                //========================================================================
                
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
                float delayedLeft = lpFilter.filterSignal (interpolatedLeft, 0);
                float delayedRight = lpFilter.filterSignal (interpolatedRight, 1);
                
                //apply gain ramps
                dryWetRamp.applyRamp (dryWetPropotion);
                
                //count dry & wet gains
                wetGain = dryWetPropotion;
                dryGain = 1;
                
                //output signal
                leftBufferW [sample] = dryGain * inputLeft + wetGain * delayedLeft;
                
                rightBufferW [sample] = dryGain * inputRight + wetGain * delayedRight;
                
                //store input signal in the delay buffer
                leftDelayW [delayCounter] = (inputLeft + feedbackGain * delayedLeft
                                             + prevSampleGain * prevDelayedLeft);
                
                rightDelayW [delayCounter] = (inputRight + feedbackGain * delayedRight
                                              + prevSampleGain * prevDelayedRight);
                
                //store previous values
                prevDelayedLeft = delayedLeft;
                prevDelayedRight = delayedRight;
                
                //increase counters
                delayCounter++;
            }
        }
        else
        {
            releaseResources();
        }
    }
//=====================================================================================
//    PROCESSING FUNCTIONS FOR ONE SAMPLE
//=====================================================================================
    AudioSampleBuffer processSampleMono (const float leftInput, const float rightInput,
                                         const float* leftDelayR,
                                         const float* rightDelayR,
                                         float* leftDelayW,
                                         float* rightDelayW)
    {
        AudioSampleBuffer output (1, 1);
        float* outputLeft = output.getWritePointer (0, 0);
    }
    
    AudioSampleBuffer processSampleStereo (const float leftInput, const float rightInput,
                                           const float* leftDelayR,
                                           const float* rightDelayR,
                                           float* leftDelayW,
                                           float* rightDelayW)
    {
        //=================================================
        //  PREPARE OUTPUT BUFFER
        //=================================================
        AudioSampleBuffer output (2, 1);
        float* outputLeft = output.getWritePointer (0, 0);
        float* outputRight = output.getWritePointer (1, 0);
        
        //=================================================
        //  DO PROCESSING
        //=================================================
        
        //check counter
        if (delayCounter >= circularBufferSize)
            delayCounter = 0;
        
        //get current delay time
        //========================================================================
        float delayInSamplesLeft;
        float delayInSamplesRight;
        
        {
            //get current wt value
            float wtValueLeft = wavetable->applyWavetable (inputLeft, 0);
            float wtValueRight = wavetable->applyWavetable (inputRight, 1);
            
            //convert wt value
            delayInSamplesLeft = delayRange.convertFrom0to1 (wtValueLeft);
            delayInSamplesRight = delayRange.convertFrom0to1 (wtValueRight);
        }
        //========================================================================
        
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
        float delayedLeft = lpFilter.filterSignal (interpolatedLeft, 0);
        float delayedRight = lpFilter.filterSignal (interpolatedRight, 1);
        
        //apply gain ramps
        dryWetRamp.applyRamp (dryWetPropotion);
        
        //count dry & wet gains
        wetGain = dryWetPropotion;
        dryGain = 1;
        
        //output signal
        leftBufferW [sample] = dryGain * inputLeft + wetGain * delayedLeft;
        
        rightBufferW [sample] = dryGain * inputRight + wetGain * delayedRight;
        
        //store input signal in the delay buffer
        leftDelayW [delayCounter] = (inputLeft + feedbackGain * delayedLeft
                                     + prevSampleGain * prevDelayedLeft);
        
        rightDelayW [delayCounter] = (inputRight + feedbackGain * delayedRight
                                      + prevSampleGain * prevDelayedRight);
        
        //store previous values
        prevDelayedLeft = delayedLeft;
        prevDelayedRight = delayedRight;
        
        //increase counters
        delayCounter++;
    }
};

