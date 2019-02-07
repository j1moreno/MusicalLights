# MusicalLights
Arduino library to sync a custom-length string of lights to music.

Not exactly a VU Meter, although it behaves similarly; it attempts to use the full range of light levels regardless of volume.
By keeping track of maximum input, it can define thresholds for each light level on the fly -- sort of an adaptive VU Meter. It can be helpful when you want to display lights flashing to music, but the audio is low volume.
This library allows for custom number of light outputs, up to the physical number of digital pins on Arduino (14 on Uno).
It can also function as a traditional VU Meter, if desired.

Demo video: https://www.youtube.com/watch?v=37XU9SHiMog

## Usage

Initialization:

MusicalMeter meter(6, 13); // 6-level meter, start from pin 13

Use the Display() method for basic VU Meter functionality.
For adaptive VU Meter, use DisplayAdaptive().

## How to Use

This code was created using PlatformIO and VS Code; therefore the easiest way to use the code is to open it in a similar environment.

### PlatformIO
- Clone repository into an empty directory
- Open PlatformIO plugin within VS Code
- Select Open Project
- Browse for cloned repository and open it

That's it! 
Compile and run main.cpp (it contains a demo of functionality with input on A0 and six LEDs).

### Arduino IDE
For running in the Arduino IDE, you will need to add the library code to the Arduino Libraries directory in order to access it from your main ino file, instructions are here:
https://www.arduino.cc/en/Guide/Libraries#toc4
