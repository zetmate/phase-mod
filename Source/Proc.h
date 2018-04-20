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
    
    Proc()  : sampleRate (1), resourcesReleased (false), separateProcessing (true),
                dryWetPropotion(1), prevOutputLeft (0.0f), prevOutputRight (0.0f),
                channelSet (mono)
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
        voiceClose.prepare (newSampleRate, bufferSize);
        voiceMid.prepare (newSampleRate, bufferSize);
        voiceFar.prepare (newSampleRate, bufferSize);
        voiceEcho.prepare (newSampleRate, bufferSize);
        
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
    
    //functions for changing number of the proceesor's channels
    void setToMono()
    {
        //set appropriate processing functions
        if (separateProcessing)
        {
            processBlock = &Proc::processBlockMonoSeparate;
        }
        else
        {
            processBlock = &Proc::processBlockMonoCascade;
        }
        
        
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
        //set appropriate processing functions
        if (separateProcessing)
        {
            processBlock = &Proc::processBlockStereoSeparate;
        }
        else
        {
            processBlock = &Proc::processBlockStereoCascade;
        }
        
        //set internal processors to stereo behaviour
        voiceClose.setToStereo();
        voiceMid.setToStereo();
        voiceFar.setToStereo();
        voiceEcho.setToStereo();
        
        //set wavetavle to stereo
        
        //set channel set
        channelSet = stereo;
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
    
    //functions for connection with GUI
    void setSeparateProcessing()
    {
        if (channelSet == stereo)
        {
            processBlock = &Proc::processBlockStereoSeparate;
        }
        else
        {
            processBlock = &Proc::processBlockMonoSeparate;
        }
        
        separateProcessing = true;
        prevOutputLeft = 0.0;
        prevOutputRight = 0.0;
    }
    
    void setCascadeProcessing()
    {
        if (channelSet == stereo)
        {
            processBlock = &Proc::processBlockStereoCascade;
        }
        else
        {
            processBlock = &Proc::processBlockMonoCascade;
        }

        separateProcessing = false;
    }
    
    void setGlobalDryWet (float newDryWetPropotion)
    {
        dryWetRamp.setRange (dryWetPropotion, newDryWetPropotion);
    }
    
    //internal processors
    Flanger voiceClose,
            voiceMid,
            voiceFar,
            voiceEcho;
    
    //function pointers
    std::function<void (Proc&, AudioSampleBuffer& buffer, AudioPlayHead* playHead)>
                            processBlock = {};
    
protected:
    double sampleRate;
    bool resourcesReleased;
    bool separateProcessing;
    
    double dryWetPropotion;
    Ramp dryWetRamp;
    
    //for global feedback
    float prevOutputLeft, prevOutputRight;
    
    //channel set
    ChannelSet channelSet;
    
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
                const float inputLeft = bufferR[sample];
                
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
                voiceClose.processSample (voiceClose, inputLeft, 0,
                                          voiceClose_output, voiceClose_output,
                                          voiceClose_DelayR, voiceClose_DelayR,
                                          voiceClose_DelayW, voiceClose_DelayW);
                
                //voice Mid
                voiceMid.processSample (voiceMid, inputLeft, 0,
                                        voiceMid_output, voiceMid_output,
                                        voiceMid_DelayR, voiceMid_DelayR,
                                        voiceMid_DelayW, voiceMid_DelayW);
                
                //voice Far
                voiceFar.processSample (voiceFar, inputLeft, 0,
                                        voiceFar_output, voiceFar_output,
                                        voiceFar_DelayR, voiceFar_DelayR,
                                        voiceFar_DelayW, voiceFar_DelayW);
                
                //voice Echo
                voiceEcho.processSample (voiceEcho, inputLeft, 0,
                                         voiceEcho_output, voiceEcho_output,
                                         voiceEcho_DelayR, voiceEcho_DelayR,
                                         voiceEcho_DelayW, voiceEcho_DelayW);
                
                //============================================
                //  COMPUTE RESULTS
                //============================================
                
                float processedLeft = 0.0;
                float processedRight = 0.0;
                {
                    float voiceClose = voiceClose_output;
                    float voiceMid = voiceMid_output;
                    float voiceFar = voiceFar_output;
                    float voiceEcho = voiceEcho_output;
                    
                    processedLeft = voiceClose + voiceFar;
                    processedRight = voiceMid + voiceEcho;
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
                float dryGain = 1;
                float k = Utility::fatCurveDown (dryWetPropotion, 0.5, 1, 1.5, 1, 0);
                
                bufferW [sample] = (outputLeft * wetGain + inputLeft * dryGain) * k;
            }
        }
        else
        {
            releaseResources();
        }
    };
    
    void processBlockMonoCascade (AudioSampleBuffer& buffer, AudioPlayHead* playHead)
    {
        //constants
        const int numSamples = buffer.getNumSamples();
        
        //get pointers to buffer
        const float* bufferR = buffer.getReadPointer(0);
        float* bufferW = buffer.getWritePointer (0);
        
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
                voiceClose.processSample (voiceClose,
                                          inputLeft,
                                          inputRight,
                                          voiceClose_outputLeft, voiceClose_outputRight,
                                          voiceClose_LeftDelayR,
                                          voiceClose_RightDelayR,
                                          voiceClose_LeftDelayW,
                                          voiceClose_RightDelayW);
                
                //voice Mid
                voiceMid.processSample (voiceMid,
                                        inputLeft,
                                        inputRight,
                                        voiceMid_outputLeft, voiceMid_outputRight,
                                        voiceMid_LeftDelayR,
                                        voiceMid_RightDelayR,
                                        voiceMid_LeftDelayW,
                                        voiceMid_RightDelayW);
                
                //voice Far
                voiceFar.processSample (voiceFar,
                                        inputLeft,
                                        inputRight,
                                        voiceFar_outputLeft, voiceFar_outputRight,
                                        voiceFar_LeftDelayR,
                                        voiceFar_RightDelayR,
                                        voiceFar_LeftDelayW,
                                        voiceFar_RightDelayW);
                
                //voice Echo
                voiceEcho.processSample (voiceEcho,
                                         inputLeft,
                                         inputRight,
                                         voiceEcho_outputLeft, voiceEcho_outputRight,
                                         voiceEcho_LeftDelayR,
                                         voiceEcho_RightDelayR,
                                         voiceEcho_LeftDelayW,
                                         voiceEcho_RightDelayW);
                
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
        //if (true)
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
                voiceClose.processSample (voiceClose,
                                          inputLeft,
                                          inputRight,
                                          voiceClose_outputLeft, voiceClose_outputRight,
                                          voiceClose_LeftDelayR,
                                          voiceClose_RightDelayR,
                                          voiceClose_LeftDelayW,
                                          voiceClose_RightDelayW);
                
                //voice Mid
                voiceMid.processSample (voiceMid,
                                        voiceClose_outputLeft,
                                        voiceClose_outputRight,
                                        voiceMid_outputLeft, voiceMid_outputRight,
                                        voiceMid_LeftDelayR,
                                        voiceMid_RightDelayR,
                                        voiceMid_LeftDelayW,
                                        voiceMid_RightDelayW);
                
                //voice Far
                voiceFar.processSample (voiceFar,
                                        voiceMid_outputLeft,
                                        voiceMid_outputRight,
                                        voiceFar_outputLeft, voiceFar_outputRight,
                                        voiceFar_LeftDelayR,
                                        voiceFar_RightDelayR,
                                        voiceFar_LeftDelayW,
                                        voiceFar_RightDelayW);
                
                //voice Echo
                voiceEcho.processSample (voiceEcho,
                                         voiceFar_outputLeft,
                                         voiceFar_outputRight,
                                         voiceEcho_outputLeft, voiceEcho_outputRight,
                                         voiceEcho_LeftDelayR,
                                         voiceEcho_RightDelayR,
                                         voiceEcho_LeftDelayW,
                                         voiceEcho_RightDelayW);
                
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



