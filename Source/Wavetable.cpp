/*
  ==============================================================================

    Wavetable.cpp
    Created: 10 Jan 2018 11:32:24am
    Author:  Macbook

  ==============================================================================
*/

#include "Wavetable.h"


//===============================================================================
//===============================================================================
Wavetable::Wavetable()
        : wavetable (new AudioBuffer<double>(0, 0)), currentSample(INFINITY), sampleRate(44100), wtSize(0), valueChanged(false)

{
}

Wavetable::~Wavetable()
{
}

bool Wavetable::setSampleRate (double newSampleRate)
{
    if (sampleRate != newSampleRate)
    {
        setSampleRateRelatedValues (newSampleRate);
        
        sampleRate = newSampleRate;
        return true;
    }
    else
    {
        return false;
    }
}

void Wavetable::setRange (NormalisableRange<float> const &rangeToUse)
{
    range = rangeToUse;
}

void Wavetable::resetWavetable()
{
    currentSample = 0;
    //countWavetable();
    
    if (valueChanged)
    {
        countWavetable();
        valueChanged = false;
    }
}

void Wavetable::valueHasChanged()
{
    valueChanged = true;
}

//===============================================================================
//===============================================================================

//constructors
Envelope::Envelope()
        : attackSamples(0), releaseSamples(0),
         attackStartLevel(0), attackEndLevel(0), releaseLevel(0),
         threshold(1), attackQuality(2), releaseQuality(2),
         attackTimeMs(0), releaseTimeMs(0), attackIsFat(true),
         releaseIsFat(true)
{
}

Envelope::~Envelope()
{
}

double Envelope::applyWavetable (float input, int channel)
{
    const double* wtData = wavetable->getReadPointer(channel);
    
    if (input >= threshold && currentSample >= Utility::max (wtSize, 250))
        resetWavetable();

    if (currentSample == wtSize)
        return getReleaseLevel();
    
    double value = range.convertFrom0to1 (wtData[currentSample]);
    currentSample++;
    
    return value;
}

void Envelope::countWavetable()
{
    //count and set wavetable size
    wtSize = attackSamples + releaseSamples;
    
    //get write pointer to wavetable
    double* wtW = wavetable->getWritePointer (0);
   
    //add attack curve
    if (attackStartLevel < attackEndLevel)
    {
        for (int sample = 0; sample < attackSamples; ++sample)
        {
            wtW[sample] = Utility::fatCurveUp (sample, attackStartLevel, attackEndLevel,
                                               2.1, attackSamples, attackSamples);
        }
    }
    else
    {
        for (int sample = 0; sample < attackSamples; ++sample)
        {
            wtW[sample] = Utility::fatCurveDown (sample, attackEndLevel, attackStartLevel,
                                                 2.2, attackSamples, 0);
        }
    }
    
    //add release curve
    if (attackEndLevel > releaseLevel)
    {
        for (int sample = attackSamples; sample < wtSize; ++sample)
        {
            wtW[sample] = Utility::fatCurveDown (sample, releaseLevel, attackEndLevel,
                                                 2.2, releaseSamples, attackSamples);
        }
    }
    else if (attackEndLevel < releaseLevel)
    {
        for (int sample = attackSamples; sample < wtSize; ++sample)
        {
            wtW[sample] = Utility::fatCurveUp (sample, attackEndLevel, releaseLevel,
                                               2.3, releaseSamples, releaseSamples);
        }
    }
    currentSample = 0;
}

//getters

double Envelope::getAttackTime() const
{
    return Utility::samplesToMs ((double) attackSamples, sampleRate);
}

double Envelope::getAttackStartLevel() const
{
    return range.convertFrom0to1 (attackStartLevel);
}

double Envelope::getAttackEndLevel() const
{
    return range.convertFrom0to1 (attackEndLevel);
}

double Envelope::getReleaseTime() const
{
    return Utility::samplesToMs ((double) releaseSamples, sampleRate);
}

double Envelope::getReleaseLevel() const
{
    return range.convertFrom0to1 (releaseLevel);
}

//setters

void Envelope::setAllParameters   (double attackTime, double releaseTime,
                                   double newAttackStartLevel,
                                   double newAttackEndLevel,
                                   double newReleaseLevel,
                                   double newAttackQuality,
                                   double newReleaseQuality,
                                   double newThreshold)
{
    setAttackTime (attackTime);
    setReleaseTime (releaseTime);
    setAttackStartLevel (newAttackStartLevel);
    setAttackEndLevel (newAttackEndLevel);
    setReleaseLevel (newReleaseLevel);
    setAttackQuality (newAttackQuality);
    setReleaseQuality (newReleaseQuality);
    setThreshold (newThreshold);
}

void Envelope::setAttackTime (double newAttackTime)
{
    attackSamples = Utility::msToSamples (newAttackTime, sampleRate);
    attackTimeMs = newAttackTime;
}

void Envelope::setReleaseTime (double newReleaseTime)
{
    releaseSamples = Utility::msToSamples (newReleaseTime, sampleRate);
    releaseTimeMs = newReleaseTime;
}

