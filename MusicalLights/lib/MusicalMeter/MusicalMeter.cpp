#include <Arduino.h>
#include <MusicalMeter.h>

MusicalMeter::MusicalMeter(uint8_t numberOfLevels, uint8_t startingPin) {
  zero_level_ = 0;  // set to default for now
  current_max_ = 0; // set to zero for now
  number_of_levels_ = numberOfLevels;
  led_pins_ = new uint8_t[numberOfLevels];
  // initialize the digital pins as output
  uint8_t pin_index = startingPin;
  for (uint8_t i=0; i<number_of_levels_; i++) {
    led_pins_[i] = pin_index;
    pinMode(led_pins_[i], OUTPUT);  // configure pin as output
    pin_index--;  // decrease the pin index to go to next pin down
  }
  average_index_ = 0; // used for averaging
}

void MusicalMeter::SetZeroLevel(uint16_t zero_level) {
  zero_level_ = zero_level;
}

void MusicalMeter::CycleLevels(uint32_t delay_between_levels) {
  for (int i = 0; i < number_of_levels_; i++) {
    digitalWrite(led_pins_[i], HIGH);
    delay(delay_between_levels);
    digitalWrite(led_pins_[i], LOW);
  }
}

void MusicalMeter::DisplayAdaptive(uint16_t audio_level) {
  // account for "negative values"
  if (audio_level < zero_level_) {
    audio_level = zero_level_ + (zero_level_ - audio_level);
  }
  if (audio_level > current_max_) {
    current_max_ = audio_level;
  }  
  DisplayAudioLevelBasic(audio_level, current_max_);
}

void MusicalMeter::DisplayAudioLevelBasic(uint16_t audio_level, 
                                          uint16_t max_value = 1023) {
  int i;
  if (audio_level == zero_level_) {
    for (i = 0; i < number_of_levels_; i++) {
      digitalWrite(led_pins_[i], LOW);
    }
    return;
  }
  int chunk_size = max_value/number_of_levels_;
  int active_levels = audio_level/chunk_size;
  for (i = 0; i < active_levels; i++) {
    digitalWrite(led_pins_[i], HIGH);
  }
  // reset lights
  for (int j = i; j < number_of_levels_; j++) {
    digitalWrite(led_pins_[j], LOW);
  }
}

void MusicalMeter::WriteToBuffer(uint16_t level) {
  if (average_index_ >= AVERAGE_COUNT_BUFFER_SIZE) {
    average_index_ = 0;
  }
  average_buffer_[average_index_] = level;
  average_index_++;
}

uint16_t MusicalMeter::GetBufferAverage() {
  uint16_t average = 0;
  for (uint8_t i = 0; i < AVERAGE_COUNT_BUFFER_SIZE; i++) {
    average += average_buffer_[i];
  }

  return average/AVERAGE_COUNT_BUFFER_SIZE;
}

void MusicalMeter::Display(uint16_t audio_level) {
  WriteToBuffer(audio_level);
  uint16_t average = GetBufferAverage();
  DisplayAudioLevelBasic(average);
}
