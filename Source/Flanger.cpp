/*
  ==============================================================================

    Flanger.cpp
    Created: 20 Apr 2018 11:49:23am
    Author:  Macbook

  ==============================================================================
*/

#include "Flanger.h"

//=====================================================================================
//    PROCESSING FUNCTIONS FOR ONE SAMPLE
//=====================================================================================
void Flanger::processSampleMono (const float input,
                                 float& output,
                                 const float* delayR,
                                 float* delayW)
{
    //check counter
    if (delayCounter >= circularBufferSize)
        delayCounter = 0;
    
    //get current delay time
    //========================================================================
    float delayInSamples;
    
    {
        //get current wt value
        float wtValue = wavetable->applyWavetable (input, 0);
        
        //convert wt value
        delayInSamples = delayRange.convertFrom0to1 (wtValue);
    }
    //========================================================================
    
    //interpolate samples
    //========================================================================
    float interpolated = Utility::fractDelayCubicInt (delayR,
                                                      delayInSamples,
                                                      delayCounter,
                                                      circularBufferSize);
    //=========================================================================
    
    //filter interpolated samples
    float delayed = lpFilter.filterSignal (interpolated, 0);
    
    //apply gain ramps
    dryWetRamp.applyRamp (dryWetPropotion);
    
    //count dry & wet gains
    wetGain = dryWetPropotion;
    dryGain = 1 - wetGain;
    
    //output signal
    output = dryGain * input + wetGain * delayed;
    
    //store input signal in the delay buffer
    delayW [delayCounter] = (input + feedbackGain * delayed
                             + prevSampleGain * prevDelayedLeft);
    
    //store previous values
    prevDelayedLeft = delayed;
    
    //increase counters
    delayCounter++;
}

void Flanger::processSampleStereo (const float inputLeft, const float inputRight,
                                   float& outputLeft, float& outputRight,
                                   const float* leftDelayR,
                                   const float* rightDelayR,
                                   float* leftDelayW,
                                   float* rightDelayW)
{
    //check counter
    if (delayCounter >= circularBufferSize)
        delayCounter = 0;
    
    //get current delay time
    //========================================================================
    float delayInSamples;
    
    {
        //get current wt value
        float wtValueLeft = wavetable->applyWavetable (inputLeft, 0);
        
        //convert wt value
        delayInSamples = delayRange.convertFrom0to1 (wtValueLeft);
    }
    //========================================================================
    
    //interpolate samples
    //========================================================================
    float interpolatedLeft = Utility::fractDelayCubicInt (leftDelayR,
                                                          delayInSamples,
                                                          delayCounter,
                                                          circularBufferSize);
    
    float interpolatedRight = Utility::fractDelayCubicInt (rightDelayR,
                                                           delayInSamples,
                                                           delayCounter,
                                                           circularBufferSize);
    //=========================================================================
    
    //filter interpolated samples
    float delayedLeft = lpFilter.filterSignal (interpolatedLeft, 0);
    float delayedRight = lpFilter.filterSignal (interpolatedRight, 1);
    
    float filteredLeft = lpFilter1.filterSignal (delayedLeft, 0);
    float filteredRight = lpFilter1.filterSignal (delayedRight, 1);
    
    //apply gain ramps
    dryWetRamp.applyRamp (dryWetPropotion);
    
    //count dry & wet gains
    wetGain = dryWetPropotion;
    dryGain = 1 - wetGain;
    
    //output signal
    outputLeft = dryGain * inputLeft + wetGain * filteredLeft;
    
    outputRight = dryGain * inputRight + wetGain * filteredRight;
    
    //store input signal in the delay buffer
    
        leftDelayW [delayCounter] = (inputLeft + feedbackGain * filteredLeft
                                     + prevSampleGain * prevDelayedLeft);
        
        rightDelayW [delayCounter] = (inputRight + feedbackGain * filteredRight
                                      + prevSampleGain * prevDelayedRight);
    
    //store previous values
    prevDelayedLeft = filteredLeft;
    prevDelayedRight = filteredRight;
    
    //increase counters
    delayCounter++;
}