#include <Arduino.h>
#include <MusicalMeter.h>

// Constructor for MusicalMeter object.
// Creates an array of size number_of_levels, counting down from starting_pin
// Example: a 3-level MusicalMeter with starting pin 13 will create an array
// of size 3, containing 13, 12, and 11 (corresponding to pins).
// Also initializes some private fields, such as current_max_, which will be
// updated when the object begins reading signal values.
MusicalMeter::MusicalMeter(uint8_t number_of_levels, uint8_t starting_pin) {
  zero_level_ = 0;  // set to default for now
  current_max_ = 0; // set to zero for now
  number_of_levels_ = number_of_levels;
  led_pins_ = new uint8_t[number_of_levels];
  // initialize the digital pins as output
  uint8_t pin_index = starting_pin;
  for (uint8_t i=0; i<number_of_levels_; i++) {
    led_pins_[i] = pin_index;
    pinMode(led_pins_[i], OUTPUT);  // configure pin as output
    pin_index--;  // decrease the pin index to go to next pin down
  }
  average_index_ = 0; // used for averaging
}

// Setter for zero_level_ private field.
// zero_level is used when input signal is DC-biased;
// Biasing may be desired in order to get a more complete audio signal.
// Ideally, the biasing point should be at the midpoint of the possible range
// of analog values Arduino can detect (0-1023) -- so about 512, which turns
// out to be around 2.5 volts.
void MusicalMeter::SetZeroLevel(uint16_t zero_level) {
  zero_level_ = zero_level;
}

// Tester function to display levels activating.
// Cycles between all defined levels -- does not require input signal.
// Delay is in milliseconds.
void MusicalMeter::CycleLevels(uint32_t delay_between_levels) {
  for (int i = 0; i < number_of_levels_; i++) {
    digitalWrite(led_pins_[i], HIGH);
    delay(delay_between_levels);
    digitalWrite(led_pins_[i], LOW);
  }
}

// Main function for adaptive display of signal level.
// Corrects reading in case of biasing, then defines max_value if larger than
// current. Finally, passes to DisplayAudioLevelBasic to display level.
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

// Activates pins based on input signal level and current maximum value.
// Default maximum is upper limit of Arduino reading.
// Any remaining pins are deactivated.
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

// Writes values to internal buffer for later average calculations.
void MusicalMeter::WriteToBuffer(uint16_t level) {
  if (average_index_ >= AVERAGE_COUNT_BUFFER_SIZE) {
    average_index_ = 0;
  }
  average_buffer_[average_index_] = level;
  average_index_++;
}

// Reads values in internal buffer and returns average value.
uint16_t MusicalMeter::GetBufferAverage() {
  uint16_t average = 0;
  for (uint8_t i = 0; i < AVERAGE_COUNT_BUFFER_SIZE; i++) {
    average += average_buffer_[i];
  }

  return average/AVERAGE_COUNT_BUFFER_SIZE;
}

// Basic VU meter functionality.
// Does not adapt to lower levels; determines ouput based on default
// maximum (1023). Performs averaging on output, in order to get a
// smoother output.
void MusicalMeter::Display(uint16_t audio_level) {
  WriteToBuffer(audio_level);
  uint16_t average = GetBufferAverage();
  DisplayAudioLevelBasic(average);
}
