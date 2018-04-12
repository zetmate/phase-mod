/*
 ==============================================================================
 
 Compressor.h
 Created: 22 Jan 2018 12:30:23pm
 Author:  Macbook
 
 ==============================================================================
 */

#pragma once

//#include "/Users/Macbook/Desktop/coding/plug-in development/Comp-2/Source/Utilitiy.h"

#include "Utility.h"

class Compressor
{
public:
    Compressor()  : threshold(-1),
    ratio(INFINITY),
    attackMs(2),
    releaseMs(10),
    makeUpGain(-1),
    xg(0, 0),
    xl(0, 0),
    yg(0, 0),
    yl(0, 0),
    c(0, 0),
    xg_prev(-122),
    xl_prev(-122),
    y1_prev(0),
    yl_prev(0)
    
    {
    }
    
    ~Compressor()
    {
    }
    
    void countCompressor (AudioBuffer<float> *buffer, double sampleRate)
    {
        //prepare attack&release
        attackSamples = Utility::msToSamples (attackMs, sampleRate);
        releaseSamples = Utility::msToSamples (releaseMs, sampleRate);
        
        float alphaAttack = expf(-1 / (0.001 * sampleRate * attackMs));
        float alphaRelease = expf (-1 / (0.001 * sampleRate * releaseMs));
        
        const int numChannels = buffer->getNumChannels();
        const int numSamples = buffer->getNumSamples();
        
        //prepare buffers
        xg.setSize (1, numSamples);
        xl.setSize (1, numSamples);
        yg.setSize (1, numSamples);
        yl.setSize (1, numSamples);
        c.setSize (1, numSamples);
        
        //get pointers
        float* xgW = xg.getWritePointer (0);
        float* xlW = xl.getWritePointer (0);
        float* ygW = yg.getWritePointer (0);
        float* ylW = yl.getWritePointer (0);
        float* cW = c.getWritePointer (0);
        
        const float* xgR = xg.getReadPointer(0);
        const float* xlR = xl.getReadPointer(0);
        const float* ygR = yg.getReadPointer(0);
        const float* ylR = yl.getReadPointer(0);
        //const float* cR = c.getReadPointer(0);
        
        for (int sample = 0; sample < numSamples; ++sample)
        {
            const float leftSample = *buffer->getReadPointer(0, sample);
            const float rightSample = *buffer->getReadPointer(1, sample);
            
            const float sampleLevel = (Utility::magnitude(leftSample) + Utility::magnitude(rightSample) / numChannels);
            
            //            //
            
            if (sampleLevel < 0.000001)
                xgW[sample] = -120;
            else
                xgW[sample] = Utility::toDb (sampleLevel);
            //==========================================================
            
            //            GAIN COMPUTER (static)
            
            if (xgR[sample] >= threshold)
                //ygW[sample] = threshold + (xg_prev - threshold) / ratio;
                ygW[sample] = threshold + (1 / ratio - 1) * (sampleLevel - threshold);
            else
                ygW[sample] = xgR[sample];
            
            xg_prev = ygR[sample];
            
            xlW[sample] = xgR[sample] - ygR[sample];
            //==============================================================
            
            //            LEVEL DETECTOR
            
            if (xlR[sample] > yl_prev)
                ylW[sample] = alphaAttack * yl_prev + (1 - alphaAttack) * xlR[sample];
            else
                ylW[sample] = alphaRelease * yl_prev + (1 - alphaRelease) * xlR[sample];
            //ylW[sample] = alphaRelease * yl_prev;
            
            //            float y1 = std::max(xlR[sample], alphaRelease * y1_prev
            //                                + (1 - alphaRelease) * xlR[sample]);
            //
            //            ylW[sample] = alphaAttack * yl_prev + (1 - alphaAttack) * y1;
            
            
            yl_prev = ylR[sample];
            
            //=========================================================================
            
            //            FIND CONTROL
            cW[sample] = Utility::fromDb (makeUpGain - ylR[sample]);
            
            yl_prev = ylR[sample];
            //=========================================================================
        }
    }
    
    float compressSignal (float currentValue, int sample)
    {
        float comp = *c.getReadPointer (0, sample);
        float newValue = currentValue * comp;
        return newValue;
    }
    
    float threshold;
    float ratio;
    float attackMs;
    float releaseMs;
    
    float makeUpGain;
    
private:
    AudioBuffer<float> xg, xl, yg, yl, c;
    
    int attackSamples;
    int releaseSamples;
    
    float xg_prev;
    float xl_prev;
    float y1_prev;
    float yl_prev;
};

