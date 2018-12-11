#include <Arduino.h>

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int led_start = 13;  // pin number where LEDs will start from
const int led_count = 4;  // number of LEDs that have been connected to Arduino, counting down from pin 13
const int ZERO_VALUE = 514;  // when no music is playing, pin reads this value because of biasing

int zero_thresh = 100;  // number of times sensor can read zero without reacting
int zero_count = 0;  // keeps track of number of times zero value has been read
int thresh_count = led_count + 2;
int sensorValue = 0;        // value read from the pot
int led_pins[led_count];  // create an array containing all defined led pins
int thresholds[led_count+2];  // 2 extra thresholds, first will be ignored for case when sensor pick up 0, last will be used for last LED threshold
int led_states[led_count];  // array will hold determined states of leds

int current_max = ZERO_VALUE;  // holds current max amplitude measured

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  // initialize the digital pins as output
  int pin_index = led_start;
  for(int i=0; i<led_count; i++){
    led_pins[i] = pin_index;
    pinMode(led_pins[i], OUTPUT);  // configure pin as output
    pin_index--;  // decrease the pin index to go to next pin down
    // temporarily set all led_states to low:
    led_states[i] = LOW;
  }
  for(int j=0; j<thresh_count; j++){
    thresholds[j] = ZERO_VALUE;
  }  
  
}

void printValue(int value){
   Serial.print("sensor = " );                       
  Serial.print(sensorValue); 
  Serial.print("\n"); 
}

void ledReact(int sensorValue){
  // set states to low to start
  //  for(int i=0; i<led_count; i++){
  //    led_states[i] = LOW;
  //  }
  // cycle through all thresholds to determine whether LED should be on
  for(int i=0; i<led_count; i++){
    if(sensorValue >= thresholds[i+1]){
      led_states[i] = HIGH;
    }else{
      led_states[i] = LOW; 
    }
  }
  // now set all leds to their determined state
  for(int j=0; j<led_count; j++){
    digitalWrite(led_pins[j], led_states[j]);
  }
}

void toggleLEDs(int on_off)
{
  for(int i=0; i<led_count; i++){
    digitalWrite(led_pins[i], on_off);
  }
}

void redefineThresholds(int sensorValue)
{
  Serial.print("New max value! Redefining thresholds \n");
  Serial.print(sensorValue);
  Serial.print("\n");
  // sensor value is max value, define thresholds accordingly:
  int region_size = (sensorValue - ZERO_VALUE)/(led_count+2);  // 2 extra thresholds
  for(int i=0; i<thresh_count; i++){
    thresholds[i] = region_size * i + ZERO_VALUE;  // account for offset
    Serial.print(thresholds[i]);
    Serial.print("\n");
  }
  // reset all states to low
//  for(int j=0; j<led_count; j++){
//    digitalWrite(led_pins[j], LOW);
//  }
}

boolean exceedsZeroCount(int sensorValue)
{
  if (sensorValue == ZERO_VALUE){
    zero_count++;
  }
  if (zero_count < zero_thresh) {
    return false;
  }else if (sensorValue < thresholds[1]){
    return true;
  }else{
    // reset counter
    zero_count = 0;
    current_max = ZERO_VALUE;
    return true;
  } 
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);  
  
  // account for "negative values"
  if (sensorValue < ZERO_VALUE) {
    sensorValue = ZERO_VALUE + (ZERO_VALUE - sensorValue);
  }
  
  // print to serial:
  printValue(sensorValue);
  
  if (sensorValue > current_max){
    // if higher max was found, redefine the thresholds
    redefineThresholds(sensorValue);
    current_max = sensorValue;
  }  
  
    // account for crossing through zero value on oscillation
    if (!exceedsZeroCount(sensorValue)){
        ledReact(sensorValue);
    }
  

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(2);  
}