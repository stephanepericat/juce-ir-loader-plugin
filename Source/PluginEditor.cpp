/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
IRLoaderAudioProcessorEditor::IRLoaderAudioProcessorEditor (IRLoaderAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    loadButton.setButtonText("Load an IR...");
//    loadButton.onClick = [&]() {
//
//    };
    addAndMakeVisible(loadButton);
    
    irNameLabel.setText("No IR loaded...", juce::NotificationType::dontSendNotification);
    irNameLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(irNameLabel);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
}

IRLoaderAudioProcessorEditor::~IRLoaderAudioProcessorEditor()
{
}

//==============================================================================
void IRLoaderAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

//    g.setColour (juce::Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void IRLoaderAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    loadButton.setBounds(getWidth() / 2 - 75, getHeight() / 2 - 25, 150, 50);
    irNameLabel.setBounds(getWidth() / 2 - 75, getHeight() / 2 + 35, 150, 50);
}
