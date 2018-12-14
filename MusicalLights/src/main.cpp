#include <Arduino.h>
#include <VuMeter.cpp>

#define AUDIO_INPUT_PIN A0

VuMeter meter(6, 13);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(meter.GetNumberOfLevels());
}

void loop() {
  // put your main code here, to run repeatedly:
  int reading = analogRead(AUDIO_INPUT_PIN);
  meter.DisplayAudioLevel(reading);
  delay(2);
}