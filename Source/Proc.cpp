/*
  ==============================================================================

    Proc.cpp
    Created: 1 Jun 2018 3:16:28pm
    Author:  Macbook

  ==============================================================================
*/

#include "Proc.h"

//===========================================================================================
//                                      MONO MONO MONO
//===========================================================================================

void Proc::processBlockMonoSeparate (AudioSampleBuffer& buffer)
{
    //constants
    const int numSamples = buffer.getNumSamples();
    
    //get pointers to buffer
    const float* bufferR = buffer.getReadPointer(0);
    float* bufferW = buffer.getWritePointer (0);
    
    //============================================
    //  GET POINTERS TO DELAY BUFFERS
    //============================================
    
    //voice close
    const float* voiceClose_DelayR = voiceClose.getReadPointerToDelayBuffer (0);
    float* voiceClose_DelayW = voiceClose.getWritePointerToDelayBuffer (0);
    
    //voice mid
    const float* voiceMid_DelayR = voiceMid.getReadPointerToDelayBuffer (0);
    float* voiceMid_DelayW = voiceMid.getWritePointerToDelayBuffer (0);
    
    //voice far
    const float* voiceFar_DelayR = voiceFar.getReadPointerToDelayBuffer (0);
    float* voiceFar_DelayW = voiceFar.getWritePointerToDelayBuffer (0);
    
    //voice echo
    const float* voiceEcho_DelayR = voiceEcho.getReadPointerToDelayBuffer (0);
    float* voiceEcho_DelayW = voiceEcho.getWritePointerToDelayBuffer (0);
    
    
    //============================================
    //  PROCESS EACH SAMPLE
    //============================================
    
    for (int sample = 0; sample < numSamples; ++sample)
    {
        //get current values from Ramps
        inputGainRamp.applyRamp (inputGain);
        effectGainRamp.applyRamp (effectGain);
        gain1Ramp.applyRamp (gain1);
        gain2Ramp.applyRamp (gain2);
        
        //get input signal
        const float input = bufferR[sample] * inputGain;
        
        //============================================
        //  DECLARE VARIABLES
        //============================================
        
        //voices' outputs
        float voiceClose_output = 0.0;
        float voiceMid_output = 0.0;
        float voiceFar_output = 0.0;
        float voiceEcho_output = 0.0;
        
        //============================================
        //  PROCESS VOICES
        //============================================
        
        //voice Close
        voiceClose.processSampleMono (input, voiceClose_output,
                                      voiceClose_DelayR, voiceClose_DelayW);
        
        //voice Mid
        voiceMid.processSampleMono (input, voiceMid_output,
                                    voiceMid_DelayR, voiceMid_DelayW);
        
        //voice Far
        voiceFar.processSampleMono (input, voiceFar_output,
                                    voiceFar_DelayR, voiceFar_DelayW);
        
        //voice Echo
        voiceEcho.processSampleMono (input, voiceEcho_output,
                                     voiceEcho_DelayR, voiceEcho_DelayW);
        
        //============================================
        //  COMPUTE RESULTS
        //============================================
        float processed1 = voiceClose_output + voiceMid_output;
        float processed2 = voiceMid_output + voiceFar_output;
        
        float processed = (processed1 + processed2) * 0.5;
        
        //============================================
        //  LIMIT PROCESSED SIGNAL
        //============================================
        float output = 0.0;
        {
            float g = limiterLeft.getGainReductionValueForSample (processed);
            float limited = processed * g;
            g = limiter1Left.getGainReductionValueForSample (limited);
            output = (limited * g) * effectGain;
        }
        
        //============================================
        // COMPUTE DRY/WET
        //============================================
        
        //get current dry/wet propotion value
        dryWetRamp.applyRamp (dryWetPropotion);
        float wetGain = dryWetPropotion;
        float dryGain = 1 - wetGain;
        
        bufferW [sample] = output * wetGain + input * dryGain;
    }
}

