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
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"frequency", 1},
                                                  "Control for which notes to go through",
                                                  juce::NormalisableRange<float>(0, 180, 1, 1.2),
                                                  101),
    }),
    filteredNoteCount(0),
    distribution(0, 100)
{
    frequencyParameter  = parameters.getRawParameterValue ("frequency");
    parameters.addParameterListener("frequency", this);
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
     stop note on events since it isn't desirable to halt pitch wheel changes
     and stuff like that. Also, for every note on we catch, we should catch a corresponding
     note off (This is probably good practice, but necessary?). There are two modes, chance
     based gating (only X% of notes make it through) and periodic gating (every X out of Y
     notes make it through).
     */
    juce::MidiBuffer filteredMidi;
    for (const auto metadata : midiMessages) {
        const auto message { metadata.getMessage() };
        
        // We only want to intercept the message if it's a note-on
        if (message.isNoteOn()) {
            
            // Probability mode
            if (juce::roundToInt(frequencyParameter->load()) <= 100) {
                if (randomNumber() <= juce::roundToInt(frequencyParameter->load())) filteredMidi.addEvent(message, metadata.samplePosition);
            }
            
            // Periodic mode
            else {
                ++filteredNoteCount;
                
                switch (juce::roundToInt(frequencyParameter->load())) {
                    case 101:
                        filterNote(filteredMidi, metadata, 1, 1);
                        break;

                    case 102:
                        filterNote(filteredMidi, metadata, 1, 2);
                        break;

                    case 103:
                        filterNote(filteredMidi, metadata, 1, 3);
                        break;

                    case 104:
                        filterNote(filteredMidi, metadata, 2, 3);
                        break;

                    case 105:
                        filterNote(filteredMidi, metadata, 1, 4);
                        break;

                    case 106:
                        filterNote(filteredMidi, metadata, 3, 4);
                        break;

                    case 107:
                        filterNote(filteredMidi, metadata, 1, 5);
                        break;

                    case 108:
                        filterNote(filteredMidi, metadata, 2, 5);
                        break;

                    case 109:
                        filterNote(filteredMidi, metadata, 3, 5);
                        break;

                    case 110:
                        filterNote(filteredMidi, metadata, 4, 5);
                        break;

                    case 111:
                        filterNote(filteredMidi, metadata, 1, 6);
                        break;

                    case 112:
                        filterNote(filteredMidi, metadata, 5, 6);
                        break;

                    case 113:
                        filterNote(filteredMidi, metadata, 1, 7);
                        break;

                    case 114:
                        filterNote(filteredMidi, metadata, 2, 7);
                        break;

                    case 115:
                        filterNote(filteredMidi, metadata, 3, 7);
                        break;

                    case 116:
                        filterNote(filteredMidi, metadata, 4, 7);
                        break;

                    case 117:
                        filterNote(filteredMidi, metadata, 5, 7);
                        break;

                    case 118:
                        filterNote(filteredMidi, metadata, 6, 7);
                        break;

                    case 119:
                        filterNote(filteredMidi, metadata, 1, 8);
                        break;

                    case 120:
                        filterNote(filteredMidi, metadata, 3, 8);
                        break;

                    case 121:
                        filterNote(filteredMidi, metadata, 5, 8);
                        break;

                    case 122:
                        filterNote(filteredMidi, metadata, 7, 8);
                        break;

                    case 123:
                        filterNote(filteredMidi, metadata, 1, 9);
                        break;

                    case 124:
                        filterNote(filteredMidi, metadata, 2, 9);
                        break;

                    case 125:
                        filterNote(filteredMidi, metadata, 4, 9);
                        break;

                    case 126:
                        filterNote(filteredMidi, metadata, 5, 9);
                        break;

                    case 127:
                        filterNote(filteredMidi, metadata, 7, 9);
                        break;

                    case 128:
                        filterNote(filteredMidi, metadata, 8, 9);
                        break;

                    case 129:
                        filterNote(filteredMidi, metadata, 1, 10);
                        break;

                    case 130:
                        filterNote(filteredMidi, metadata, 3, 10);
                        break;

                    case 131:
                        filterNote(filteredMidi, metadata, 7, 10);
                        break;

                    case 132:
                        filterNote(filteredMidi, metadata, 9, 10);
                        break;

                    case 133:
                        filterNote(filteredMidi, metadata, 1, 11);
                        break;

                    case 134:
                        filterNote(filteredMidi, metadata, 2, 11);
                        break;

                    case 135:
                        filterNote(filteredMidi, metadata, 3, 11);
                        break;

                    case 136:
                        filterNote(filteredMidi, metadata, 4, 11);
                        break;

                    case 137:
                        filterNote(filteredMidi, metadata, 5, 11);
                        break;

                    case 138:
                        filterNote(filteredMidi, metadata, 6, 11);
                        break;

                    case 139:
                        filterNote(filteredMidi, metadata, 7, 11);
                        break;

                    case 140:
                        filterNote(filteredMidi, metadata, 8, 11);
                        break;

                    case 141:
                        filterNote(filteredMidi, metadata, 9, 11);
                        break;

                    case 142:
                        filterNote(filteredMidi, metadata, 10, 11);
                        break;

                    case 143:
                        filterNote(filteredMidi, metadata, 1, 12);
                        break;

                    case 144:
                        filterNote(filteredMidi, metadata, 5, 12);
                        break;

                    case 145:
                        filterNote(filteredMidi, metadata, 7, 12);
                        break;

                    case 146:
                        filterNote(filteredMidi, metadata, 11, 12);
                        break;

                    case 147:
                        filterNote(filteredMidi, metadata, 1, 13);
                        break;

                    case 148:
                        filterNote(filteredMidi, metadata, 2, 13);
                        break;

                    case 149:
                        filterNote(filteredMidi, metadata, 3, 13);
                        break;

                    case 150:
                        filterNote(filteredMidi, metadata, 4, 13);
                        break;

                    case 151:
                        filterNote(filteredMidi, metadata, 5, 13);
                        break;

                    case 152:
                        filterNote(filteredMidi, metadata, 6, 13);
                        break;

                    case 153:
                        filterNote(filteredMidi, metadata, 7, 13);
                        break;

                    case 154:
                        filterNote(filteredMidi, metadata, 8, 13);
                        break;

                    case 155:
                        filterNote(filteredMidi, metadata, 9, 13);
                        break;

                    case 156:
                        filterNote(filteredMidi, metadata, 10, 13);
                        break;

                    case 157:
                        filterNote(filteredMidi, metadata, 11, 13);
                        break;

                    case 158:
                        filterNote(filteredMidi, metadata, 12, 13);
                        break;

                    case 159:
                        filterNote(filteredMidi, metadata, 1, 14);
                        break;

                    case 160:
                        filterNote(filteredMidi, metadata, 3, 14);
                        break;

                    case 161:
                        filterNote(filteredMidi, metadata, 5, 14);
                        break;

                    case 162:
                        filterNote(filteredMidi, metadata, 9, 14);
                        break;

                    case 163:
                        filterNote(filteredMidi, metadata, 11, 14);
                        break;

                    case 164:
                        filterNote(filteredMidi, metadata, 13, 14);
                        break;

                    case 165:
                        filterNote(filteredMidi, metadata, 1, 15);
                        break;

                    case 166:
                        filterNote(filteredMidi, metadata, 2, 15);
                        break;

                    case 167:
                        filterNote(filteredMidi, metadata, 4, 15);
                        break;

                    case 168:
                        filterNote(filteredMidi, metadata, 7, 15);
                        break;

                    case 169:
                        filterNote(filteredMidi, metadata, 8, 15);
                        break;

                    case 170:
                        filterNote(filteredMidi, metadata, 11, 15);
                        break;

                    case 171:
                        filterNote(filteredMidi, metadata, 13, 15);
                        break;

                    case 172:
                        filterNote(filteredMidi, metadata, 14, 15);
                        break;

                    case 173:
                        filterNote(filteredMidi, metadata, 1, 16);
                        break;

                    case 174:
                        filterNote(filteredMidi, metadata, 3, 16);
                        break;

                    case 175:
                        filterNote(filteredMidi, metadata, 5, 16);
                        break;

                    case 176:
                        filterNote(filteredMidi, metadata, 7, 16);
                        break;

                    case 177:
                        filterNote(filteredMidi, metadata, 9, 16);
                        break;

                    case 178:
                        filterNote(filteredMidi, metadata, 11, 16);
                        break;

                    case 179:
                        filterNote(filteredMidi, metadata, 13, 16);
                        break;

                    case 180:
                        filterNote(filteredMidi, metadata, 15, 16);
                        break;
                        
                    default:
                        continue;
                }
            }
        } else {
            filteredMidi.addEvent(message, metadata.samplePosition);
        }
    }
    midiMessages.swapWith(filteredMidi);
}

void TriggerConditionAudioProcessor::parameterChanged (const juce::String &parameterID, float newValue)
{
    if (parameterID == "frequency") filteredNoteCount = 0;
}

void TriggerConditionAudioProcessor::filterNote(juce::MidiBuffer& buffer, const juce::MidiMessageMetadata& messageMetadata, int passedMessagesPerPeriod, int periodLength)
{
    if (filteredNoteCount <= passedMessagesPerPeriod) {
        auto message { messageMetadata.getMessage() };
        buffer.addEvent(message, messageMetadata.samplePosition);
    }
    
    if (filteredNoteCount == periodLength) {
        filteredNoteCount = 0;
    }
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
