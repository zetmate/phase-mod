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
    
    if (lfoCounter_delay >= lfoNumSamples_delay)
        lfoCounter_delay = 0;
    
    if (lfoCounter_feedback >= lfoNumSamples_feedback)
        lfoCounter_feedback = 0;
    
    //get current values from ramps
    feedbackRamp.applyRamp (feedbackGain);
    minDelayRamp.applyRamp (minDelaySamp);
    maxDelayRamp.applyRamp (maxDelaySamp);
    lfoFreqRamp_delay.applyRamp (lfoNumSamples_delay);
    lfoFreqRamp_feedback.applyRamp (lfoNumSamples_feedback);
    
    //get current delay time
    //========================================================================
    float delayInSamples;
    
    //update delay range
    delayRange.start = floor (minDelaySamp);
    delayRange.end = floor (maxDelaySamp);
    
    //get current delay time
    if (lfoDelayOn)
    {
        //round current lfoNumSamples to int
        int lfoNumSamples_int = floor (lfoNumSamples_delay);
        
        //get current lfo value
        float lfoValue = getCurrentLfoValue_delay (lfoCounter_delay, lfoNumSamples_int,
                                                   lfoShape_delay);
        
        //convert wt value
        delayInSamples = delayRange.convertFrom0to1 (lfoValue);
    }
    else
    {
        delayInSamples = floor (maxDelaySamp);
    }
    //========================================================================
    
    //interpolate samples
    //========================================================================
    float interpolated = Utility::fractDelayComplexInt (delayR,
                                                        delayInSamples,
                                                        delayCounter,
                                                        circularBufferSize);
    //=========================================================================
    
    //filter interpolated samples
    float filtered = hpFilter.filterSignal (interpolated, 0);
    
    //apply gain ramps
    dryWetRamp.applyRamp (dryWetPropotion);
    
    //count dry & wet gains
    wetGain = dryWetPropotion;
    dryGain = 1 - wetGain;
    
    //output signal
    output = dryGain * input + wetGain * filtered;
    
    //get current feedbackGain from lfo
    if (lfoFbOn)
    {
        float fbLfoValue = getCurrentLfoValue_fb (lfoCounter_feedback, lfoNumSamples_feedback,
                                                  lfoShape_feedback);
        feedbackRamp.setRange (feedbackGain, fbLfoValue * 2 - 1);
    }
    
    //store input signal in the delay buffer
    delayW [delayCounter] = (input + feedbackGain * filtered
                             + prevSampleGain * prevDelayedLeft);
    
    //store previous values
    prevDelayedLeft = filtered;
    
    //increase counters
    delayCounter++;
    lfoCounter_delay++;
    lfoCounter_feedback++;
}

void Flanger::processSampleStereo (const float inputLeft, const float inputRight,
                                   float& outputLeft, float& outputRight,
                                   const float* leftDelayR,
                                   const float* rightDelayR,
                                   float* leftDelayW,
                                   float* rightDelayW)
{
    //get ramps values
    feedbackRamp.applyRamp (feedbackGain);
    minDelayRamp.applyRamp (minDelaySamp);
    maxDelayRamp.applyRamp (maxDelaySamp);
    lfoFreqRamp_delay.applyRamp (lfoNumSamples_delay);
    lfoFreqRamp_feedback.applyRamp (lfoNumSamples_feedback);
    
    //check counter
    if (delayCounter >= circularBufferSize)
        delayCounter = 0;
   
    if (lfoCounter_delay >= lfoNumSamples_delay)
        lfoCounter_delay = 0;
    
    if (lfoCounter_feedback >= lfoNumSamples_feedback)
        lfoCounter_feedback = 0;
    
    //get current delay time
    //========================================================================
    float delayInSamples;
    
    //update delay range
    delayRange.start = floor (minDelaySamp);
    delayRange.end = floor (maxDelaySamp);
    
    //get current delay time
    if (lfoDelayOn)
    {
        //round current lfoNumSamples to int
        int lfoNumSamples_int = floor (lfoNumSamples_delay);
        
        //get current lfo value
        float lfoValue = getCurrentLfoValue_delay (lfoCounter_delay, lfoNumSamples_int,
                                                   lfoShape_delay);
        
        //convert wt value
        delayInSamples = delayRange.convertFrom0to1 (lfoValue);
    }
    else
    {
        delayInSamples = floor (maxDelaySamp);
    }
    //========================================================================
    
    //interpolate samples
    //========================================================================
    float interpolatedLeft = Utility::fractDelayComplexInt (leftDelayR,
                                                          delayInSamples,
                                                          delayCounter,
                                                          circularBufferSize);
    
    float interpolatedRight = Utility::fractDelayComplexInt (rightDelayR,
                                                           delayInSamples,
                                                           delayCounter,
                                                           circularBufferSize);
    //=========================================================================
    
    //filter interpolated samples
    float filteredLeft = hpFilter.filterSignal (interpolatedLeft, 0);
    float filteredRight = hpFilter.filterSignal (interpolatedRight, 1);
    
    //apply gain ramps
    dryWetRamp.applyRamp (dryWetPropotion);
    
    //count dry & wet gains
    wetGain = dryWetPropotion;
    dryGain = 1 - wetGain;
    
    //output signal
    outputLeft = dryGain * inputLeft + wetGain * filteredLeft;
    
    outputRight = dryGain * inputRight + wetGain * filteredRight;
    
    //get current feedbackGain from lfo
    if (lfoFbOn)
    {
        float fbLfoValue = getCurrentLfoValue_fb (lfoCounter_feedback, lfoNumSamples_feedback,
                                                  lfoShape_feedback);
        feedbackRamp.setRange (feedbackGain, fbLfoValue * 2 - 1);
    }
    
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
    lfoCounter_delay++;
    lfoCounter_feedback++;
}
