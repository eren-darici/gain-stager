//
// Created by Eren Darici on 12/27/2025.
//

#include "GainStager.h"

void GainStager::prepare(double sampleRate)
{
    inputGainSmoothed.reset(sampleRate, 0.02f); // 20ms
    inputGainSmoothed.setCurrentAndTargetValue(1.0f);
}

void GainStager::setInputGain(float linearGain)
{
    inputGainSmoothed.setTargetValue(linearGain);
}

void GainStager::process(juce::AudioBuffer<float>& buffer)
{
    // Only this block -so discard prev values
    prePeak = 0.0f;
    postPeak = 0.0f;

    // Get the buffer related info
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    // For each channel and sample
    for (int channel = 0; channel < numChannels; ++channel)
    {
        // Get the sample write pointer
        float* samples = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; sample++)
        {
            const float inputSample = samples[sample]; // Get the input sample
            prePeak = juce::jmax(prePeak, std::abs(inputSample)); // Calculate the peak

            const float gain = inputGainSmoothed.getNextValue(); // Get the gain
            const float outputSample = inputSample * gain; // Apply the gain
            samples[sample] = outputSample; // Return the sample

            postPeak = juce::jmax(postPeak, std::abs(outputSample)); // Post peak
        }
    }
}