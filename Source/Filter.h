/*
  ==============================================================================

    Filter.h
    Created: 1 Jan 2018 3:18:54pm
    Author:  Macbook

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Filter
{
public:
    Filter();
    virtual ~Filter();
    
    virtual void countCoefficients (double sampleRate) = 0;
//    virtual void countLpfCoefficients (double sampleRate, double cutOffFreq) = 0;
//    virtual void countHpfCoefficients (double sampleRate, double cutOffFreq) = 0;

    virtual float filterSignal (float input, int channel);
    
    friend class BandPassFilter;
    friend class BandStopFilter;
    friend class DevilFilter;
    
    void clearBuffers();
    
    double quality;
    double frequency;
    double bandwidth;
    double bandwidthIndex;
    
protected:
    float a0;
    float a1;
    float a2;
    float b1;
    float b2;
    
private:
    AudioSampleBuffer sampleA1;
    AudioSampleBuffer sampleA2;
    AudioSampleBuffer sampleB1;
    AudioSampleBuffer sampleB2;
    
    //read pointers
    const float* readA1L;
    const float* readA1R;
    const float* readA2L;
    const float* readA2R;
    const float* readB1L;
    const float* readB1R;
    const float* readB2L;
    const float* readB2R;
    
    //write pointers
    float* writeA1L;
    float* writeA1R;
    float* writeA2L;
    float* writeA2R;
    float* writeB1L;
    float* writeB1R;
    float* writeB2L;
    float* writeB2R;
};

struct LowPassFilter  : public Filter
{
    LowPassFilter() {};
    ~LowPassFilter() {};
    
    void countCoefficients (double sampleRate) override
    {
        double angle = 2 * double_Pi * frequency / sampleRate;
        double d = 1 / quality;
        double sinAngle = sin(angle);
        double beta = 0.5 * (1 - d/2 * sinAngle) / (1 + d/2 * sinAngle);
        double gamma = (0.5 + beta) * cos(angle);
        
        a0 = static_cast<float>((0.5 + beta - gamma) / 2);
        a1 = static_cast<float>(0.5 + beta - gamma);
        a2 = a0;
        b1 = static_cast<float>(-2 * gamma);
        b2 = static_cast<float>(2 * beta);
    }
};

struct HighPassFilter  : public Filter
{
    HighPassFilter() {};
    ~HighPassFilter() {};
    
    void countCoefficients (double sampleRate) override
    {
        double angle = 2 * double_Pi * frequency / sampleRate;
        double d = 1 / quality;
        double sinAngle = sin(angle);
        double beta = 0.5 * (1 - d/2 * sinAngle) / (1 + d/2 * sinAngle);
        double gamma = (0.5 + beta) * cos(angle);
        
        a0 = static_cast<float>((0.5 + beta + gamma) / 2);
        a1 = static_cast<float>(-(0.5 + beta + gamma));
        a2 = a0;
        b1 = static_cast<float>(-2 * gamma);
        b2 = static_cast<float>(2 * beta);
    }
};

//class BandFilter  : public Filter
//{
//public:
//    BandFilter() {};
//    virtual ~BandFilter() {};
//    
//protected:
//    LowPassFilter lpf;
//    HighPassFilter hpf;
//    double lowFrequency = frequency - bandWidth / 2;
//    double highFrequency = frequency + bandWidth / 2;
//};

class BandPassFilter  : public Filter
{
public:
    BandPassFilter() {};
    ~BandPassFilter() {};
    
    void countCoefficients (double sampleRate) override
    {
        bandwidth = frequency * bandwidthIndex;
        lowFrequency = frequency - bandwidth / 2;
        highFrequency = frequency + bandwidth / 2;
        
        if (lowFrequency < 60)
            lowFrequency = 60;
        
        if (highFrequency > 20000)
            highFrequency = 20000;
        
        lowPass.frequency = highFrequency;
        lowPass.quality = quality / 2;
        lowPass.countCoefficients (sampleRate);
        
        highPass.frequency = lowFrequency;
        highPass.quality = quality / 2;
        highPass.countCoefficients (sampleRate);
    }
    
    float filterSignal (float input, int channel) override
    {
        float output = 0.0f;
        float lowPassedSignal = lowPass.filterSignal(input, channel);
        float bandPassedSignal = highPass.filterSignal(lowPassedSignal, channel);
        output = bandPassedSignal;
        return output;
    }
    
private:
    LowPassFilter lowPass;
    HighPassFilter highPass;
    double lowFrequency;
    double highFrequency;

};

struct BandStopFilter  : public Filter
{
    BandStopFilter() {};
    ~BandStopFilter() {};
    
    void countCoefficients (double sampleRate) override
    {
        bandwidth = frequency * bandwidthIndex;
        lowFrequency = frequency - bandwidth / 2;
        highFrequency = frequency + bandwidth / 2;
        
        if (lowFrequency < 60)
            lowFrequency = 60;
        
        if (highFrequency > 16000)
            highFrequency = 16000;
        
        lowPass.frequency = lowFrequency;
        lowPass.quality = quality / 2;
        lowPass.countCoefficients (sampleRate);
        
        highPass.frequency = highFrequency;
        highPass.quality = quality / 2;
        highPass.countCoefficients (sampleRate);
    }
    
    float filterSignal (float input, int channel) override
    {
        float output = 0.0f;
        float lowPassedSignal = lowPass.filterSignal(input, channel);
        float highPassedSignal = highPass.filterSignal(input, channel);
        output = 0.5 * (lowPassedSignal + highPassedSignal);
        return output;
    }
    
private:
    LowPassFilter lowPass;
    HighPassFilter highPass;
    double lowFrequency;
    double highFrequency;
};

class DevilFilter  : public Filter
{
    void countCoefficients (double sampleRate) override
    {
        bandwidth = frequency * bandwidthIndex;
        lowFrequency = frequency - bandwidth / 2;
        highFrequency = frequency + bandwidth / 2;
        
        if (lowFrequency < 60)
            lowFrequency = 60;
        
        if (highFrequency > 16000)
            highFrequency = 16000;
        
        lowPass.frequency = highFrequency;
        lowPass.quality = quality / 2;
        lowPass.countCoefficients (sampleRate);
        
        highPass.frequency = lowFrequency;
        highPass.quality = quality / 2;
        highPass.countCoefficients (sampleRate);
    }
    
    float filterSignal (float input, int channel) override
    {
        float output = 0.0f;
        float lowPassedSignal = lowPass.filterSignal (input, channel);
        float highPassedSignal = highPass.filterSignal (input, channel);
        output = 0.5 * (lowPassedSignal + highPassedSignal);
        return output;
    }
    
private:
    LowPassFilter lowPass;
    HighPassFilter highPass;
    double lowFrequency;
    double highFrequency;
};

class AntiAliasingFilter  : public Filter
{
public:
    AntiAliasingFilter (int nummPoles = 8)
    : frequency (20000), numPoles (nummPoles)
    {
    }
    
    AntiAliasingFilter (float frequency, int nummPoles = 8)
        : frequency (frequency), numPoles (nummPoles)
    {
    }
    
    ~AntiAliasingFilter()
    {
    }
    
    void countCoefficients (double sampleRate) override
    {
        filter1.frequency = frequency;
        filter1.quality = 0.707 / (numPoles / 2);
        filter1.countCoefficients (sampleRate);
        
        filter2.frequency = frequency;
        filter2.quality = 0.707 / (numPoles / 2);
        filter2.countCoefficients (sampleRate);
        
        filter3.frequency = frequency;
        filter3.quality = 0.707 / (numPoles / 2);
        filter3.countCoefficients (sampleRate);
        
        filter4.frequency = frequency;
        filter4.quality = 0.707 / (numPoles / 2);
        filter4.countCoefficients (sampleRate);
    }
    
    float filterSignal (float input, int channel) override
    {
        float output = input;
        
        output = filter1.filterSignal (input, channel);
        output = filter2.filterSignal (input, channel);
        output = filter3.filterSignal (input, channel);
        output = filter4.filterSignal (input, channel);
        
        return output;
    }
    
    float frequency;
    
private:
    LowPassFilter filter1;
    LowPassFilter filter2;
    LowPassFilter filter3;
    LowPassFilter filter4;
    int numPoles;
};

