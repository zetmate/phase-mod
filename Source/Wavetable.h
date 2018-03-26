/*
  ==============================================================================

    Wavetable.h
    Created: 10 Jan 2018 11:32:24am
    Author:  Macbook

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Wavetable
{
public:
    Wavetable();
    virtual ~Wavetable();
    
    virtual void countWavetable() = 0;
    virtual double applyWavetable (float input) = 0;
    virtual void setSampleRateRelatedValues (double newSampleRate) = 0;
    
    bool setSampleRate (double newSampleRate);
    void setRange (NormalisableRange<float> const &rangeToUse);
    void resetWavetable();
    void valueHasChanged();
    
    AudioBuffer<double> wavetable;
    
protected:
    NormalisableRange<float> range;
    int currentSample;
    
    double sampleRate;
    int wtSize;
    
private:
    bool valueChanged;
};

//========================================================================
//========================================================================

//class Sine  : public Wavetable
//{
//    Sine();
//    ~Sine();
//    
//    void countWavetable (int numChannels) override;
//    double applyWavetable (int channel) override;
//    void setFrequency (double newFrequency);
//    
//private:
//    double frequency, interval;
//};

//========================================================================
class Envelope  : public Wavetable
{
public:
    Envelope();
    ~Envelope();
    
    void countWavetable() override;
    double applyWavetable (float input) override;
    void setSampleRateRelatedValues (double newSampleRate) override;
    
    double getAttackTime() const;
    double getAttackStartLevel() const;
    double getAttackEndLevel() const;
    double getReleaseTime() const;
    double getReleaseLevel() const;
    
    void setAllParameters (double attackTime, double releaseTime,
                           double newAttackStartLevel,
                           double newAttackEndLevel,
                           double newReleaseLevel,
                           double newAttackQuality,
                           double newReleaseQuality,
                           double newThreshold);
    
    void setAttackTime (double newAttackTime);
    void setAttackStartLevel (double newAttackStartLevel);
    void setAttackEndLevel (double newAttackEndLevel);
    void setReleaseTime (double newReleaseTime);
    void setReleaseLevel (double newReleaseLevel);
    
    void setThreshold (double newThresholdInDecibels);
    
    void setAttackQuality (double newQuality);
    void setReleaseQuality (double newQuality);
    
private:
    int attackSamples, releaseSamples;
    double attackStartLevel, attackEndLevel, releaseLevel;
    float threshold;
    
    double attackQuality;
    double releaseQuality;
    
    double attackTimeMs, releaseTimeMs, minTimeMs;
    
    bool attackIsFat;
    bool releaseIsFat;
    
};

class Oscilator  : public Wavetable
{
public:
    enum OscilatorWaveform
    {
        sin,
        saw,
        tri,
        square
    };
    
    Oscilator();
    ~Oscilator();
    
    void countWavetable() override;
    double applyWavetable (float input) override;
    void setSampleRateRelatedValues (double newSampleRate) override;
    
    void setAllParameters (double newFrequency, double newAmplitude, double newOffset);
    void setFrequency (double newFrequency);
    void setAmplitude (double newAmplitude);
    void setOffset (double newOffset);
    
private:
    double frequency;
    double amplitude;
    double offset;
};





