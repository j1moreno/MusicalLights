#include <Arduino.h>
#include <MusicalMeter.h>

MusicalMeter::MusicalMeter(int numberOfLevels, int startingPin, int zero_value) {
  number_of_levels_ = numberOfLevels;
  led_pins_ = new int[numberOfLevels];
  // initialize the digital pins as output
  int pin_index = startingPin;
  for(int i=0; i<number_of_levels_; i++) {
    led_pins_[i] = pin_index;
    pinMode(led_pins_[i], OUTPUT);  // configure pin as output
    pin_index--;  // decrease the pin index to go to next pin down
    thresholds_ = new int[number_of_levels_ + 2];  // 2 extra thresholds, first will be ignored for case when sensor pick up 0, last will be used for last LED threshold
    led_states_ = new int[number_of_levels_];  // array will hold determined states of leds
    thresh_count_ = number_of_levels_ + 2;
  }
}

int MusicalMeter::GetNumberOfLevels() {
  return number_of_levels_;
}

void MusicalMeter::CycleLevels(int delay_between_levels) {
  for (int i = 0; i < number_of_levels_; i++) {
    digitalWrite(led_pins_[i], HIGH);
    delay(delay_between_levels);
    digitalWrite(led_pins_[i], LOW);
  }
}

bool MusicalMeter::ExceedsZeroCount(int audio_level)
{
  if (audio_level == ZERO_VALUE) {
    zero_count_++;
  }
  if (zero_count_ < zero_thresh) {
    return false;
  } else if (audio_level < thresholds_[1]) {
    return true;
  } else {
    // reset counter
    zero_count_ = 0;
    current_max_ = ZERO_VALUE;
    return true;
  } 
}

void MusicalMeter::RedefineThresholds(int audio_level)
{
  Serial.print("New max value! Redefining thresholds \n");
  Serial.print(audio_level);
  Serial.print("\n");
  // sensor value is max value, define thresholds accordingly:
  int region_size = (audio_level - ZERO_VALUE)/(number_of_levels_+2);  // 2 extra thresholds
  for(int i=0; i<thresh_count_; i++) {
    thresholds_[i] = region_size * i + ZERO_VALUE;  // account for offset
    Serial.print(thresholds_[i]);
    Serial.print("\n");
  }
}

void MusicalMeter::LedReact(int audio_level){
  // cycle through all thresholds to determine whether LED should be on
  for(int i=0; i<number_of_levels_; i++) {
    if(audio_level >= thresholds_[i+1]) {
      led_states_[i] = HIGH;
    } else {
      led_states_[i] = LOW; 
    }
  }
  // now set all leds to their determined state
  for(int j=0; j<number_of_levels_; j++) {
    digitalWrite(led_pins_[j], led_states_[j]);
  }
}

void MusicalMeter::DisplayAudioLevel(int audio_level) {
  Serial.print(audio_level);
  Serial.print("\t");
  // account for "negative values"
  if (audio_level < ZERO_VALUE) {
    audio_level = ZERO_VALUE + (ZERO_VALUE - audio_level);
  }
  Serial.println(audio_level);
  if (audio_level > current_max_) {
    // if higher max was found, redefine the thresholds
    RedefineThresholds(audio_level);
    current_max_ = audio_level;
  }  
  // account for crossing through zero value on oscillation
  if (!ExceedsZeroCount(audio_level)) {
      LedReact(audio_level);
  }
}

void MusicalMeter::DisplayAudioLevelBasic(int audio_level) {
  if (audio_level == 0) {
    for (int i = 0; i < number_of_levels_; i++) {
      digitalWrite(led_pins_[i], LOW);
    }
    return;
  }
  int chunk_size = 1023/number_of_levels_;
  int active_levels = audio_level/chunk_size;
  for (int i = 0; i < active_levels; i++) {
    digitalWrite(led_pins_[i], HIGH);
  }
  // reset lights
  for (int i = i; i < number_of_levels_; i++) {
    digitalWrite(led_pins_[i], LOW);
  }
}