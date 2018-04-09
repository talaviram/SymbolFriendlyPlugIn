/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SymbolFriendlyPlugInAudioProcessor::SymbolFriendlyPlugInAudioProcessor()
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
}

SymbolFriendlyPlugInAudioProcessor::~SymbolFriendlyPlugInAudioProcessor()
{
}

//==============================================================================
const String SymbolFriendlyPlugInAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SymbolFriendlyPlugInAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SymbolFriendlyPlugInAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double SymbolFriendlyPlugInAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SymbolFriendlyPlugInAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SymbolFriendlyPlugInAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SymbolFriendlyPlugInAudioProcessor::setCurrentProgram (int index)
{
}

const String SymbolFriendlyPlugInAudioProcessor::getProgramName (int index)
{
    return {};
}

void SymbolFriendlyPlugInAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SymbolFriendlyPlugInAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SymbolFriendlyPlugInAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SymbolFriendlyPlugInAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SymbolFriendlyPlugInAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (juce::Random().nextBool() && mBetterChance > 1000)
    {
        juce::AudioPlayHead::CurrentPositionInfo result;
        getPlayHead()->getCurrentPosition(result);
        SomeStruct* someStruct = nullptr;
        if (someStruct->intVal > 10)
            DBG("processor no crash?");
    }
    mBetterChance++;
}

//==============================================================================
bool SymbolFriendlyPlugInAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SymbolFriendlyPlugInAudioProcessor::createEditor()
{
    return new SymbolFriendlyPlugInAudioProcessorEditor (*this);
}

//==============================================================================
void SymbolFriendlyPlugInAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SymbolFriendlyPlugInAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SymbolFriendlyPlugInAudioProcessor();
}
