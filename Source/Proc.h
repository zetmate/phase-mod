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
        processBlock = &Proc::processBlockMono;
    }
    
    void setToStereo()
    {
        processBlock = &Proc::processBlockStereo;
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
            
            //set resourcesReleased to true
            resourcesReleased = true;
        }
    }
    
    //functions for connection with GUI
    //----------
    
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
            
            for (int sample = 0; sample < numSamples; ++sample)
            {
                //get input signal
                const float inputLeft = leftBufferR[sample];
                const float inputRight = rightBufferR[sample];
            }
        }
        else
        {
            releaseResources();
        }
    }
};

