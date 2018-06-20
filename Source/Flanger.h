/*
  ==============================================================================

    Proc.h
    Created: 18 Jan 2018 1:19:46pm
    Author:  Macbook

  ==============================================================================
*/

#pragma once

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
    
    enum LfoShape
    {
        sin,
        triangle,
        saw,
        square,
        random,
        noise
    };
    
    Flanger()  : sampleRate (1), channelSet (mono), resourcesReleased(false), minDelayMs(0), maxDelayMs (39), delayBuffer (0, 0), circularBufferSize (0), delayCounter(0), prevDelayedLeft(0), prevDelayedRight(0), feedbackGain(0), prevSampleGain(0), dryGain(0), wetGain(0), dryWetPropotion(1), depthFrom0to1(0.3), maxDelaySamp(0), minDelaySamp(4), lfoShape_delay(sin), lfoShape_feedback(sin), lfoCounter_delay(0), lfoCounter_feedback(0), lfoNumSamples_delay(0), lfoNumSamples_feedback(0), lfoFreq_delay(0.01), lfoFreq_feedback(1), prevLfoValue_delay(0), delayLfoOn(true), fbLfoOn(false)
    {
        //initialise smart pointers with objects
        
        //set initial values
        setDryWetMix (1);
        delayRange.start = 6;
        hpFilter.frequency = 20;
        hpFilter.quality = 0.7;
        
        //set wt parameters
        
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
        //set time
        dryWetRamp.setTime (5, sampleRate);
        feedbackRamp.setTime (2, sampleRate);
        prevSampleGainRamp.setTime (5, sampleRate);
        minDelayRamp.setTime (250, sampleRate);
        maxDelayRamp.setTime (250, sampleRate);
        lfoFreqRamp_delay.setTime (70, sampleRate);
        lfoFreqRamp_feedback.setTime (70, sampleRate);
        currentDelayRamp.setTime (15, sampleRate);
        lfoAmpRamp_feedback.setTime (5, sampleRate);
        
        //prepare filters
        //count coefficients
        hpFilter.countCoefficients (sampleRate);
        
        //prepare ranges: set start, end, interval, skew
        setMinDelayTime (minDelayMs);
        setMaxDelayTime (maxDelayMs);
        minDelaySamp = minDelayRamp.getEndValue();
        maxDelaySamp = maxDelayRamp.getEndValue();
        
        //prepare lfos
        //set sample rate, range, mono/stereo & count wtt
        lfoNumSamples_delay = sampleRate / lfoFreq_delay;
        lfoNumSamples_feedback = sampleRate / lfoFreq_feedback;
        lfoFreqRamp_delay.setRange (lfoNumSamples_delay, lfoNumSamples_delay);
        lfoFreqRamp_feedback.setRange (lfoNumSamples_feedback, lfoNumSamples_feedback);
        randomLfo.reset();
        
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
            prevDelayedLeft = 0;
            prevDelayedRight = 0;
            
            //clear filters' buffers
            hpFilter.clearBuffers();
            
            //reset wavetables
            randomLfo.reset();
            
            //reset counters
            delayCounter = 0;
            lfoCounter_delay = 0;
            lfoCounter_feedback = 0;
            
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
    }
    
    void setToStereo()
    {
        //set channelSet
        channelSet = stereo;
        
        //set wavetavle to stereo
    }
    
    //setters
    //lfo freqs
    void setFrequencyForDelayLfo (float newFrequency)
    {
        lfoFreq_delay = newFrequency;
        lfoFreqRamp_delay.setRange (lfoNumSamples_delay, sampleRate / newFrequency);
    }
    void setFrequencyForFeedbackLfo (float newFrequency)
    {
        lfoFreq_feedback = newFrequency;
        lfoFreqRamp_feedback.setRange (lfoNumSamples_feedback, sampleRate / newFrequency);
    }
    
    //lfo shapes
    void setShapeForDelayLfo (LfoShape lfoShape)
    {
        lfoShape_delay = lfoShape;
    }
    void setShapeForFeedbackLfo (LfoShape lfoShape)
    {
        lfoShape_feedback = lfoShape;
    }
    
    //lfo amp
    void setAmplitudeForFeedbackLfo (double newAmplitude)
    {
        lfoAmpRamp_feedback.setRange (lfoAmp_feedback, newAmplitude);
    }
    
    //lfo on/off
    void setDelayLfoOn (bool shouldBeOn)
    {
        delayLfoOn = shouldBeOn;
        //lfoCounter_delay = lfoNumSamples_delay / 4;
    }
    void setFbLfoOn (bool shouldBeOn)
    {
        fbLfoOn = shouldBeOn;
    }
    
    void setMaxDelayTime (float newMaxDelayMs)
    {
        maxDelayMs = newMaxDelayMs;
        int targetMaxDelaySamp = Utility::msToSamples (maxDelayMs, sampleRate);
        maxDelayRamp.setRange (maxDelaySamp, targetMaxDelaySamp);
        setDepth (depthFrom0to1);
    }
    
    void setMinDelayTime (float newMinDelayMs)
    {
        minDelayMs = newMinDelayMs;
        float minDelaySampTarget = std::max (4, Utility::msToSamples (newMinDelayMs,
                                                                      sampleRate));
        minDelayRamp.setRange (minDelaySamp, minDelaySampTarget);
    }
    
    void setSweepWidth (float sweepWidthMs)
    {
        sweepWidth = std::min ((float)sweepWidthMs, (float)maxDelayMs);
        setMinDelayTime (maxDelayMs - sweepWidthMs);
    }
    
    void setDepth (float newDepthFrom0to1)
    {
        depthFrom0to1 = newDepthFrom0to1;
        setSweepWidth (maxDelayMs * newDepthFrom0to1);
    }
    
    void setFeedbackGain (float newFeedbackGain)
    {
        feedbackRamp.setRange (feedbackGain, newFeedbackGain);
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
        prevSampleGainRamp.setRange (prevSampleGain, newPrevSampGain);
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
    
    friend class Proc;
private:
    double sampleRate;
    ChannelSet channelSet;
    
    bool resourcesReleased;
    
    //filters
    HighPassFilter hpFilter;
    
    NormalisableRange <double> delayRange;
    double minDelayMs, maxDelayMs;
    
    AudioSampleBuffer delayBuffer;
    int circularBufferSize;
    int delayCounter;
    
    //samples for double feedback
    float prevDelayedLeft, prevDelayedRight;
    
    //gain values
    double feedbackGain;
    double prevSampleGain;
    double dryGain;
    double wetGain;
    double dryWetPropotion;
    
    //delay values
    double sweepWidth;
    double depthFrom0to1;
    double maxDelaySamp, minDelaySamp;
    
    //ramps
    Ramp dryWetRamp;
    Ramp feedbackRamp;
    Ramp prevSampleGainRamp;
    Ramp maxDelayRamp;
    Ramp minDelayRamp;
    Ramp lfoFreqRamp_delay;
    Ramp lfoFreqRamp_feedback;
    Ramp currentDelayRamp;
    Ramp lfoAmpRamp_feedback;
    
    //lfo stuff===============
    //shapes
    LfoShape lfoShape_delay;
    LfoShape lfoShape_feedback;
    
    //counters
    int lfoCounter_delay, lfoCounter_feedback;
    
    //numSamples
    double lfoNumSamples_delay, lfoNumSamples_feedback;
    
    //freqs
    double lfoFreq_delay, lfoFreq_feedback;
    
    //amps
    double lfoAmp_feedback = 1;
    
    //other stuff
    double prevLfoValue_delay;
    bool delayLfoOn, fbLfoOn;
    RandomLfo randomLfo;
    
    //DAW transport state object
    AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    
    float getCurrentLfoValue_delay (int counter, int numSamples, LfoShape shape)
    {
        float value = 0;
        float targetValue = 0;
        
        if (shape == sin)
        {
            value = Utility::sinFromMinus1toPlus1 (counter, numSamples,
                                          1.0, 0.0, 0.0) / 2.0 + 0.5;
        }
        else if (shape == triangle)
        {
            value = Utility::triangleFrom0to1 (counter, numSamples,
                                               1.0, 0.0, 0.0);
        }
        else if (shape == saw)
        {
            targetValue = Utility::sawFrom0to1 (counter, numSamples,
                                                1.0, 0.0, 0.0);
            currentDelayRamp.setRange (prevLfoValue_delay, targetValue);
            value = currentDelayRamp.applyRamp (prevLfoValue_delay);
        }
        else if (shape == square)
        {
            targetValue = Utility::squareFrom0to1 (counter, numSamples,
                                             1.0, 0.0, 0.0);
            currentDelayRamp.setRange (prevLfoValue_delay, targetValue);
            value = currentDelayRamp.applyRamp (prevLfoValue_delay);
        }
        else if (shape == random)
        {
            targetValue = randomLfo.randomFrom0to1 (numSamples, 1.0, 0.0, 0.0);
            currentDelayRamp.setRange (prevLfoValue_delay, targetValue);
            value = currentDelayRamp.applyRamp (prevLfoValue_delay);
        }
        else if (shape == noise)
        {
            value = randomLfo.noiseFrom0to1 (numSamples, 1.0, 0.0, 0.0);
        }
        return value;
    }
    
    float getCurrentLfoValue_fb (int counter, int numSamples, LfoShape shape)
    {
        float value = 0;
        
        if (shape == sin)
            value = Utility::sinFromMinus1toPlus1 (counter, numSamples,
                                          1.0, 0.0, 0.0);
        else if (shape == triangle)
            value = Utility::triangleFrom0to1 (counter, numSamples,
                                               1.0, 0.0, 0.0) * 2 - 1;
        else if (shape == saw)
            value = Utility::sawFrom0to1 (counter, numSamples,
                                          1.0, 0.0, 0.0) * 2 - 1;
        else if (shape == square)
            value = Utility::squareFrom0to1 (counter, numSamples,
                                             1.0, 0.0, 0.0) * 2 - 1;
        else if (shape == random)
            value = randomLfo.randomFrom0to1 (numSamples, 1.0, 0.0, 0.0) * 2 - 1;
        
        return value;
    }
};


