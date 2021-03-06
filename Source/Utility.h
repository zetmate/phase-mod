/*
 ==============================================================================
 
 Utilities.h
 Created: 21 Jan 2018 11:55:17am
 Author:  Macbook
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Tempo.h"

//==================================================================================
// my utilities

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
    static double sinFromMinus1toPlus1 (double currentSample, double numSamples, double amplitude,
                                        double ampOffset, double phaseOffsetFrom0to1)
    {
        double phaseOffsetInSamples = numSamples * phaseOffsetFrom0to1;
        currentSample += floor (phaseOffsetInSamples);
        
        if (currentSample >= numSamples)
            currentSample = currentSample - numSamples;
        else if (currentSample < 0)
            currentSample = (numSamples - 1) + currentSample;
        
        numSamples += -1;
        
        double correctAmplitude = std::min (1 - ampOffset, amplitude);
        
        double x = currentSample / (numSamples + 1);
        double period = 2 * double_Pi;
        
        return sin (x * period) / (2 / correctAmplitude) + (correctAmplitude / 2) + ampOffset;
    }
    
    static double triangleFrom0to1 (double currentSample, double numSamples,
                                    double amplitude, double ampOffset,
                                    double phaseOffsetFrom0to1)
    {
        double hip = numSamples / 2;
        
        double phaseOffsetInSamples = numSamples * phaseOffsetFrom0to1;
        currentSample += floor (phaseOffsetInSamples);
        
        if (currentSample >= numSamples)
            currentSample = currentSample - numSamples;
        else if (currentSample < 0)
            currentSample = (numSamples - 1) + currentSample;
        
        numSamples += -1;
        
        double correctAmplitude = std::min (1 - ampOffset, amplitude);
        
        double x;
        if (currentSample < hip)
            x = currentSample;
        else
            x = numSamples - currentSample;
        
        return x / numSamples * correctAmplitude + ampOffset;
    }
    
    static double sawFrom0to1 (double currentSample, double numSamples,
                               double amplitude, double ampOffset,
                               double phaseOffsetFrom0to1)
    {
        double phaseOffsetInSamples = numSamples * phaseOffsetFrom0to1;
        currentSample += floor (phaseOffsetInSamples);
        
        if (currentSample >= numSamples)
            currentSample = currentSample - numSamples;
        else if (currentSample < 0)
            currentSample = (numSamples - 1) + currentSample;
        
        numSamples += -1;
        
        double correctAmplitude = std::min (1 - ampOffset, amplitude);
        
        double x = numSamples - currentSample;
        
        return x / numSamples * correctAmplitude + ampOffset;
    }
    
    static double squareFrom0to1 (double currentSample, double numSamples,
                                  double amplitude, double ampOffset,
                                  double phaseOffsetFrom0to1)
    {
        double hip = numSamples / 2;
        
        double phaseOffsetInSamples = numSamples * phaseOffsetFrom0to1;
        currentSample += floor (phaseOffsetInSamples);
        
        if (currentSample >= numSamples)
            currentSample = currentSample - numSamples;
        else if (currentSample < 0)
            currentSample = (numSamples - 1) + currentSample;
        
        numSamples += -1;
        
        double correctAmplitude = std::min (1 - ampOffset, amplitude);
        
        if (currentSample < hip)
        {
            return 1 * correctAmplitude + ampOffset;
        }
        else
        {
            return ampOffset;
        }
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
    //INTERPOLATION & FRACTIONAL DELAYS
    
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
    
    // Optimal 8x (6-point, 5th-order) (z-form)
    static float optimal8x_6point5order_interpolation (float fract,
                                                       float sMin2, float sMin1, float s0,
                                                       float s1, float s2, float s3)
    {
        float z = fract;
        float even1 = s1 + s0, odd1 = s1 - s0;
        float even2 = s2 + sMin1, odd2 = s2 - sMin1;
        float even3 = s3 + sMin2, odd3 = s3 - sMin2;
        
        float c0 = (even1 * 0.41660797292569773 + even2 * 0.08188468587188069
                    + even3 * 0.00150734119050266);
        
        float c1 = (odd1 * 0.32232780822726981 + odd2 * 0.21076321997422021
                    + odd3 * 0.00907649978070957);
        
        float c2 = (even1 * -0.205219993961471501 + even2 * 0.18282942057327367
                    + even3 * 0.02239057377093268);
        
        float c3 = (odd1 * -0.21022298520246224 + odd2 * 0.02176417471349534
                    + odd3 * 0.02898626924395209);
        
        float c4 = (even1 * 0.04149963966704384 + even2 * -0.06224707096203808
                    + even3 * 0.02074742969707599);
        
        float c5 = (odd1 * 0.07517133281176167 + odd2 * -0.03751837438141215
                    + odd3 * 0.00747588873055296);
        
        return ((((c5 * z + c4) * z + c3) * z + c2) * z + c1) * z + c0;
    }
    
    static float fractDelayCubicInt (const float* delayBuffer,
                                     const float delayInSamples,
                                     const int delayCounter,
                                     const int delayBufferSize)
    {
        int delayInSamplesInt = (int) ceilf (delayInSamples);
        float fraction = delayInSamples - floorf (delayInSamples);
        
        //read delayed signal
        int indexPrev = 0;
        int indexN = 0;
        int indexN1 = 0;
        int indexN2 = 0;
        
        //count indexes
        
        if (delayCounter >= delayInSamplesInt)
            indexN = delayCounter - delayInSamplesInt;
        else
            indexN = (delayBufferSize) - (delayInSamplesInt - delayCounter);
        
        if (indexN == 0)
            indexPrev = delayBufferSize - 1;
        else
            indexPrev = indexN - 1;
        
        if (indexN == delayBufferSize - 1)
            indexN1 = 0;
        else
            indexN1 = indexN + 1;
        
        if (indexN1 == delayBufferSize - 1)
            indexN2 = 0;
        else
            indexN2 = indexN1 + 1;
        
        
        //intepolate sample
        
        float interpolatedSample = cubicInterpolation (fraction,
                                                       delayBuffer[indexPrev],
                                                       delayBuffer[indexN],
                                                       delayBuffer[indexN1],
                                                       delayBuffer[indexN2]);
        
        return interpolatedSample;
    }
    
    static float fractDelayComplexInt (const float* delayBuffer,
                                       const float delayInSamples,
                                       const int delayCounter,
                                       const int delayBufferSize)
    {
        int delayInSamplesInt = (int) ceilf (delayInSamples);
        float fraction = delayInSamplesInt - delayInSamples;
        
        //read delayed signal
        int sMin2 = 0;
        int sMin1 = 0;
        int s0 = 0;
        int s1 = 0;
        int s2 = 0;
        int s3 = 0;
        
        //count indexes
        
        if (delayCounter >= delayInSamplesInt)
            s0 = delayCounter - delayInSamplesInt;
        else
            s0 = (delayBufferSize) - (delayInSamplesInt - delayCounter);
        
        if (s0 == 0)
            sMin1 = delayBufferSize - 1;
        else
            sMin1 = s0 - 1;
        
        if (sMin1 == 0)
            sMin2 = delayBufferSize - 1;
        else
            sMin2 = sMin1 - 1;
        
        if (s0 == delayBufferSize - 1)
            s1 = 0;
        else
            s1 = s0 + 1;
        
        if (s1 == delayBufferSize - 1)
            s2 = 0;
        else
            s2 = s1 + 1;
        
        if (s2 == delayBufferSize - 1)
            s3 = 0;
        else
            s3 = s2 + 1;
        
        //intepolate sample
        
        float interpolatedSample = optimal8x_6point5order_interpolation (fraction,
                                                                         delayBuffer[sMin2],
                                                                         delayBuffer[sMin1],
                                                                         delayBuffer[s0],
                                                                         delayBuffer[s1],
                                                                         delayBuffer[s2],
                                                                         delayBuffer[s3]);
        return interpolatedSample;
    }
    
    //==================================================================================
    //==================================================================================
    //GUI STUFF
    
    static void addSlider (Slider* slider, Label* label,
                           const String& labelText, double minValue, double maxValue,
                           double interval, double skewMidPoint,
                           const String& valueSuff, Slider::SliderStyle sliderStyle,
                           Slider::TextEntryBoxPosition textBoxPosition,
                           double initialValue, Component* parent,
                           Slider::Listener* listener,
                           bool makeVisible, LookAndFeel *lookAndFeel)
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
                slider->setLookAndFeel (lookAndFeel);
                
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
    
    static void addTextButton (TextButton* button, const String& buttonText,
                               bool shouldBeOn, bool makeVisible, bool toggleOnClick,
                               Component* parent, Button::Listener* listener, LookAndFeel *lookAndFeel)
    {
        button->setToggleState (shouldBeOn, sendNotification);
        button->setButtonText (buttonText);
        button->setClickingTogglesState (toggleOnClick);
        button->setLookAndFeel (lookAndFeel);
        
        if (parent != nullptr)
        {
            parent->addAndMakeVisible (button);
        }
        
        if (listener != nullptr)
        {
           button->addListener (listener);
        }
        
        if (makeVisible)
        {
            parent->addAndMakeVisible (button);
        }
        else
        {
            parent->addChildComponent (button);
        }
    }
    
    static void setSliderEnabled (Slider* slider, Label* label, bool shouldBeEnabled)
    {
        if (shouldBeEnabled)
        {
            slider->setEnabled (true);
            label->setColour (Label::textColourId,
                              Colour::fromFloatRGBA (250, 250, 250, 0.95));
        }
        else
        {
            slider->setEnabled (false);
            label->setColour (Label::textColourId,
                              Colour::fromFloatRGBA (250, 250, 250, 0.5));
        }
    }
    
    static double tempoToHz (int tempoIndex, double bpm)
    {
        double one4Hz = bpm / 60;
        double hz;
        
        switch (tempoIndex)
        {
            case 4:
                hz = one4Hz * 2;
                break;
                
            case 5:
                hz = one4Hz;
                break;
                
            case 3:
                hz = one4Hz * 4;
                break;
                
            case 6:
                hz = one4Hz / 2;
                break;
                
            case 2:
                hz = one4Hz * 8;
                break;
                
            case 1:
                hz = one4Hz * 16;
                break;
                
            case 7:
                hz = one4Hz / 4;
                break;
                
            case 8:
                hz = one4Hz / 8;
                break;
                
            case 9:
                hz = one4Hz / 16;
                break;
                
            case 10:
                hz = one4Hz / 32;
                break;
                
            default:
                hz = 1;
                break;
        }
        return hz;
    }
    
    static int freqToTempoIndex (double freq, double bpm)
    {
        float a = freq / (bpm / 60);
        
        if (a <= 1 / 32)
            return 10;
        else if (a <= 1 / 16)
            return 9;
        else if (a <= 1 / 8)
            return 8;
        else if (a <= 1 / 4)
            return 7;
        else if (a <= 1 / 2)
            return 6;
        else if (a <= 1)
            return 5;
        else if (a <= 2)
            return 4;
        else if (a <= 4)
            return 3;
        else if (a <= 8)
            return 2;
        else if (a <= 16)
            return 1;
        
        return 1;
    }

};


class RandomLfo
{
public:
    RandomLfo()  : value (random.nextDouble()), counter(0)
    {
    }
    
    ~RandomLfo()
    {
    }
    
    void reset()
    {
        counter = 0;
    }

    double randomFrom0to1 (double numSamples,
                           double amplitude, double ampOffset,
                           double phaseOffsetFrom0to1)
    {
        counter++;
        
        if (counter >= numSamples / numValues)
        {
            value = random.nextDouble();
            counter = 0;
        }
        
        double correctAmplitude = std::min (1 - ampOffset, amplitude);
        
        return value * correctAmplitude + ampOffset;
    }
    
    double noiseFrom0to1 (double numSamples,
                           double amplitude, double ampOffset,
                           double phaseOffsetFrom0to1)
    {
        counter++;
        
        if (counter >= numSamples / 10000)
        {
            value = random.nextDouble();
            counter = 0;
        }
        
        double correctAmplitude = std::min (1 - ampOffset, amplitude);
        
        return value * correctAmplitude + ampOffset;
    }
    
private:
    Random random;
    double value;
    int counter;
    int numValues = 8;
};
