/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "Proc.h"


//==============================================================================
/**
*/
class Vibrato2AudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Vibrato2AudioProcessor();
    ~Vibrato2AudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    Proc proc;
    friend class EffectEditor;
    friend class ModEditor;
private:
    //some gui related stuff
    AudioProcessorValueTreeState treeState;
    UndoManager undoManager;
    
    //Ranges
    NormalisableRange<float>    depthRange,
                                delayRange,
                                feedbackRange,
                                fbLfoAmpRange,
                                procTypeRange,
                                fbTypeRange;
    
    NormalisableRange<float>    mixRange,
                                voice1MixRange,
                                voice2MixRange,
                                masterRange;
    
    NormalisableRange<float>    lfo1onRange,
                                lfo2onRange,
                                lfo3onRange,
                                lfo1FreqRange,
                                lfo2FreqRange,
                                lfo3FreqRange,
                                lfo1TFreqRange,
                                lfo2TFreqRange,
                                lfo3TFreqRange,
                                triplet1Range,
                                triplet2Range,
                                triplet3Range,
                                dotted1Range,
                                dotted2Range,
                                dotted3Range,
                                lfo1TypeRange,
                                lfo2TypeRange,
                                lfo3TypeRange,
                                tempoSync1Range,
                                tempoSync2Range,
                                tempoSync3Range,
                                sync2to1Range,
                                syncAllRange,
                                phase0Range,
                                phase90Range,
                                phase180Range;
    
    //MAIN PARAMETERS==========================================================
    String depthId = "depth", depthName = "Depth", depthLabelText = depthName;
    float depthDefault = 30;
    
    String delayId = "delay", delayName = "Delay", delayLabelText = delayName;
    float delayDefault = 60;
    
    String feedbackId = "feedback", feedbackName = "Feedback", feedbackLabelText = delayName;
    float feedbackDefault = -70;
    
    String fbLfoAmpId = "fbLfoAmp", fbLfoAmpName = "Feedback LFO Depth", fbLfoAmpLabelText = fbLfoAmpName;
    float fbLfoAmpDefault = 80;
    
    String procTypeId = "procType", procTypeName = "Processing Type", procTypeLabelText = procTypeName;
    float procTypeDefault = 1;
    
    String fbTypeId = "fbTypeId", fbTypeName = "Feedback Type", fbTypeLabelText = fbTypeName;
    float fbTypeDefault = 0;
    
    String mixId = "mix", mixName = "Mix", mixLabelText = mixName.toUpperCase();
    float mixDefault = 50;
    
    String voice1MixId = "voice1Mix", voice1MixName = "Voice 1 Mix", voice1MixLabelText = voice1MixName.toUpperCase();
    float voice1MixDefault = 100;
    
    String voice2MixId = "voice2Mix", voice2MixName = "Voice 2 Mix", voice2MixLabelText = voice2MixName.toUpperCase();
    float voice2MixDefault = 100;
    
    String masterId = "master", masterName = "Master", masterLabelText = masterName.toUpperCase();
    float masterDefault = 0;
    
    // MODULATION PARAMETERS
    //on / off buttons
    String lfo1onId = "lfo1on", lfo1onName = "LFO1 on/off", lfo1onLabelText = lfo1onName.toUpperCase();
    float lfo1onDefault = 1;
    
    String lfo2onId = "lfo2on", lfo2onName = "LFO2 on/off", lfo2onLabelText = lfo2onName.toUpperCase();
    float lfo2onDefault = 1;
    
    String lfo3onId = "lfo3on", lfo3onName = "LFO3 on/off", lfo3onLabelText = lfo3onName.toUpperCase();
    float lfo3onDefault = 0;
    
    //frequency
    // not synced
    String lfo1FreqId = "lfo1Freq", lfo1FreqName = "LFO1 frequency", lfo1FreqLabelText = lfo1FreqName.toUpperCase();
    float lfo1FreqDefault = 0.01;
    
    String lfo2FreqId = "lfo2Freq", lfo2FreqName = "LFO2 frequency", lfo2FreqLabelText = lfo2FreqName.toUpperCase();
    float lfo2FreqDefault = 0.01;
    
    String lfo3FreqId = "lfo3Freq", lfo3FreqName = "LFO3 frequency", lfo3FreqLabelText = lfo3FreqName.toUpperCase();
    float lfo3FreqDefault = 2;
    
    //tempo synced
    String lfo1TFreqId = "lfo1TFreq", lfo1TFreqName = "LFO1 tempo freq", lfo1TFreqLabelText = lfo1TFreqName.toUpperCase();
    float lfo1TFreqDefault = -9;
    
    String lfo2TFreqId = "lfo2TFreq", lfo2TFreqName = "LFO2 tempo freq", lfo2TFreqLabelText = lfo2TFreqName.toUpperCase();
    float lfo2TFreqDefault = -9;
    
    String lfo3TFreqId = "lfo3TFreq", lfo3TFreqName = "LFO3 tempo freq", lfo3TFreqLabelText = lfo3TFreqName.toUpperCase();
    float lfo3TFreqDefault = -4;
    
    //triplet and dotted buttons
    String triplet1Id = "triplet1", triplet1Name = "Triplet", triplet1LabelText = triplet1Name;
    float triplet1Default = 0;
    
    String triplet2Id = "triplet2", triplet2Name = "Triplet", triplet2LabelText = triplet2Name;
    float triplet2Default = 0;
    
    String triplet3Id = "triplet3", triplet3Name = "Triplet", triplet3LabelText = triplet3Name;
    float triplet3Default = 0;
    
    String dotted1Id = "dotted1", dotted1Name = "Dotted", dotted1LabelText = dotted1Name;
    float dotted1Default = 0;
    
    String dotted2Id = "dotted2", dotted2Name = "Dotted", dotted2LabelText = dotted2Name;
    float dotted2Default = 0;
    
    String dotted3Id = "dotted3", dotted3Name = "Dotted", dotted3LabelText = dotted3Name;
    float dotted3Default = 0;
    
    //LFO SHAPE COMBO BOXES
    String lfo1TypeId = "lfo1Type", lfo1TypeName = "LFO1 Shape", lfo1TypeLabelText = lfo1TypeName;
    float lfo1TypeDefault = 0;
    
    String lfo2TypeId = "lfo2Type", lfo2TypeName = "LFO2 Shape", lfo2TypeLabelText = lfo2TypeName;
    float lfo2TypeDefault = 0;
    
    String lfo3TypeId = "lfo3Type", lfo3TypeName = "LFO3 Shape", lfo3TypeLabelText = lfo3TypeName;
    float lfo3TypeDefault = 0;
    
    //TEMPO SYNC BUTTONS
    String tempoSync1Id = "tempoSync1", tempoSync1Name = "Tempo Sync", tempoSync1LabelText = tempoSync1Name;
    float tempoSync1Default = 0;
    
    String tempoSync2Id = "tempoSync2", tempoSync2Name = "Tempo Sync", tempoSync2LabelText = tempoSync2Name;
    float tempoSync2Default = 0;
    
    String tempoSync3Id = "tempoSync3", tempoSync3Name = "Tempo Sync", tempoSync3LabelText = tempoSync3Name;
    float tempoSync3Default = 0;
    
    //lfo sync buttons
    String sync2to1Id = "sync2to1", sync2to1Name = "sync 1&2", sync2to1LabelText = sync2to1Name;
    float sync2to1Default = 0;
    
    String syncAllId = "syncAll", syncAllName = "sync all", syncAllLabelText = syncAllName;
    float syncAllDefault = 0;
    
    String phase0Id = "phase0", phase0Name = "LFO2 phase shift 0", phase0LabelText = phase0Name;
    float phase0Default = 1;
    
    String phase90Id = "phase90", phase90Name = "LFO2 phase shift 90", phase90LabelText = phase90Name;
    float phase90Default = 0;
    
    String phase180Id = "phase180", phase180Name = "LFO2 phase shift 180", phase180LabelText = phase180Name;
    float phase180Default = 0;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Vibrato2AudioProcessor)
};
