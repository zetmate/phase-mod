/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Vibrato2AudioProcessor::Vibrato2AudioProcessor()  : parameters (*this, &undoManager),
                                                    depthRange (0, 100, 1, 1),
                                                    delayRange (2, 500, 0.1, 1),
                                                    feedbackRange (-99, 99, 1, 1),
                                                    fbLfoAmpRange (15, 99, 1, 1),
                                                    procTypeRange (0, 1, 1, 1),
                                                    fbTypeRange (0, 1, 1, 1),
                                                    mixRange (0, 100, 1, 1),
                                                    voice1MixRange (0, 200, 1, 1),
                                                    voice2MixRange (0, 200, 1, 1),
                                                    masterRange (-24, 16, 0.1, 1),
                                                    lfo1onRange (0, 1, 1, 1),
                                                    lfo2onRange (0, 1, 1, 1),
                                                    lfo3onRange (0, 1, 1, 1),
                                                    lfo1FreqRange (0.01, 17.00, 0.01, 1),
                                                    lfo2FreqRange (0.01, 17.00, 0.01, 1),
                                                    lfo3FreqRange (0.01, 17.00, 0.01, 1),
                                                    lfo1TFreqRange (-10, -1, 1, 1),
                                                    lfo2TFreqRange (-10, -1, 1, 1),
                                                    lfo3TFreqRange (-10, -1, 1, 1),
                                                    triplet1Range (0, 1, 1, 1),
                                                    triplet2Range (0, 1, 1, 1),
                                                    triplet3Range (0, 1, 1, 1),
                                                    dotted1Range (0, 1, 1, 1),
                                                    dotted2Range (0, 1, 1, 1),
                                                    dotted3Range (0, 1, 1, 1),
                                                    lfo1TypeRange (0, 5, 1, 1),
                                                    lfo2TypeRange (0, 5, 1, 1),
                                                    lfo3TypeRange (0, 4, 1, 1),
                                                    tempoSync1Range (0, 1, 1, 1),
                                                    tempoSync2Range (0, 1, 1, 1),
                                                    tempoSync3Range (0, 1, 1, 1),
                                                    sync2to1Range (0, 1, 1, 1),
                                                    syncAllRange (0, 1, 1, 1),
                                                    phase0Range (0, 1, 1, 1),
                                                    phase90Range (0, 1, 1, 1),
                                                    phase180Range (0, 1, 1, 1)
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )

