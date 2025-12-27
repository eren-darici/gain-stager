#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor
    (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    setSize (200, 300);
    startTimerHz (10);

    // Input gain knob
    inputGainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    inputGainSlider.setTextBoxStyle(
        juce::Slider::TextBoxBelow, false, 60, 20);
    inputGainSlider.setRange(-24.0, 24.0, 0.01);
    inputGainSlider.setTextValueSuffix(" dB");

    addAndMakeVisible(inputGainSlider);

    inputGainSliderAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.getAPVTS(),
            "inputGain",
            inputGainSlider);

    backgroundImage = juce::ImageCache::getFromMemory(
        BinaryData::background_png,
        BinaryData::background_pngSize
    );

    setResizable(false, false);


}



AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour::fromRGB(49, 54, 56));

    g.drawImageWithin(
        backgroundImage,
        0, 0,
        getWidth(), getHeight(),
        juce::RectanglePlacement::stretchToFit
    );

    auto area = meterArea;

    const int meterWidth = 40;
    const int gap = 20;
    const int totalWidth = meterWidth * 2 + gap;

    // Center the whole meter block
    auto meters = area.withSizeKeepingCentre(totalWidth, area.getHeight());

    auto preArea  = meters.removeFromLeft(meterWidth);
    meters.removeFromLeft(gap);
    auto postArea = meters.removeFromLeft(meterWidth);

    auto drawMeter = [&g] (juce::Rectangle<int> bounds, float db, bool isPost)
    {
        // Background
        g.setColour(juce::Colour::fromRGB(204, 214, 235));
        g.fillRect(bounds);

        const float norm = juce::jlimit(
            0.0f, 1.0f,
            juce::Decibels::decibelsToGain(db, -120.0f));

        const int fillHeight = int(norm * bounds.getHeight());

        // Color logic
        if (isPost && db >= 0.0f)
            g.setColour(juce::Colour::fromRGB(202, 60, 37));
        else
            g.setColour(juce::Colour::fromRGB(0, 155, 114));

        g.fillRect(bounds.removeFromBottom(fillHeight));
    };

    auto drawScale = [&g] (juce::Rectangle<int> bounds)
    {
        g.setColour(juce::Colours::grey);
        g.setFont(10.0f);

        const float marks[] = { 0.0f, -6.0f, -12.0f, -18.0f };

        for (float db : marks)
        {
            const float norm = juce::Decibels::decibelsToGain(db, -120.0f);
            const int y = bounds.getBottom() - int(norm * bounds.getHeight());

            // Tick line
            g.drawLine(
                float(bounds.getX() - 4), float(y),
                float(bounds.getX()),     float(y),
                1.0f);

            // Label
            g.drawText(
                juce::String(db, 0),
                bounds.getX() - 36, y - 7,
                30, 14,
                juce::Justification::right);
        }
    };



    drawMeter(preArea,  preDb,  false);
    drawMeter(postArea, postDb, true);

    g.setColour(juce::Colours::white);
    drawScale(preArea);
    // drawScale(postArea);
    g.drawFittedText("PRE",  preArea,  juce::Justification::centredBottom, 1);
    g.drawFittedText("POST", postArea, juce::Justification::centredBottom, 1);
}


void AudioPluginAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);

    // Top: knob
    auto knobArea = area.removeFromTop(140);
    inputGainSlider.setBounds(knobArea.reduced(20));

    // Bottom: meters (leave space, paint handles drawing)
    meterArea = area;
}


void AudioPluginAudioProcessorEditor::timerCallback()
{
    preDb  = processorRef.getPreMeterDb();
    postDb = processorRef.getPostMeterDb();
    repaint();
}