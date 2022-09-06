/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TriggerConditionAudioProcessor::TriggerConditionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    parameters(*this, nullptr, juce::Identifier("TriggerCondition"), {
        std::make_unique<juce::AudioParameterBool>("chanceMode",
                                                   "Filter based on chance",
                                                   false),
        std::make_unique<juce::AudioParameterInt>("percentage",
                                                  "Chance for notes to go through",
                                                  0,
                                                  100,
                                                  10),
        std::make_unique<juce::AudioParameterInt>("allowedMessageFrequency",
                                                  "One i X messages go through",
                                                  1,
                                                  1000,
                                                  4)
    }),
    filteredNotes(0),
    distribution(0, 100)
{
    chanceModeParameter = parameters.getRawParameterValue ("chanceMode");
    percentageParameter  = parameters.getRawParameterValue ("percentage");
    allowedMessageFrequencyParameter = parameters.getRawParameterValue("allowedMessageFrequency");
}

TriggerConditionAudioProcessor::~TriggerConditionAudioProcessor()
{
}

//==============================================================================
const juce::String TriggerConditionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TriggerConditionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TriggerConditionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TriggerConditionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TriggerConditionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TriggerConditionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TriggerConditionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TriggerConditionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TriggerConditionAudioProcessor::getProgramName (int index)
{
    return {};
}

void TriggerConditionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TriggerConditionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
}

void TriggerConditionAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TriggerConditionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void TriggerConditionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    // Since this is a MIDI-plugin, we shouldn't have any audio channels.
    jassert(buffer.getNumChannels() == 0);
    
    /* Here we want to check for MIDI messages to stop. We only want to
     stop note on/off events since it isn't desirable to halt pitch wheel changes
     and stuff like that. Also, for every note on we catch, we should catch a corresponding
     note off (This is probably good practice, but necessary?). There are two modes, chance based gating (only X% of notes make it through) and
     periodic gating (every X notes make it through).
     */
    juce::MidiBuffer filteredMidi;
    for (const auto metadata : midiMessages) {
        const auto message { metadata.getMessage() };
        
        // We only want to intercept the message if it's a note-on
        if (message.isNoteOn()) {
            
            if (juce::roundToInt(chanceModeParameter->load())) {
                if (randomNumber() > juce::roundToInt(percentageParameter->load())) continue;
            } else {
                ++filteredNotes;
                if (filteredNotes < juce::roundToInt(allowedMessageFrequencyParameter->load())) {
                    continue;
                } else {
                    filteredNotes = 0;
                }
            }
        }
        filteredMidi.addEvent(message, metadata.samplePosition);
    }
    midiMessages.swapWith(filteredMidi);
}

//==============================================================================
bool TriggerConditionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TriggerConditionAudioProcessor::createEditor()
{
    return new TriggerConditionAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void TriggerConditionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TriggerConditionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TriggerConditionAudioProcessor();
}
