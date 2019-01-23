#include <Arduino.h>
#include <MusicalMeter.h>

#define AUDIO_INPUT_PIN A0

MusicalMeter meter(6, 13);

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t reading = analogRead(AUDIO_INPUT_PIN) * 20;
  meter.DisplayAdaptive(reading);
  // meter.Display(reading);
  // meter.CycleLevels(1000);
  delay(2);
}