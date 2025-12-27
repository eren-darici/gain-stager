# Gain Stager 1.0.0

<p align="center">
  <img src="source/assets/screenshot.png" alt="Gain‑Stager Screenshot" />
</p>

**Gain‑Stager** is a simple gain staging audio plugin built with **JUCE and C++**. It helps you manage and visualize gain levels in your audio projects to prevent distortion and keep signals clean.

> ⚠️ Note: Only the **UI parts** of this plugin were AI-assisted. The audio processing logic is fully handcrafted.

---

## Features

- Gain staging
- Visual feedback of audio levels
- Works with VST3/AU formats
- Cross-platform support via JUCE

---

## Installation

### Precompiled VST3

1. Download the latest precompiled VST3 file from the releases page.
2. Copy `GainStager.vst3` to your DAW's plugin folder:
    - **Windows:** `C:\Program Files\Common Files\VST3\`
    - **Mac:** `/Library/Audio/Plug-Ins/VST3/`
3. Restart your DAW and insert the plugin on a track or bus.

### Compile from Source

1. Clone the repository:
   ```bash
   git clone https://github.com/eren-darici/gain-stager.git
   cd gain-stager
   
2. Open the project in your IDE (CLion, Visual Studio, Xcode).
3. Build the plugin.
4. Install the generated VST3/AU file into your DAW plugin folder.