void Proc::processBlockMonoCascade (AudioSampleBuffer& buffer)
{
    //constants
    const int numSamples = buffer.getNumSamples();
    
    //get pointers to buffer
    const float* bufferR = buffer.getReadPointer(0);
    float* bufferW = buffer.getWritePointer (0);
    
    //============================================
    //  GET POINTERS TO DELAY BUFFERS
    //============================================
    
    //voice close
    const float* voiceClose_DelayR = voiceClose.getReadPointerToDelayBuffer (0);
    float* voiceClose_DelayW = voiceClose.getWritePointerToDelayBuffer (0);
    
    //voice mid
    const float* voiceMid_DelayR = voiceMid.getReadPointerToDelayBuffer (0);
    float* voiceMid_DelayW = voiceMid.getWritePointerToDelayBuffer (0);
    
    //voice far
    const float* voiceFar_DelayR = voiceFar.getReadPointerToDelayBuffer (0);
    float* voiceFar_DelayW = voiceFar.getWritePointerToDelayBuffer (0);
    
    //voice echo
    const float* voiceEcho_DelayR = voiceEcho.getReadPointerToDelayBuffer (0);
    float* voiceEcho_DelayW = voiceEcho.getWritePointerToDelayBuffer (0);
    
    
    //============================================
    //  PROCESS EACH SAMPLE
    //============================================
    
    for (int sample = 0; sample < numSamples; ++sample)
    {
        //get current values from Ramps
        inputGainRamp.applyRamp (inputGain);
        effectGainRamp.applyRamp (effectGain);
        gain1Ramp.applyRamp (gain1);
        gain2Ramp.applyRamp (gain2);
        
        //get input signal
        const float input = bufferR[sample] * inputGain;
        
        //============================================
        //  DECLARE VARIABLES
        //============================================
        
        //voices' outputs
        float voiceClose_output = 0.0;
        float voiceMid_output = 0.0;
        float voiceFar_output = 0.0;
        float voiceEcho_output = 0.0;
        
        //============================================
        //  PROCESS VOICES
        //============================================
        
        //voice Close
        voiceClose.processSampleMono (input, voiceClose_output,
                                      voiceClose_DelayR, voiceClose_DelayW);
        
        //voice Mid
        voiceMid.processSampleMono (voiceClose_output, voiceMid_output,
                                    voiceMid_DelayR, voiceMid_DelayW);
        
        //voice Far
        voiceFar.processSampleMono (voiceMid_output, voiceFar_output,
                                    voiceFar_DelayR, voiceFar_DelayW);
        
        //voice Echo
        voiceEcho.processSampleMono (voiceFar_output, voiceEcho_output,
                                     voiceEcho_DelayR, voiceEcho_DelayW);
        
        //============================================
        //  COMPUTE DRY & WET GAINS
        //============================================
        
        //get current dry/wet propotion value
        dryWetRamp.applyRamp (dryWetPropotion);
        float wetGain = dryWetPropotion;
        float dryGain = 1 - wetGain;
        
        //============================================
        //  LIMIT PROCESSED SIGNAL
        //============================================
        
        float g = limiterLeft.getGainReductionValueForSample (voiceEcho_output);

        //output signal
        float limited = g * voiceEcho_output;

        g = limiter1Left.getGainReductionValueForSample (limited);
        
        float output = (g * limited) * effectGain;
        
        bufferW [sample] = output * wetGain + input * dryGain;
    }
}

//===========================================================================================
//                                  STEREO STEREO STEREO
//===========================================================================================