#endif
{
    depthRange.setSkewForCentre (50);
    delayRange.setSkewForCentre (40);
    feedbackRange.setSkewForCentre (0);
    fbLfoAmpRange.setSkewForCentre (50);
    
    mixRange.setSkewForCentre (50);
    voice1MixRange.setSkewForCentre (100);
    voice2MixRange.setSkewForCentre (100);
    masterRange.setSkewForCentre (0);
    
    lfo1FreqRange.setSkewForCentre (3);
    lfo2FreqRange.setSkewForCentre (3);
    lfo3FreqRange.setSkewForCentre (3);
    lfo1TFreqRange.setSkewForCentre (-5);
    lfo2TFreqRange.setSkewForCentre (-5);
    lfo3TFreqRange.setSkewForCentre (-5);
    
    parameters.createAndAddParameter (depthId, depthName, depthLabelText, depthRange,
                                     depthDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (delayId, delayName, delayLabelText, delayRange,
                                     delayDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (feedbackId, feedbackName, feedbackLabelText, feedbackRange,
                                     feedbackDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (fbLfoAmpId, fbLfoAmpName, fbLfoAmpLabelText, fbLfoAmpRange,
                                     fbLfoAmpDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (procTypeId, procTypeName, procTypeLabelText, procTypeRange,
                                     procTypeDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (fbTypeId, fbTypeName, fbTypeLabelText, fbTypeRange,
                                     fbTypeDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (mixId, mixName, mixLabelText, mixRange,
                                     mixDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (voice1MixId, voice1MixName, voice1MixLabelText, voice1MixRange,
                                     voice1MixDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (voice2MixId, voice2MixName, voice2MixLabelText, voice2MixRange,
                                     voice2MixDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (masterId, masterName, masterLabelText, masterRange,
                                     masterDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (lfo1onId, lfo1onName, lfo1onLabelText, lfo1onRange,
                                     lfo1onDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (lfo2onId, lfo2onName, lfo2onLabelText, lfo2onRange,
                                     lfo2onDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (lfo3onId, lfo3onName, lfo3onLabelText, lfo3onRange,
                                     lfo3onDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (lfo1FreqId, lfo1FreqName, lfo1FreqLabelText, lfo1FreqRange,
                                     lfo1FreqDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (lfo2FreqId, lfo2FreqName, lfo2FreqLabelText, lfo2FreqRange,
                                     lfo2FreqDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (lfo3FreqId, lfo3FreqName, lfo3FreqLabelText, lfo3FreqRange,
                                     lfo3FreqDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (lfo1TFreqId, lfo1TFreqName, lfo1TFreqLabelText, lfo1TFreqRange,
                                     lfo1TFreqDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (lfo2TFreqId, lfo2TFreqName, lfo2TFreqLabelText, lfo2TFreqRange,
                                     lfo2TFreqDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (lfo3TFreqId, lfo3TFreqName, lfo3TFreqLabelText, lfo3TFreqRange,
                                     lfo3TFreqDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (triplet1Id, triplet1Name, triplet1LabelText, triplet1Range,
                                     triplet1Default, nullptr, nullptr);
    
    parameters.createAndAddParameter (triplet2Id, triplet2Name, triplet2LabelText, triplet2Range,
                                     triplet2Default, nullptr, nullptr);
    
    parameters.createAndAddParameter (triplet3Id, triplet3Name, triplet3LabelText, triplet3Range,
                                     triplet3Default, nullptr, nullptr);
    
    parameters.createAndAddParameter (dotted1Id, dotted1Name, dotted1LabelText, dotted1Range,
                                     dotted1Default, nullptr, nullptr);
    
    parameters.createAndAddParameter (dotted2Id, dotted2Name, dotted2LabelText, dotted2Range,
                                     dotted2Default, nullptr, nullptr);
    
    parameters.createAndAddParameter (dotted3Id, dotted3Name, dotted3LabelText, dotted3Range,
                                     dotted3Default, nullptr, nullptr);
    
    parameters.createAndAddParameter (lfo1TypeId, lfo1TypeName, lfo1TypeLabelText, lfo1TypeRange,
                                     lfo1TypeDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (lfo2TypeId, lfo2TypeName, lfo2TypeLabelText, lfo2TypeRange,
                                     lfo2TypeDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (lfo3TypeId, lfo3TypeName, lfo3TypeLabelText, lfo3TypeRange,
                                     lfo3TypeDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (tempoSync1Id, tempoSync1Name, tempoSync1LabelText, tempoSync1Range,
                                     tempoSync1Default, nullptr, nullptr);
    
    parameters.createAndAddParameter (tempoSync2Id, tempoSync2Name, tempoSync2LabelText, tempoSync2Range,
                                     tempoSync2Default, nullptr, nullptr);
    
    parameters.createAndAddParameter (tempoSync3Id, tempoSync3Name, tempoSync3LabelText, tempoSync3Range,
                                     tempoSync3Default, nullptr, nullptr);
    
    parameters.createAndAddParameter (sync2to1Id, sync2to1Name, sync2to1LabelText, sync2to1Range,
                                     sync2to1Default, nullptr, nullptr);
    
    parameters.createAndAddParameter (syncAllId, syncAllName, syncAllLabelText, syncAllRange,
                                     syncAllDefault, nullptr, nullptr);
    
    parameters.createAndAddParameter (phase0Id, phase0Name, phase0LabelText, phase0Range,
                                     phase0Default, nullptr, nullptr);
    
    parameters.createAndAddParameter (phase90Id, phase90Name, phase90LabelText, phase90Range,
                                     phase90Default, nullptr, nullptr);
    
    parameters.createAndAddParameter (phase180Id, phase180Name, phase180LabelText, phase180Range,
                                     phase180Default, nullptr, nullptr);
    
    parameters.state = ValueTree (Identifier ("Whalor"));
    
    feedbackParameter = parameters.getRawParameterValue (feedbackId);
    parameters.addParameterListener (feedbackId, this);
}

Vibrato2AudioProcessor::~Vibrato2AudioProcessor()
{
}

//==============================================================================
const String Vibrato2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Vibrato2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Vibrato2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Vibrato2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Vibrato2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Vibrato2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Vibrato2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Vibrato2AudioProcessor::setCurrentProgram (int index)
{
}

const String Vibrato2AudioProcessor::getProgramName (int index)
{
    return {};
}

void Vibrato2AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Vibrato2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    if (getTotalNumOutputChannels() == 2)
        proc.setToStereo();
    else
        proc.setToMono();
    
    proc.prepare (sampleRate, samplesPerBlock);
    //proc.setFeedbackGain (*feedbackParameter);
}

void Vibrato2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Vibrato2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainInputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainInputChannelSet() != AudioChannelSet::stereo())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Vibrato2AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    proc.processBlock (buffer, getPlayHead());
}

//==============================================================================
bool Vibrato2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Vibrato2AudioProcessor::createEditor()
{
    return new Vibrato2AudioProcessorEditor (*this);
}

//==============================================================================
void Vibrato2AudioProcessor::parameterChanged (const String& parameterID, float newValue)
{
    if (getActiveEditor() == nullptr)
    {
        if (parameterID == feedbackId)
            proc.setFeedbackGain (newValue / 100.0);
    }
}

void Vibrato2AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void Vibrato2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Vibrato2AudioProcessor();
}
