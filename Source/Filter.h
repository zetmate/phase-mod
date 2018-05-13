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

    virtual float filterSignal (float input, int channel);
    
    friend class BandPassFilter;
    friend class BandStopFilter;
    friend class DevilFilter;
    
    virtual void clearBuffers();
    
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

class AntiAliasingFilter16  : public Filter
{
public:
    AntiAliasingFilter16()
    {
    }
    
    ~AntiAliasingFilter16()
    {
    }
    
    void countCoefficients (double sampleRate) override
    {
        filter1.frequency = frequency;
        filter1.quality = 0.707 / 8.0f;
        filter1.countCoefficients (sampleRate);
        
        filter2.frequency = frequency;
        filter2.quality = 0.707 / 8.0f;
        filter2.countCoefficients (sampleRate);
        
        filter3.frequency = frequency;
        filter3.quality = 0.707 / 8.0f;
        filter3.countCoefficients (sampleRate);
        
        filter4.frequency = frequency;
        filter4.quality = 0.707 / 8.0f;
        filter4.countCoefficients (sampleRate);
        
        filter5.frequency = frequency;
        filter5.quality = 0.707 / 8.0f;
        filter5.countCoefficients (sampleRate);
        
        filter6.frequency = frequency;
        filter6.quality = 0.707 / 8.0f;
        filter6.countCoefficients (sampleRate);
        
        filter7.frequency = frequency;
        filter7.quality = 0.707 / 8.0f;
        filter7.countCoefficients (sampleRate);
        
        filter8.frequency = frequency;
        filter8.quality = 0.707 / 8.0f;
        filter8.countCoefficients (sampleRate);
    }
    
    float filterSignal (float input, int channel) override
    {
        float output = input;
        
        output = filter1.filterSignal (input, channel);
        output = filter2.filterSignal (output, channel);
        output = filter3.filterSignal (output, channel);
        output = filter4.filterSignal (output, channel);
        output = filter5.filterSignal (output, channel);
        output = filter6.filterSignal (output, channel);
        output = filter7.filterSignal (output, channel);
        output = filter8.filterSignal (output, channel);
        
        return output;
    }
    
    void clearBuffers() override
    {
        filter1.clearBuffers();
        filter2.clearBuffers();
        filter3.clearBuffers();
        filter4.clearBuffers();
        filter5.clearBuffers();
        filter6.clearBuffers();
        filter7.clearBuffers();
        filter8.clearBuffers();
    }
    
    float frequency;
    
private:
    LowPassFilter filter1;
    LowPassFilter filter2;
    LowPassFilter filter3;
    LowPassFilter filter4;
    LowPassFilter filter5;
    LowPassFilter filter6;
    LowPassFilter filter7;
    LowPassFilter filter8;
};

class AntiAliasingFilter32  : public Filter
{
public:
    AntiAliasingFilter32()
    {
    }
    
    ~AntiAliasingFilter32()
    {
    }
    
    void countCoefficients (double sampleRate) override
    {
        filter1.frequency = frequency;
        filter1.quality = 0.707 / 2.0f;
        filter1.countCoefficients (sampleRate);
        
        filter2.frequency = frequency;
        filter2.quality = 0.707 / 2.0f;
        filter2.countCoefficients (sampleRate);
    }
    
    float filterSignal (float input, int channel) override
    {
        float output = input;
        
        output = filter1.filterSignal (output, channel);
        output = filter2.filterSignal (output, channel);
        
        return output;
    }
    
    void clearBuffers() override
    {
        filter1.clearBuffers();
        filter2.clearBuffers();
    }
    
    float frequency;
private:
    AntiAliasingFilter16 filter1;
    AntiAliasingFilter16 filter2;
};
