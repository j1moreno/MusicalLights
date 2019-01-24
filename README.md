# MusicalLights
Arduino library to sync a custom-length string of lights to music.

Not exactly a VU Meter, although it behaves similarly; it attempts to use the full range of light levels regardless of volume.
By keeping track of maximum input, it can define thresholds for each light level on the fly -- sort of an adaptive VU Meter. It can be helpful when you want to display light flashing to music, but the audio is low volume.
This library allows for custom number of light outputs, up to the physical number of digital pins on Arduino (14 on Uno).
It can also function as a traditional VU Meter, if desired.

## Usage

Initialization:

MusicalMeter meter(6, 13); // 6 levels meter, start from pin 13

Use the Display() method for basic VU Meter functionality.
For adaptive VU Meter, use DisplayAdaptive().

## Setup

This code was created using PlatformIO and VS Code; therefore the easiest way to use the code is to open it in a similar environment. The library itself should go in the lib directory, while the main.cpp demo file should go under src directory.