void Proc::processBlockStereoSeparate (AudioSampleBuffer& buffer)
{
    //constants
    const int numSamples = buffer.getNumSamples();
    
    //get pointers to buffer
    const float* leftBufferR = buffer.getReadPointer(0);
    const float* rightBufferR = buffer.getReadPointer(1);
    float* leftBufferW = buffer.getWritePointer (0);
    float* rightBufferW = buffer.getWritePointer (1);
    
    //============================================
    //  GET POINTERS TO DELAY BUFFERS
    //============================================
    
    //voice close
    const float* voiceClose_LeftDelayR = voiceClose.getReadPointerToDelayBuffer (0);
    const float* voiceClose_RightDelayR = voiceClose.getReadPointerToDelayBuffer (1);
    float* voiceClose_LeftDelayW = voiceClose.getWritePointerToDelayBuffer (0);
    float* voiceClose_RightDelayW = voiceClose.getWritePointerToDelayBuffer (1);
    
    //voice mid
    const float* voiceMid_LeftDelayR = voiceMid.getReadPointerToDelayBuffer (0);
    const float* voiceMid_RightDelayR = voiceMid.getReadPointerToDelayBuffer (1);
    float* voiceMid_LeftDelayW = voiceMid.getWritePointerToDelayBuffer (0);
    float* voiceMid_RightDelayW = voiceMid.getWritePointerToDelayBuffer (1);
    
    //voice far
    const float* voiceFar_LeftDelayR = voiceFar.getReadPointerToDelayBuffer (0);
    const float* voiceFar_RightDelayR = voiceFar.getReadPointerToDelayBuffer (1);
    float* voiceFar_LeftDelayW = voiceFar.getWritePointerToDelayBuffer (0);
    float* voiceFar_RightDelayW = voiceFar.getWritePointerToDelayBuffer (1);
    
    //voice echo
    const float* voiceEcho_LeftDelayR = voiceEcho.getReadPointerToDelayBuffer (0);
    const float* voiceEcho_RightDelayR = voiceEcho.getReadPointerToDelayBuffer (1);
    float* voiceEcho_LeftDelayW = voiceEcho.getWritePointerToDelayBuffer (0);
    float* voiceEcho_RightDelayW = voiceEcho.getWritePointerToDelayBuffer (1);
    
    
    //============================================
    //  PROCESS EACH SAMPLE
    //============================================
    
    for (int sample = 0; sample < numSamples; ++sample)
    {
        //get current values from Ramps
        inputGainRamp.applyRamp (inputGain);
        effectGainRamp.applyRamp (effectGain);
        gain1Ramp.applyRamp (gain1);
        gain2Ramp.applyRamp (gain2);
        
        //get input signal
        const float inputLeft = leftBufferR[sample] * inputGain;
        float inputRight;
        
        if (channelSet == stereo)
            inputRight = rightBufferR[sample] * inputGain;
        else
            inputRight = inputLeft;
        
        //============================================
        //  DECLARE VARIABLES
        //============================================
        
        //voices' outputs
        float voiceClose_outputLeft = 0.0;
        float voiceClose_outputRight = 0.0;
        //
        float voiceMid_outputLeft = 0.0;
        float voiceMid_outputRight = 0.0;
        //
        float voiceFar_outputLeft = 0.0;
        float voiceFar_outputRight = 0.0;
        //
        float voiceEcho_outputLeft = 0.0;
        float voiceEcho_outputRight = 0.0;
        
        
        //============================================
        //  PROCESS VOICES
        //============================================
        
        //voice Close
        voiceClose.processSampleStereo (inputLeft, inputRight,
                                        voiceClose_outputLeft, voiceClose_outputRight,
                                        voiceClose_LeftDelayR, voiceClose_RightDelayR,
                                        voiceClose_LeftDelayW, voiceClose_RightDelayW);
        
        //voice Mid
        voiceMid.processSampleStereo (inputLeft, inputRight,
                                      voiceMid_outputLeft, voiceMid_outputRight,
                                      voiceMid_LeftDelayR, voiceMid_RightDelayR,
                                      voiceMid_LeftDelayW, voiceMid_RightDelayW);
        
        //voice Far
        voiceFar.processSampleStereo (inputLeft, inputRight,
                                      voiceFar_outputLeft, voiceFar_outputRight,
                                      voiceFar_LeftDelayR, voiceFar_RightDelayR,
                                      voiceFar_LeftDelayW, voiceFar_RightDelayW);
        
        //voice Echo
        voiceEcho.processSampleStereo (inputLeft, inputRight,
                                       voiceEcho_outputLeft, voiceEcho_outputRight,
                                       voiceEcho_LeftDelayR, voiceEcho_RightDelayR,
                                       voiceEcho_LeftDelayW, voiceEcho_RightDelayW);
        
        //============================================
        //  COMPUTE RESULTS
        //============================================
        
        float processedLeft = 0.0;
        float processedRight = 0.0;
        {
            float voiceClose = voiceClose_outputLeft + voiceClose_outputRight;
            float voiceMid = voiceMid_outputLeft + voiceMid_outputRight;
            float voiceFar = voiceFar_outputLeft + voiceFar_outputRight;
            float voiceEcho = voiceEcho_outputLeft + voiceEcho_outputRight;
            
            processedLeft = (voiceClose * gain1 + voiceFar * gain2) * 0.5;
            processedRight = (voiceMid * gain1 + voiceEcho * gain2) * 0.5;
        }
        
        //============================================
        //  LIMIT PROCESSED SIGNAL
        //============================================
        float outputLeft = 0.0;
        float outputRight = 0.0;
        {
            float gLeft = limiterLeft.getGainReductionValueForSample (processedLeft);
            float gRight = limiterRight.getGainReductionValueForSample (processedRight);
            
            float limitedLeft = processedLeft * gLeft;
            float limitedRight = processedRight * gRight;
            
            gLeft = limiter1Left.getGainReductionValueForSample (limitedLeft);
            gRight = limiter1Right.getGainReductionValueForSample (limitedRight);
            
            outputLeft = (limitedLeft * gLeft) * effectGain;
            outputRight = (limitedRight * gRight) * effectGain;
        }
        
        //============================================
        // COMPUTE DRY/WET
        //============================================
        
        //get current dry/wet propotion value
        dryWetRamp.applyRamp (dryWetPropotion);
        float wetGain = dryWetPropotion;
        float dryGain = 1 - wetGain;
        
        leftBufferW [sample] = outputLeft * wetGain + inputLeft * dryGain;
        rightBufferW [sample] = outputRight * wetGain + inputRight * dryGain;
    }
}

