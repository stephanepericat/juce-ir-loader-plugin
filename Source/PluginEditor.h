/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class IRLoaderAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    IRLoaderAudioProcessorEditor (IRLoaderAudioProcessor&);
    ~IRLoaderAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextButton loadButton;
    juce::Label irNameLabel;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    IRLoaderAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRLoaderAudioProcessorEditor)
};
