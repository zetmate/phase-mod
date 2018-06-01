/*
  ==============================================================================

    Ramp.h
    Created: 9 Jan 2018 9:14:24am
    Author:  Macbook

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Ramp
{
public:
    Ramp();
    ~Ramp();
    
    double getEndValue() const;
    
    void setSampleRate (double newSampleRate);
    void setRange (double start, double end);
    void setTime (double ms, double sampleRate);
    void updateInterval (double &currentValue);
    double applyRamp (double &value);
    
private:
    double startValue;
    double endValue;
    double currentValue;
    
    double interval;
    double time;
    double numSamples;
    
    double sampleRate;
};
