#include <Arduino.h>
#include <MusicalMeter.h>

#define AUDIO_INPUT_PIN A0

MusicalMeter meter(6, 13);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Serial.println(meter.GetNumberOfLevels());
}

void loop() {
  // put your main code here, to run repeatedly:
  int reading = analogRead(AUDIO_INPUT_PIN) * 20;
  // Serial.println(reading);
  // meter.DisplayAudioLevel(reading);
  meter.DisplayAudioLevelAverage(reading);
  // meter.CycleLevels(1000);
  delay(2);
}