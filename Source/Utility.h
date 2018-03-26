/*
 ==============================================================================
 
 Utilities.h
 Created: 21 Jan 2018 11:55:17am
 Author:  Macbook
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==================================================================================
// math utilities

class Utility
{
public:
    static double magnitude (double n)
    {
        if (n < 0)
            n *= -1;
        
        return n;
    }
    
    static bool compareTwoDoubles (double x, double y, int numSignificantDigits)
    {
        return roundToInt (x * pow (10, numSignificantDigits))
        == roundToInt (y * pow (10, numSignificantDigits));
    }
    
    //==================================================================================
    // value conversions
    
    static double toDb (double value)
    {
        return 20 * log10 (value);
    }
    
    static double fromDb (double dB)
    {
        return pow (10, dB / 20);
    }
    
    static int msToSamples (double ms, double sampleRate)
    {
        return roundToInt (sampleRate / 1000 * ms);
    }
    
    static float msToSamplesF (double ms, double sampleRate)
    {
        return sampleRate / 1000 * ms;
    }
    
    static double samplesToMs (double samples, double sampleRate)
    {
        return samples / (sampleRate / 1000);
    }
    
    //==================================================================================
    // curves
    
    static double fatCurveUp (double xValue, double yMin, double yMax,
                              double quality, double width, double xEnd)
    {
        double x = magnitude (xValue - xEnd);
        double y = - (pow (x, quality)) / pow (width, quality) * (yMax - yMin) + yMax;
        return y;
    }
    
    static double fatCurveDown (double xValue, double yMin, double yMax,
                                double quality, double width, double xStart)
    {
        double x = magnitude (xValue - xStart);
        double y = - (pow (x, quality)) / pow (width, quality) * (yMax - yMin) + yMax;
        return y;
    }
    
    static double thinCurveUp (double xValue, double yMin, double yMax,
                               double quality, double width, double xStart)
    {
        double x = magnitude (xValue - xStart);
        double y = pow (x, quality) / pow (width, quality) * (yMax - yMin) + yMin;
        return y;
    }
    
    static double thinCurveDown (double xValue, double yMin, double yMax,
                                 double quality, double width, double xEnd)
    {
        double x = magnitude (xValue - xEnd);
        double y = pow (x, quality) / pow (width, quality) * (yMax - yMin) + yMin;
        return y;
    }
    
    //==================================================================================
    //wavetable oscilators
    static double sinFrom0to1 (double currentSample, double numSamples, double amplitude,
                               double offset, double sampleRate)
    {
        double x = currentSample / (numSamples + 1);
        double period = 2 * double_Pi;
        
        double fixedAmplitude = min (1 - offset, amplitude);
        
        return sin (x * period) / (2 / fixedAmplitude) + (fixedAmplitude / 2) + offset;
    }
    
    //==================================================================================
    //utilities
    static double max (double a, double b)
    {
        if (a > b)
            return a;
        else
            return b;
    }
    
    static double min (double a, double b)
    {
        if (a < b)
            return a;
        else
            return b;
    }
    
    //==================================================================================
    //sample rate convertions
    
    static void oversample (const AudioSampleBuffer& buffer,
                            AudioSampleBuffer& oversampled, int factor)
    {
        const int numChannels = buffer.getNumChannels();
        const int numSamples = buffer.getNumSamples();
        
        oversampled.setSize (numChannels, numSamples * factor);
        oversampled.clear();
        
        for (int channel = 0; channel < numChannels; ++channel)
        {
            const float* read = buffer.getReadPointer (channel);
            float* write = oversampled.getWritePointer (channel);
            
            for (int sample = 0; sample < numSamples; ++sample)
                write[sample * factor] = read[sample];
        }
    }
    
    static void downsample (const AudioSampleBuffer& buffer,
                            AudioSampleBuffer& downsampled, int factor)
    {
        const int numChannels = buffer.getNumChannels();
        const int numSamples = buffer.getNumSamples();
        
        downsampled.setSize (numChannels, numSamples / factor);
        downsampled.clear();
        
        for (int channel = 0; channel < numChannels; ++channel)
        {
            const float* read = buffer.getReadPointer (channel);
            float* write = downsampled.getWritePointer (channel);
            
            for (int sample = 0; sample < numSamples; sample += 4)
                write[sample / 4] = read[sample];
        }
    }
    //==================================================================================
    //INTERPOLATION
    
    static float linearInterpolation (float fract, float sampleN, float sampleN1)
    {
        return (1 - fract) * sampleN + fract * sampleN1;
    }
    
    static float cubicInterpolation (float fract, float samplePrev, float sampleN,
                                     float sampleN1, float sampleN2)
    {
        float c3 = -samplePrev + sampleN - sampleN1 + sampleN2;
        float c2 = samplePrev - sampleN - c3;
        float c1 = sampleN1 - samplePrev;
        float c0 = sampleN;
        
        return c3 * powf (fract, 3) + c2 * powf (fract, 2) + c1 * fract + c0;
    }
    
    //==================================================================================
    //==================================================================================
    //GUI STUFF
    
    static void addSlider (Slider* slider, Label* label,
                           String labelText, double minValue, double maxValue,
                           double interval, double skewMidPoint,
                           String valueSuff, Slider::SliderStyle sliderStyle,
                           Slider::TextEntryBoxPosition textBoxPosition,
                           double initialValue, Component* parent,
                           Slider::Listener* listener,
                           bool makeVisible)
    {
        if (listener != nullptr)
            slider->addListener (listener);
        
        if (parent != nullptr)
        {
            if (slider != nullptr)
            {
                slider->setRange (minValue, maxValue, interval);
                slider->setSkewFactorFromMidPoint (skewMidPoint);
                slider->setTextBoxStyle (textBoxPosition, false, slider->getTextBoxWidth(), slider->getTextBoxHeight());
                slider->setTextValueSuffix (valueSuff);
                slider->setSliderStyle (sliderStyle);
                slider->setValue (initialValue);
                
            }
            
            if (label != nullptr)
            {
                label->attachToComponent (slider, false);
                label->setText (labelText, dontSendNotification);
                label->setJustificationType (Justification::centred);
            }
            
            if (makeVisible)
            {
                parent->addAndMakeVisible (slider);
                parent->addAndMakeVisible (label);
            }
            else
            {
                parent->addChildComponent (slider);
            }
        }
    }
};
