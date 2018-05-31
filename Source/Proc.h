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
        stereo,
        monoToStereo
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
        dryWetRamp.setTime (5, sampleRate);
        dryWetRamp.updateInterval();
        
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
    
    void setGlobalDryWet (float newDryWetPropotion)
    {
        dryWetRamp.setRange (dryWetPropotion, newDryWetPropotion);
    }
    
    void setFeedbackPolarity (bool shouldBePositive)
    {
        voiceClose.setFeedbackPolarity (shouldBePositive);
        voiceMid.setFeedbackPolarity (shouldBePositive);
        voiceFar.setFeedbackPolarity (shouldBePositive);
        voiceEcho.setFeedbackPolarity (shouldBePositive);
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
    
    void setDoubleFeedback (bool doubleFeedback, float feedbackValue)
    {
        if (doubleFeedback)
        {
            float prevSampleGain = Utility::thinCurveDown (feedbackValue, 0.05, 0.9, 1.1, 0.99, 0.99);
            
            voiceClose.setPrevSampleGain (prevSampleGain);
            voiceMid.setPrevSampleGain (prevSampleGain);
            voiceFar.setPrevSampleGain (prevSampleGain);
            voiceEcho.setPrevSampleGain (prevSampleGain);
        }
        else
        {
            voiceClose.setPrevSampleGain (0);
            voiceMid.setPrevSampleGain (0);
            voiceFar.setPrevSampleGain (0);
            voiceEcho.setPrevSampleGain (0);
        }
    }
    
    void setPrevSampleGain (float gain)
    {
        voiceClose.setPrevSampleGain (gain);
        voiceMid.setPrevSampleGain (gain);
        voiceFar.setPrevSampleGain (gain);
        voiceEcho.setPrevSampleGain (gain);
    }
    
    void setLowCut1 (float frequency)
    {
        voiceClose.setLowCutFrequency (frequency);
        voiceMid.setLowCutFrequency (frequency);
    }
    
    void setLowCut2 (float frequency)
    {
        voiceFar.setLowCutFrequency (frequency);
        voiceEcho.setLowCutFrequency (frequency);
    }
    
    void setHighCut1 (float frequency)
    {
        voiceClose.setHighCutFrequency (frequency);
        voiceMid.setHighCutFrequency (frequency);
    }
    
    void setHighCut2 (float frequency)
    {
        voiceFar.setHighCutFrequency (frequency);
        voiceEcho.setHighCutFrequency (frequency);
    }
    
    void setReso (float resonance)
    {
        voiceClose.setResonance (resonance);
        voiceMid.setResonance (resonance);
        voiceFar.setResonance (resonance);
        voiceEcho.setResonance (resonance);
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
        if (channelSet == stereo)
        {
            if (processorType == separate)
                processBlockStereoSeparate (buffer, playHead);
            else
                processBlockStereoCascade (buffer, playHead);
        }
        else
        {
            if (processorType == separate)
                processBlockMonoSeparate (buffer, playHead);
            else
                processBlockMonoCascade (buffer, playHead);
        }
    }
    
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
    
    //DAW transport state object
    AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    
private:
    
    //Mono processing functions
    void processBlockMonoSeparate (AudioSampleBuffer& buffer, AudioPlayHead* playHead)
    {
        //prepare transport state info
        bool transportIsAvailable = playHead->getCurrentPosition(currentPositionInfo);
        
        //process only if the transport state = is playing or not available
        if (currentPositionInfo.isPlaying || !(transportIsAvailable))
        {
            //constants
            const int numSamples = buffer.getNumSamples();
            
            //get pointers to buffer
            const float* bufferR = buffer.getReadPointer(0);
            float* bufferW = buffer.getWritePointer (0);
            
            //============================================
            //  GET POINTERS TO DELAY BUFFERS
            //============================================
            
            //voice close
            const float* voiceClose_DelayR = voiceClose.getReadPointerToDelayBuffer (0);
            float* voiceClose_DelayW = voiceClose.getWritePointerToDelayBuffer (0);
            
            //voice mid
            const float* voiceMid_DelayR = voiceMid.getReadPointerToDelayBuffer (0);
            float* voiceMid_DelayW = voiceMid.getWritePointerToDelayBuffer (0);
            
            //voice far
            const float* voiceFar_DelayR = voiceFar.getReadPointerToDelayBuffer (0);
            float* voiceFar_DelayW = voiceFar.getWritePointerToDelayBuffer (0);
            
            //voice echo
            const float* voiceEcho_DelayR = voiceEcho.getReadPointerToDelayBuffer (0);
            float* voiceEcho_DelayW = voiceEcho.getWritePointerToDelayBuffer (0);
            
            
            //============================================
            //  PROCESS EACH SAMPLE
            //============================================
            
            for (int sample = 0; sample < numSamples; ++sample)
            {
                //get input signal
                const float input = bufferR[sample];
                
                //============================================
                //  DECLARE VARIABLES
                //============================================
                
                //voices' outputs
                float voiceClose_output = 0.0;
                float voiceMid_output = 0.0;
                float voiceFar_output = 0.0;
                float voiceEcho_output = 0.0;
                
                //============================================
                //  PROCESS VOICES
                //============================================
                
                //voice Close
                voiceClose.processSampleMono (input, voiceClose_output,
                                              voiceClose_DelayR, voiceClose_DelayW);
                
                //voice Mid
                voiceMid.processSampleMono (input, voiceMid_output,
                                            voiceMid_DelayR, voiceMid_DelayW);
                
                //voice Far
                voiceFar.processSampleMono (input, voiceFar_output,
                                            voiceFar_DelayR, voiceFar_DelayW);
                
                //voice Echo
                voiceEcho.processSampleMono (input, voiceEcho_output,
                                             voiceEcho_DelayR, voiceEcho_DelayW);
                
                //============================================
                //  COMPUTE RESULTS
                //============================================
                
                float processed = (voiceClose_output
                                   + voiceMid_output
                                   + voiceFar_output
                                   + voiceEcho_output) * 0.5;
                
                //============================================
                //  LIMIT PROCESSED SIGNAL
                //============================================
                float output = 0.0;
                {
                    float g = limiterLeft.getGainReductionValueForSample (processed);
                    
                    output = processed * g;
                }
                
                //============================================
                // COMPUTE DRY/WET
                //============================================
                
                //get current dry/wet propotion value
                dryWetRamp.applyRamp (dryWetPropotion);
                float wetGain = dryWetPropotion;
                float dryGain = 1 - wetGain;
                float k = Utility::fatCurveUp (wetGain, 1, 1.5, 1.3, 1, 1);
                
                bufferW [sample] = (output * wetGain + input * dryGain) * k;
            }
        }
        else
        {
            releaseResources();
        }
    };
    
    void processBlockMonoCascade (AudioSampleBuffer& buffer, AudioPlayHead* playHead)
    {
        //prepare transport state info
        bool transportIsAvailable = playHead->getCurrentPosition(currentPositionInfo);
        
        //process only if the transport state = is playing or not available
        if (currentPositionInfo.isPlaying || !(transportIsAvailable))
        {
            //constants
            const int numSamples = buffer.getNumSamples();
            
            //get pointers to buffer
            const float* bufferR = buffer.getReadPointer(0);
            float* bufferW = buffer.getWritePointer (0);
            
            //============================================
            //  GET POINTERS TO DELAY BUFFERS
            //============================================
            
            //voice close
            const float* voiceClose_DelayR = voiceClose.getReadPointerToDelayBuffer (0);
            float* voiceClose_DelayW = voiceClose.getWritePointerToDelayBuffer (0);
            
            //voice mid
            const float* voiceMid_DelayR = voiceMid.getReadPointerToDelayBuffer (0);
            float* voiceMid_DelayW = voiceMid.getWritePointerToDelayBuffer (0);
            
            //voice far
            const float* voiceFar_DelayR = voiceFar.getReadPointerToDelayBuffer (0);
            float* voiceFar_DelayW = voiceFar.getWritePointerToDelayBuffer (0);
            
            //voice echo
            const float* voiceEcho_DelayR = voiceEcho.getReadPointerToDelayBuffer (0);
            float* voiceEcho_DelayW = voiceEcho.getWritePointerToDelayBuffer (0);
            
            
            //============================================
            //  PROCESS EACH SAMPLE
            //============================================
            
            for (int sample = 0; sample < numSamples; ++sample)
            {
                //get input signal
                const float input = bufferR[sample];
                
                //============================================
                //  DECLARE VARIABLES
                //============================================
                
                //voices' outputs
                float voiceClose_output = 0.0;
                float voiceMid_output = 0.0;
                float voiceFar_output = 0.0;
                float voiceEcho_output = 0.0;
                
                //============================================
                //  PROCESS VOICES
                //============================================
                
                //voice Close
                voiceClose.processSampleMono (input, voiceClose_output,
                                              voiceClose_DelayR, voiceClose_DelayW);
                
                //voice Mid
                voiceMid.processSampleMono (input, voiceMid_output,
                                            voiceMid_DelayR, voiceMid_DelayW);
                
                //voice Far
                voiceFar.processSampleMono (input, voiceFar_output,
                                            voiceFar_DelayR, voiceFar_DelayW);
                
                //voice Echo
                voiceEcho.processSampleMono (input, voiceEcho_output,
                                             voiceEcho_DelayR, voiceEcho_DelayW);
                
                //============================================
                //  COMPUTE DRY & WET GAINS
                //============================================
                
                //get current dry/wet propotion value
                dryWetRamp.applyRamp (dryWetPropotion);
                float wetGain = dryWetPropotion;
                float dryGain = 1 - wetGain;
                
                //============================================
                //  LIMIT PROCESSED SIGNAL
                //============================================
                
                float g = limiterLeft.getGainReductionValueForSample (voiceEcho_output);
                
                //output signal
                float output = g * voiceEcho_output;
                
                bufferW [sample] = (output * wetGain + input * dryGain);
            }
        }
        else
        {
            releaseResources();
        }
        
    };
    
    //Stereo processing functions
    
    void processBlockStereoSeparate (AudioSampleBuffer& buffer, AudioPlayHead* playHead)
    {
        //prepare transport state info
        bool transportIsAvailable = playHead->getCurrentPosition(currentPositionInfo);
        
        //process only if the transport state = is playing or not available
        if (currentPositionInfo.isPlaying || !(transportIsAvailable))
        {
            //constants
            const int numSamples = buffer.getNumSamples();
            
            //get pointers to buffer
            const float* leftBufferR = buffer.getReadPointer(0);
            const float* rightBufferR = buffer.getReadPointer(1);
            float* leftBufferW = buffer.getWritePointer (0);
            float* rightBufferW = buffer.getWritePointer (1);
            
            //============================================
            //  GET POINTERS TO DELAY BUFFERS
            //============================================
            
            //voice close
            const float* voiceClose_LeftDelayR = voiceClose.getReadPointerToDelayBuffer (0);
            const float* voiceClose_RightDelayR = voiceClose.getReadPointerToDelayBuffer (1);
            float* voiceClose_LeftDelayW = voiceClose.getWritePointerToDelayBuffer (0);
            float* voiceClose_RightDelayW = voiceClose.getWritePointerToDelayBuffer (1);
            
            //voice mid
            const float* voiceMid_LeftDelayR = voiceMid.getReadPointerToDelayBuffer (0);
            const float* voiceMid_RightDelayR = voiceMid.getReadPointerToDelayBuffer (1);
            float* voiceMid_LeftDelayW = voiceMid.getWritePointerToDelayBuffer (0);
            float* voiceMid_RightDelayW = voiceMid.getWritePointerToDelayBuffer (1);
            
            //voice far
            const float* voiceFar_LeftDelayR = voiceFar.getReadPointerToDelayBuffer (0);
            const float* voiceFar_RightDelayR = voiceFar.getReadPointerToDelayBuffer (1);
            float* voiceFar_LeftDelayW = voiceFar.getWritePointerToDelayBuffer (0);
            float* voiceFar_RightDelayW = voiceFar.getWritePointerToDelayBuffer (1);
            
            //voice echo
            const float* voiceEcho_LeftDelayR = voiceEcho.getReadPointerToDelayBuffer (0);
            const float* voiceEcho_RightDelayR = voiceEcho.getReadPointerToDelayBuffer (1);
            float* voiceEcho_LeftDelayW = voiceEcho.getWritePointerToDelayBuffer (0);
            float* voiceEcho_RightDelayW = voiceEcho.getWritePointerToDelayBuffer (1);
            
            
            //============================================
            //  PROCESS EACH SAMPLE
            //============================================
            
            for (int sample = 0; sample < numSamples; ++sample)
            {
                //get input signal
                const float inputLeft = leftBufferR[sample];
                float inputRight;
                
                if (channelSet == stereo)
                    inputRight = rightBufferR[sample];
                else
                    inputRight = inputLeft;
                
                //============================================
                //  DECLARE VARIABLES
                //============================================
                
                //voices' outputs
                float voiceClose_outputLeft = 0.0;
                float voiceClose_outputRight = 0.0;
                //
                float voiceMid_outputLeft = 0.0;
                float voiceMid_outputRight = 0.0;
                //
                float voiceFar_outputLeft = 0.0;
                float voiceFar_outputRight = 0.0;
                //
                float voiceEcho_outputLeft = 0.0;
                float voiceEcho_outputRight = 0.0;
                
                
                //============================================
                //  PROCESS VOICES
                //============================================
                
                //voice Close
                voiceClose.processSampleStereo (inputLeft, inputRight,
                                                voiceClose_outputLeft, voiceClose_outputRight,
                                                voiceClose_LeftDelayR, voiceClose_RightDelayR,
                                                voiceClose_LeftDelayW, voiceClose_RightDelayW);
                
                //voice Mid
                voiceMid.processSampleStereo (inputLeft, inputRight,
                                              voiceMid_outputLeft, voiceMid_outputRight,
                                              voiceMid_LeftDelayR, voiceMid_RightDelayR,
                                              voiceMid_LeftDelayW, voiceMid_RightDelayW);
                
                //voice Far
                voiceFar.processSampleStereo (inputLeft, inputRight,
                                              voiceFar_outputLeft, voiceFar_outputRight,
                                              voiceFar_LeftDelayR, voiceFar_RightDelayR,
                                              voiceFar_LeftDelayW, voiceFar_RightDelayW);
                
                //voice Echo
                voiceEcho.processSampleStereo (inputLeft, inputRight,
                                               voiceEcho_outputLeft, voiceEcho_outputRight,
                                               voiceEcho_LeftDelayR, voiceEcho_RightDelayR,
                                               voiceEcho_LeftDelayW, voiceEcho_RightDelayW);
                
                //============================================
                //  COMPUTE RESULTS
                //============================================
                
                float processedLeft = 0.0;
                float processedRight = 0.0;
                {
                    float voiceClose = voiceClose_outputLeft + voiceClose_outputRight;
                    float voiceMid = voiceMid_outputLeft + voiceMid_outputRight;
                    float voiceFar = voiceFar_outputLeft + voiceFar_outputRight;
                    float voiceEcho = voiceEcho_outputLeft + voiceEcho_outputRight;
                    
                    processedLeft = (voiceClose + voiceFar) * 0.5;
                    processedRight = (voiceMid + voiceEcho) * 0.5;
                }
                
                //============================================
                //  LIMIT PROCESSED SIGNAL
                //============================================
                float outputLeft = 0.0;
                float outputRight = 0.0;
                {
                    float gLeft = limiterLeft.getGainReductionValueForSample (processedLeft);
                    float gRight = limiterRight.getGainReductionValueForSample (processedRight);
                    
                    outputLeft = processedLeft * gLeft;
                    outputRight = processedRight * gRight;
                }
                
                //============================================
                // COMPUTE DRY/WET
                //============================================
                
                //get current dry/wet propotion value
                dryWetRamp.applyRamp (dryWetPropotion);
                float wetGain = dryWetPropotion;
                float dryGain = 1 - wetGain;
                float k = Utility::fatCurveUp (wetGain, 1, 1.5, 1.3, 1, 1);
                
                leftBufferW [sample] = (outputLeft * wetGain + inputLeft * dryGain) * k;
                rightBufferW [sample] = (outputRight * wetGain + inputRight * dryGain) * k;
            }
        }
        else
        {
            releaseResources();
        }
    }
    
    void processBlockStereoCascade (AudioSampleBuffer& buffer, AudioPlayHead* playHead)
    {
        //prepare transport state info
        bool transportIsAvailable = playHead->getCurrentPosition(currentPositionInfo);
        
        //process only if the transport state = is playing or not available
        if (currentPositionInfo.isPlaying || !(transportIsAvailable))
        {
            //constants
            const int numSamples = buffer.getNumSamples();
            
            //get pointers to buffer
            const float* leftBufferR = buffer.getReadPointer(0);
            const float* rightBufferR = buffer.getReadPointer(1);
            float* leftBufferW = buffer.getWritePointer (0);
            float* rightBufferW = buffer.getWritePointer (1);
            
            //============================================
            //  GET POINTERS TO DELAY BUFFERS
            //============================================
            
            //voice close
            const float* voiceClose_LeftDelayR = voiceClose.getReadPointerToDelayBuffer (0);
            const float* voiceClose_RightDelayR = voiceClose.getReadPointerToDelayBuffer (1);
            float* voiceClose_LeftDelayW = voiceClose.getWritePointerToDelayBuffer (0);
            float* voiceClose_RightDelayW = voiceClose.getWritePointerToDelayBuffer (1);
            
            //voice mid
            const float* voiceMid_LeftDelayR = voiceMid.getReadPointerToDelayBuffer (0);
            const float* voiceMid_RightDelayR = voiceMid.getReadPointerToDelayBuffer (1);
            float* voiceMid_LeftDelayW = voiceMid.getWritePointerToDelayBuffer (0);
            float* voiceMid_RightDelayW = voiceMid.getWritePointerToDelayBuffer (1);
            
            //voice far
            const float* voiceFar_LeftDelayR = voiceFar.getReadPointerToDelayBuffer (0);
            const float* voiceFar_RightDelayR = voiceFar.getReadPointerToDelayBuffer (1);
            float* voiceFar_LeftDelayW = voiceFar.getWritePointerToDelayBuffer (0);
            float* voiceFar_RightDelayW = voiceFar.getWritePointerToDelayBuffer (1);
            
            //voice echo
            const float* voiceEcho_LeftDelayR = voiceEcho.getReadPointerToDelayBuffer (0);
            const float* voiceEcho_RightDelayR = voiceEcho.getReadPointerToDelayBuffer (1);
            float* voiceEcho_LeftDelayW = voiceEcho.getWritePointerToDelayBuffer (0);
            float* voiceEcho_RightDelayW = voiceEcho.getWritePointerToDelayBuffer (1);
            
            
            //============================================
            //  PROCESS EACH SAMPLE
            //============================================
            
            for (int sample = 0; sample < numSamples; ++sample)
            {
                //get input signal
                const float inputLeft = leftBufferR[sample];
                const float inputRight = rightBufferR[sample];
                
                //============================================
                //  DECLARE VARIABLES
                //============================================
                
                //voices' outputs
                float voiceClose_outputLeft = 0.0;
                float voiceClose_outputRight = 0.0;
                //
                float voiceMid_outputLeft = 0.0;
                float voiceMid_outputRight = 0.0;
                //
                float voiceFar_outputLeft = 0.0;
                float voiceFar_outputRight = 0.0;
                //
                float voiceEcho_outputLeft = 0.0;
                float voiceEcho_outputRight = 0.0;
                
                //============================================
                //  PROCESS VOICES
                //============================================
                
                //voice Close
                voiceClose.processSampleStereo (inputLeft, inputRight,
                                                voiceClose_outputLeft, voiceClose_outputRight,
                                                voiceClose_LeftDelayR, voiceClose_RightDelayR,
                                                voiceClose_LeftDelayW, voiceClose_RightDelayW);
                
                //voice Mid
                voiceMid.processSampleStereo (voiceClose_outputLeft, voiceClose_outputRight,
                                              voiceMid_outputLeft, voiceMid_outputRight,
                                              voiceMid_LeftDelayR, voiceMid_RightDelayR,
                                              voiceMid_LeftDelayW, voiceMid_RightDelayW);
                
                //voice Far
                voiceFar.processSampleStereo (voiceMid_outputLeft, voiceMid_outputRight,
                                              voiceFar_outputLeft, voiceFar_outputRight,
                                              voiceFar_LeftDelayR, voiceFar_RightDelayR,
                                              voiceFar_LeftDelayW, voiceFar_RightDelayW);
                
                //voice Echo
                voiceEcho.processSampleStereo (voiceFar_outputLeft, voiceFar_outputRight,
                                               voiceEcho_outputLeft, voiceEcho_outputRight,
                                               voiceEcho_LeftDelayR, voiceEcho_RightDelayR,
                                               voiceEcho_LeftDelayW, voiceEcho_RightDelayW);
                
                //============================================
                //  COMPUTE DRY & WET GAINS
                //============================================
                
                //get current dry/wet propotion value
                dryWetRamp.applyRamp (dryWetPropotion);
                float wetGain = dryWetPropotion;
                float dryGain = 1 - wetGain;
                
                //============================================
                //  LIMIT PROCESSED SIGNAL
                //============================================
                
                float g = limiterLeft.getGainReductionValueForSample ((voiceEcho_outputLeft
                                                                      + voiceEcho_outputRight)
                                                                      * 0.5);
                
                //output signal
                float outputLeft = g * voiceEcho_outputLeft;
                float outputRight = g * voiceEcho_outputRight;
                
                leftBufferW [sample] = (outputLeft * wetGain + inputLeft * dryGain);
                rightBufferW [sample] = (outputRight * wetGain + inputRight * dryGain);
            }
        }
        else
        {
            releaseResources();
        }
    }
};



