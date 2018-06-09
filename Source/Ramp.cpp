/*
  ==============================================================================

    Ramp.cpp
    Created: 9 Jan 2018 9:14:24am
    Author:  Macbook

  ==============================================================================
*/

#include "Ramp.h"


Ramp::Ramp()  : startValue(0),
                endValue(0),
                currentValue(0),
                interval(0),
                time(0),
                numSamples(0)
{
}

Ramp::~Ramp()
{
}

double Ramp::getEndValue() const
{
    return endValue;
}

double Ramp::getCurrentValue() const
{
    return currentValue;
}

void Ramp::setRange (double start, double end)
{
    startValue = start;
    endValue = end;
    interval = (end - start) / numSamples;
    currentValue = start;
}

void Ramp::updateInterval (double &currentValue)
{
    currentValue = endValue;
    interval = 0;
}

void Ramp::setTime (double ms, double sampleRate)
{
    if (roundToInt(time) != roundToInt(ms))
    {
        time = ms;
        numSamples = (sampleRate / 1000) * ms;
    }
}

double Ramp::applyRamp (double &value)
{
    if (interval > 0)
    {
        if (value < endValue)
            value += interval;
    }
    else
    {
        if (value > endValue)
            value += interval;
    }
    currentValue = value;
    
    return value;
}

