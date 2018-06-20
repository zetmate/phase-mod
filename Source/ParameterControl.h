/*
  ==============================================================================

    ParameterControl.h
    Created: 20 Jun 2018 9:21:09pm
    Author:  Macbook

  ==============================================================================
*/

#pragma once
#include "Utility.h"
#include "Proc.h"

struct ParameterControl
{
    static void setDepth (Proc& proc, float parameterValue)
    {
        proc.setDepth (parameterValue / 100.0);
    }
};
