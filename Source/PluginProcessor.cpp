/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
IRLoaderAudioProcessor::IRLoaderAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    mFormatManager.registerBasicFormats();
}

IRLoaderAudioProcessor::~IRLoaderAudioProcessor()
{
}

//==============================================================================
const juce::String IRLoaderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool IRLoaderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool IRLoaderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool IRLoaderAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double IRLoaderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int IRLoaderAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int IRLoaderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void IRLoaderAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String IRLoaderAudioProcessor::getProgramName (int index)
{
    return {};
}

void IRLoaderAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void IRLoaderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    auto spec = juce::dsp::ProcessSpec();
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    spec.maximumBlockSize = samplesPerBlock;
    
    convolver.prepare(spec);
}

void IRLoaderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool IRLoaderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void IRLoaderAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> audioBlock = juce::dsp::AudioBlock<float>(buffer);
    convolver.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

//==============================================================================
bool IRLoaderAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* IRLoaderAudioProcessor::createEditor()
{
    return new IRLoaderAudioProcessorEditor (*this);
}

//==============================================================================
void IRLoaderAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void IRLoaderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::String IRLoaderAudioProcessor::loadFile()
{
    juce::FileChooser chooser { "Please load an impulse response" };

    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();

        convolver.loadImpulseResponse(
            file,
            juce::dsp::Convolution::Stereo::no,
            juce::dsp::Convolution::Trim::no,
            file.getSize()
        );
        
        return file.getFileName();
    }
    
    return "No IR loaded...";
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new IRLoaderAudioProcessor();
}
