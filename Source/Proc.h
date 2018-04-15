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
    Proc()  : sampleRate (0), resourcesReleased (false)
    {
        //initialise smart pointers with objects
        
        //set initial values
        
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
        
        //prepare ramps
        //set time & update interval
        
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
        
        //set wavetavle to mono
        
    }
    
    void setToStereo()
    {
        //set appropriate processing functions
        processBlock = &Proc::processBlockStereo;
        
        //set internal processors to stereo behaviour
        voiceClose.setToStereo();
        
        //set wavetavle to mono
        
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
            
            //set resourcesReleased to true
            resourcesReleased = true;
        }
    }
    
    //functions for connection with GUI
    //----------
    
    //internal processors
    Flanger voiceClose;
    
    //function pointers
    std::function<void (Proc&, AudioSampleBuffer& buffer, AudioPlayHead* playHead)>
                            processBlock = {};
    
protected:
    double sampleRate;
    bool resourcesReleased;
    
    //DAW transport state object
    AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    
private:
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
        //if (currentPositionInfo.isPlaying || !(transportIsAvailable))
        if (true)
        {
            //constants
            const int numSamples = buffer.getNumSamples();
            
            //get pointers to buffer
            const float* leftBufferR = buffer.getReadPointer(0);
            const float* rightBufferR = buffer.getReadPointer(1);
            float* leftBufferW = buffer.getWritePointer (0);
            float* rightBufferW = buffer.getWritePointer (1);
            
            //======get pointers to delay buffers
            //voice
            const float* voiceClose_LeftDelayR = voiceClose.getReadPointerToDelayBuffer (0);
            const float* voiceClose_RightDelayR = voiceClose.getReadPointerToDelayBuffer (1);
            float* voiceClose_LeftDelayW = voiceClose.getWritePointerToDelayBuffer (0);
            float* voiceClose_RightDelayW = voiceClose.getWritePointerToDelayBuffer (1);
            
            for (int sample = 0; sample < numSamples; ++sample)
            {
                //get input signal
                const float inputLeft = leftBufferR[sample];
                const float inputRight = rightBufferR[sample];
                
                //============================================
                //  PROCESS CLOSE VOICE
                //============================================
                
                //declare variables to store output of the Close voice
                float voiceCloseLeft = 0.0;
                float voiceCloseRight = 0.0;
                
                voiceClose.processSample (voiceClose,
                                          inputLeft, inputRight,
                                          voiceCloseLeft, voiceCloseRight,
                                          voiceClose_LeftDelayR,
                                          voiceClose_RightDelayR,
                                          voiceClose_LeftDelayW,
                                          voiceClose_RightDelayW);
                
                //output signal
                leftBufferW [sample] = voiceCloseLeft;
                rightBufferW [sample] = voiceCloseRight;
            }
        }
        else
        {
            releaseResources();
        }
    }
};

