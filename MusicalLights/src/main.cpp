#include <Arduino.h>
#include <MusicalMeter.h>

#define AUDIO_INPUT_PIN A0

MusicalMeter meter(6, 13);

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  int reading = analogRead(AUDIO_INPUT_PIN) * 20;
  // meter.DisplayAudioLevel(reading);
  meter.DisplayAudioLevelAverage(reading);
  // meter.CycleLevels(1000);
  delay(2);
}