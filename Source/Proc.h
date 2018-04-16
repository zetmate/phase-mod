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
    Proc()  : sampleRate (1), resourcesReleased (false), separateProcessing (true),
                dryWetPropotion(1), prevOutputLeft (0.0f), prevOutputRight (0.0f),
                globalFeedbackGain (0)
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
        processBlock = &Proc::processBlockMono;
        
        //set internal processors to mono behaviour
        voiceClose.setToMono();
        voiceMid.setToMono();
        voiceFar.setToMono();
        voiceEcho.setToMono();
        
        //set wavetavle to mono
        
    }
    
    void setToStereo()
    {
        //set appropriate processing functions
        processBlock = &Proc::processBlockStereo;
        
        //set internal processors to stereo behaviour
        voiceClose.setToStereo();
        voiceMid.setToStereo();
        voiceFar.setToStereo();
        voiceEcho.setToStereo();
        
        //set wavetavle to stereo
        
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
        separateProcessing = true;
    }
    
    void setCascadeProcessing()
    {
        separateProcessing = false;
    }
    
    void setGlobalFeedbackGain (float feedbackGain)
    {
        globalFeedbackGain = std::min (0.99f, feedbackGain);
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
    float globalFeedbackGain;
    
    //DAW transport state object
    AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    
private:
    //Mono processing functions
    void processBlockMono (AudioSampleBuffer& buffer, AudioPlayHead* playHead)
    {
        //constants
        const int numSamples = buffer.getNumSamples();
        
        //get pointers to buffer
        const float* bufferR = buffer.getReadPointer(0);
        float* bufferW = buffer.getWritePointer (0);
        
    };
    
    //Stereo processing functions
    
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
                
                //voices' inputs
                float voiceClose_inputLeft = 0.0;
                float voiceClose_inputRight = 0.0;
                //
                float voiceMid_inputLeft = 0.0;
                float voiceMid_inputRight = 0.0;
                //
                float voiceFar_inputLeft = 0.0;
                float voiceFar_inputRight = 0.0;
                //
                float voiceEcho_inputLeft = 0.0;
                float voiceEcho_inputRight = 0.0;
                
                //============================================
                //  SET INPUT DEPENDING ON PROCESSING TYPE
                //============================================
                if (separateProcessing)
                {
                    //FOR SEPARATE PROCESSING
                    
                    //voice close
                    voiceClose_inputLeft = inputLeft;
                    voiceClose_inputRight = inputRight;
                    
                    //voice mid
                    voiceMid_inputLeft = inputLeft;
                    voiceMid_inputRight = inputRight;
                    
                    //voice far
                    voiceFar_inputLeft = inputLeft;
                    voiceFar_inputRight = inputRight;
                    
                    //voice echo
                    voiceEcho_inputLeft = inputLeft;
                    voiceEcho_inputRight = inputRight;
                }
                else
                {
                    //FOR CASCADE PROCESSING
                    
                    //voice close
                    voiceClose_inputLeft = inputLeft + prevOutputLeft * globalFeedbackGain;
                    voiceClose_inputRight = inputRight + prevOutputRight * globalFeedbackGain;
                    
                    //voice mid
                    voiceMid_inputLeft = voiceClose_outputLeft;
                    voiceMid_inputRight = voiceClose_outputRight;
                    
                    //voice far
                    voiceFar_inputLeft = voiceMid_outputLeft;
                    voiceFar_inputRight = voiceMid_outputRight;
                    
                    //voice echo
                    voiceEcho_inputLeft = voiceFar_outputLeft;
                    voiceEcho_inputRight = voiceFar_outputRight;
                }
                
                //============================================
                //  PROCESS VOICES
                //============================================
                
                //voice Close
                voiceClose.processSample (voiceClose,
                                          voiceClose_inputLeft,
                                          voiceClose_inputRight,
                                          voiceClose_outputLeft, voiceClose_outputRight,
                                          voiceClose_LeftDelayR,
                                          voiceClose_RightDelayR,
                                          voiceClose_LeftDelayW,
                                          voiceClose_RightDelayW);
                
                //voice Mid
                voiceMid.processSample (voiceMid,
                                        voiceMid_inputLeft,
                                        voiceMid_inputRight,
                                        voiceMid_outputLeft, voiceMid_outputRight,
                                        voiceMid_LeftDelayR,
                                        voiceMid_RightDelayR,
                                        voiceMid_LeftDelayW,
                                        voiceMid_RightDelayW);
                
                //voice Far
                voiceFar.processSample (voiceFar,
                                        voiceFar_inputLeft,
                                        voiceFar_inputRight,
                                        voiceFar_outputLeft, voiceFar_outputRight,
                                        voiceFar_LeftDelayR,
                                        voiceFar_RightDelayR,
                                        voiceFar_LeftDelayW,
                                        voiceFar_RightDelayW);
                
                //voice Echo
                voiceEcho.processSample (voiceEcho,
                                         voiceEcho_inputLeft,
                                         voiceEcho_inputRight,
                                         voiceEcho_outputLeft, voiceEcho_outputRight,
                                         voiceEcho_LeftDelayR,
                                         voiceEcho_RightDelayR,
                                         voiceEcho_LeftDelayW,
                                         voiceEcho_RightDelayW);
                
                //============================================
                //  COMPUTE OUTPUT
                //============================================
                
                //get current dry/wet propotion value
                dryWetRamp.applyRamp (dryWetPropotion);
                float wetGain = dryWetPropotion;
                float dryGain = 1 - wetGain;
                
                //output signal
                float outputLeft = 0.0;
                float outputRight = 0.0;
                
//                if (separateProcessing)
//                {
//                    outputLeft = (voiceClose_outputLeft
//                                  + voiceMid_outputLeft
//                                  + voiceFar_outputLeft
//                                  + voiceEcho_outputLeft) * 0.25;
//                    
//                    outputRight = (voiceClose_outputRight
//                                  + voiceMid_outputRight
//                                  + voiceFar_outputRight
//                                  + voiceEcho_outputRight) * 0.25;
//                }
//                else
//                {
//                    outputLeft = voiceEcho_outputLeft;
//                    outputRight = voiceEcho_outputRight;
//                }
                
                if (separateProcessing)
                {
                    float voiceClose = voiceClose_outputLeft + voiceClose_outputRight;
                    float voiceMid = voiceMid_outputLeft + voiceMid_outputRight;
                    float voiceFar = voiceFar_outputLeft + voiceFar_outputRight;
                    float voiceEcho = voiceEcho_outputLeft + voiceEcho_outputRight;
                    
                    outputLeft = voiceClose + voiceFar;
                    outputRight = voiceMid + voiceEcho;
                }
                else
                {
                    outputLeft = voiceEcho_outputLeft;
                    outputRight = voiceEcho_outputRight;
                }
                
                leftBufferW [sample] = outputLeft * wetGain + inputLeft * dryGain;
                rightBufferW [sample] = outputRight * wetGain + inputRight * dryGain;
                
                //store output values
                prevOutputLeft = outputLeft;
                prevOutputRight = outputRight;
            }
        }
        else
        {
            releaseResources();
        }
    }
};

