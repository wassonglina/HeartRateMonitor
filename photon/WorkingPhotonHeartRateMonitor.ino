#include "FastLED.h"
FASTLED_USING_NAMESPACE;

#include "recordedData.h"
#include "buttons.h"

#define LIGHT_PIN 0
#define NUM_LEDS 24
CRGB leds[NUM_LEDS];

int l0plus = 4;
int l0minus = 5;

int currentPulseValue;
int currentBrightness;
float adjustedPulseValue;
int currentRed;

bool useRecordedData = true; // prerecorded data to simulate a heartbeat.
bool useSerialMonitor = true; // print to serial monitor (for something like Processing). Note: this will lower the framerate.

void setup() {
  pinMode(l0plus, INPUT);
  pinMode(l0minus, INPUT);
  pinMode(MODEBUTTON, INPUT_PULLUP);
  FastLED.addLeds<NEOPIXEL, LIGHT_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  if (useSerialMonitor) {
    Serial.begin(9600);
  }
}

int getNextRealValue() {
  return analogRead(A0);
}

int lastRecordedValueIndex = 11748;
int currentRecordedValueIndex = 0;
int getNextRecordedValue() {
  if (currentRecordedValueIndex >= lastRecordedValueIndex) {
    currentRecordedValueIndex = 0;
  }
  return recordedData[currentRecordedValueIndex++];
}

int getNextValue() {
  if (useRecordedData) {
    return getNextRecordedValue();
  } else {
    return getNextRealValue();
  }
}

void checkForButtonPress() {
  updateButtons(); // read, debounce, and process the buttons
  switch(buttonStatus(0)) {
    case BTNRELEASED: // button was pressed and released quickly
    useRecordedData = !useRecordedData;
    break;
    case BTNLONGPRESS: // button was held down for a while
    useRecordedData = !useRecordedData;
    break;
  }
}

void updatePulse() {
  if (useRecordedData || !(digitalRead(l0plus) == 1 || digitalRead(l0minus) == 1)){
      currentPulseValue = getNextValue();
      adjustedPulseValue = currentPulseValue / 4000.0;
      currentBrightness = adjustedPulseValue * 100;
      currentRed = adjustedPulseValue * 100;
      for(int led = 0; led < NUM_LEDS; led++) {
        leds[led] = CRGB(currentRed, 0, 255);
      }
      FastLED.setBrightness(currentBrightness);
      if (useSerialMonitor) {
          Serial.println(currentPulseValue);
      }
  } else {
      if (useSerialMonitor) {
        Serial.println("!");
      }
  }
  if (useSerialMonitor) {
      delay(4);
  }
}

void loop() {
  checkForButtonPress();
  updatePulse();
  FastLED.show();
}