//=====================================================================================
void Proc::processBlockStereoCascade (AudioSampleBuffer& buffer)
{
    //constants
    const int numSamples = buffer.getNumSamples();
    
    //get pointers to buffer
    const float* leftBufferR = buffer.getReadPointer(0);
    const float* rightBufferR = buffer.getReadPointer(1);
    float* leftBufferW = buffer.getWritePointer (0);
    float* rightBufferW = buffer.getWritePointer (1);
    
    //============================================
    //  GET POINTERS TO DELAY BUFFERS
    //============================================
    
    //voice close
    const float* voiceClose_LeftDelayR = voiceClose.getReadPointerToDelayBuffer (0);
    const float* voiceClose_RightDelayR = voiceClose.getReadPointerToDelayBuffer (1);
    float* voiceClose_LeftDelayW = voiceClose.getWritePointerToDelayBuffer (0);
    float* voiceClose_RightDelayW = voiceClose.getWritePointerToDelayBuffer (1);
    
    //voice mid
    const float* voiceMid_LeftDelayR = voiceMid.getReadPointerToDelayBuffer (0);
    const float* voiceMid_RightDelayR = voiceMid.getReadPointerToDelayBuffer (1);
    float* voiceMid_LeftDelayW = voiceMid.getWritePointerToDelayBuffer (0);
    float* voiceMid_RightDelayW = voiceMid.getWritePointerToDelayBuffer (1);
    
    //voice far
    const float* voiceFar_LeftDelayR = voiceFar.getReadPointerToDelayBuffer (0);
    const float* voiceFar_RightDelayR = voiceFar.getReadPointerToDelayBuffer (1);
    float* voiceFar_LeftDelayW = voiceFar.getWritePointerToDelayBuffer (0);
    float* voiceFar_RightDelayW = voiceFar.getWritePointerToDelayBuffer (1);
    
    //voice echo
    const float* voiceEcho_LeftDelayR = voiceEcho.getReadPointerToDelayBuffer (0);
    const float* voiceEcho_RightDelayR = voiceEcho.getReadPointerToDelayBuffer (1);
    float* voiceEcho_LeftDelayW = voiceEcho.getWritePointerToDelayBuffer (0);
    float* voiceEcho_RightDelayW = voiceEcho.getWritePointerToDelayBuffer (1);
    
    
    //============================================
    //  PROCESS EACH SAMPLE
    //============================================
    
    for (int sample = 0; sample < numSamples; ++sample)
    {
        //get current values from Ramps
        inputGainRamp.applyRamp (inputGain);
        effectGainRamp.applyRamp (effectGain);
        gain1Ramp.applyRamp (gain1);
        gain2Ramp.applyRamp (gain2);
        
        //get input signal
        const float inputLeft = leftBufferR[sample] * inputGain;
        const float inputRight = rightBufferR[sample] * inputGain;
        
        //============================================
        //  DECLARE VARIABLES
        //============================================
        
        //voices' outputs
        float voiceClose_outputLeft = 0.0;
        float voiceClose_outputRight = 0.0;
        //
        float voiceMid_outputLeft = 0.0;
        float voiceMid_outputRight = 0.0;
        //
        float voiceFar_outputLeft = 0.0;
        float voiceFar_outputRight = 0.0;
        //
        float voiceEcho_outputLeft = 0.0;
        float voiceEcho_outputRight = 0.0;
        
        //============================================
        //  PROCESS VOICES
        //============================================
        
        //voice Close
        voiceClose.processSampleStereo (inputLeft, inputRight,
                                        voiceClose_outputLeft, voiceClose_outputRight,
                                        voiceClose_LeftDelayR, voiceClose_RightDelayR,
                                        voiceClose_LeftDelayW, voiceClose_RightDelayW);
        
        //voice Mid
        voiceMid.processSampleStereo (voiceClose_outputLeft, voiceClose_outputRight,
                                      voiceMid_outputLeft, voiceMid_outputRight,
                                      voiceMid_LeftDelayR, voiceMid_RightDelayR,
                                      voiceMid_LeftDelayW, voiceMid_RightDelayW);
        
        //voice Far
        voiceFar.processSampleStereo (voiceMid_outputLeft, voiceMid_outputRight,
                                      voiceFar_outputLeft, voiceFar_outputRight,
                                      voiceFar_LeftDelayR, voiceFar_RightDelayR,
                                      voiceFar_LeftDelayW, voiceFar_RightDelayW);
        
        //voice Echo
        voiceEcho.processSampleStereo (voiceFar_outputLeft, voiceFar_outputRight,
                                       voiceEcho_outputLeft, voiceEcho_outputRight,
                                       voiceEcho_LeftDelayR, voiceEcho_RightDelayR,
                                       voiceEcho_LeftDelayW, voiceEcho_RightDelayW);
        
        //============================================
        //  COMPUTE DRY & WET GAINS
        //============================================
        
        //get current dry/wet propotion value
        dryWetRamp.applyRamp (dryWetPropotion);
        float wetGain = dryWetPropotion;
        float dryGain = 1 - wetGain;
        
        //============================================
        //  LIMIT PROCESSED SIGNAL
        //============================================
        
        float g = limiterLeft.getGainReductionValueForSample ((voiceEcho_outputLeft
                                                               + voiceEcho_outputRight)
                                                              * 0.5);
        //output signal
        float limitedLeft = g * voiceEcho_outputLeft;
        float limitedRight = g * voiceEcho_outputRight;
        
        g = limiter1Left.getGainReductionValueForSample ((limitedLeft + limitedRight) * 0.5);
        
        float outputLeft = (g * limitedLeft) * effectGain;
        float outputRight = (g * limitedRight) * effectGain;
        
        leftBufferW [sample] = outputLeft * wetGain + inputLeft * dryGain;
        rightBufferW [sample] = outputRight * wetGain + inputRight * dryGain;
    }
}
