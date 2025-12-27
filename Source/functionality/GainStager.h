#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

class GainStager
{
public:
    // Prepare
    void prepare(double sampleRate);
    // Control rate
    void setInputGain(float linearGain);
    // Audio rate
    void process(juce::AudioBuffer<float>& buffer);

    // For meters
    float getPrePeak() const { return prePeak; }
    float getPostPeak() const { return postPeak; }

private:
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> inputGainSmoothed; // Smoothed input gain
    float prePeak = 0.0f;
    float postPeak = 0.0f;
};