void Envelope::setAttackStartLevel (double newAttackStartLevel)
{
    attackStartLevel = range.convertTo0to1 (newAttackStartLevel);
    
    if (attackStartLevel > 1)
        attackStartLevel = 1;
    else if (attackStartLevel < 0)
        attackStartLevel = 0;
}

void Envelope::setAttackEndLevel (double newAttackEndLevel)
{
    attackEndLevel = range.convertTo0to1 (newAttackEndLevel);
    
    if (attackEndLevel > 1)
        attackEndLevel = 1;
    else if (attackEndLevel < 0)
        attackEndLevel = 0;
}

void Envelope::setReleaseLevel (double newReleaseLevel)
{
    releaseLevel = range.convertTo0to1 (newReleaseLevel);
    
    if (releaseLevel > 1)
        releaseLevel = 1;
    else if (releaseLevel < 0)
        releaseLevel = 0;
}

void Envelope::setAttackQuality (double newQuality)
{
    if (newQuality > 0)
        attackIsFat = true;
    else
        attackIsFat = false;
    
    attackQuality = newQuality;
}

void Envelope::setReleaseQuality (double newQuality)
{
    if (newQuality > 0)
        releaseIsFat = true;
    else
        releaseIsFat = false;
    
    releaseQuality = newQuality;
}

void Envelope::setThreshold (double newThresholdInDecibels)
{
    threshold = Utility::fromDb (newThresholdInDecibels);
}

void Envelope::setSampleRateRelatedValues (double newSampleRate)
{
    setAttackTime (attackTimeMs);
    setReleaseTime (releaseTimeMs);
}

//======================================================================================
//======================================================================================

Oscilator::Oscilator()  : frequency(1), amplitude(1), phaseOffsetLeft(0), phaseOffsetRight(0.5),
                          stereo(false)
{
}

Oscilator::~Oscilator()
{
}

//overides

double Oscilator::applyWavetable (float input, int channel)
{
    if (currentSample >= wtSize)
        resetWavetable();
    
    //get read pointer to wt
    const double* wtR = wavetable->getReadPointer (channel);
    double currentValue = range.convertFrom0to1 (wtR[currentSample]);
    
    if (!(stereo) || channel == 1)
        currentSample++;
    
    return currentValue;
}

void Oscilator::countWavetable()
{
    //set wavetable size
    wtSize = sampleRate / frequency;
    
    if (stereo)
        wavetable->setSize (2, wtSize);
    else
        wavetable->setSize (1, wtSize);
    
    wavetable->clear();
    
    //get write pointer to wavetable buffer
    double* wtW = wavetable->getWritePointer (0);
    
    //write
    for (int sample = 0; sample < wtSize; ++sample)
    {
        wtW[sample] = Utility::sinFrom0to1 (sample, wtSize, amplitude,
                                            ampOffsetLeft, phaseOffsetLeft,
                                            sampleRate);
    }
    
    if (stereo)
    {
        wtW = wavetable->getWritePointer (1);
        
        //write
        for (int sample = 0; sample < wtSize; ++sample)
        {
            wtW[sample] = Utility::sinFrom0to1 (sample, wtSize, amplitude,
                                                ampOffsetRight, phaseOffsetRight,
                                                sampleRate);
        }
    }
    
    currentSample = 0;
}

void Oscilator::setSampleRateRelatedValues (double newSampleRate)
{
}

//setters

void Oscilator::setStereoOrMono (bool isStereo, int channelOffsetDegr)
{
    stereo = isStereo;
    
    if (stereo)
        setChannelPhaseOffset (channelOffsetDegr);
    
}

void Oscilator::setChannelPhaseOffset (int channelOffsetDegr)
{
    if (channelOffsetDegr == 0)
    {
        phaseOffsetRight = phaseOffsetLeft;
    }
    else
    {
        float channelOffset = (float)channelOffsetDegr / (float)360;
        
        double sum = phaseOffsetLeft + channelOffset;
        
        if (sum <= 1)
            phaseOffsetRight = sum;
        else
            phaseOffsetRight = sum - 1;
    }
}

void Oscilator::setAllParameters (double newFrequency, double newAmplitude, double newOffset)
{
    setFrequency (newFrequency);
    setAmplitude (newAmplitude);
    setAmpOffset (newOffset);
}

void Oscilator::setFrequency (double newFrequency)
{
    frequency = newFrequency;
}

void Oscilator::setAmplitude (double newAmplitude)
{
    amplitude = newAmplitude;
}

void Oscilator::setAmpOffset (double newOffset)
{
    if (newOffset < 0)
    {
        ampOffsetLeft = 0;
        ampOffsetRight = 0;
    }
    else if (newOffset > 1)
    {
        ampOffsetLeft = 1;
        ampOffsetRight = 1;
    }
    else
    {
        ampOffsetLeft = newOffset;
        ampOffsetRight = newOffset;
    }
}

