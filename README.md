# MusicalLights
Arduino library to sync a custom-length string of lights to music.

Not exactly a VU Meter, although it behaves similarly; it attempts to use the full range of light levels regardless of volume.
By keeping track of maximum input, it can define thresholds for each light level on the fly -- sort of an adaptive VU Meter.
This library allows for custom number of light outputs, up to the physical number of digital pins on Arduino (14 on Uno).
