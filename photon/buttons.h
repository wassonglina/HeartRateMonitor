// Process button inputs and return button activity

#define NUMBUTTONS 2
#define MODEBUTTON 6
#define BRIGHTNESSBUTTON 3

#define BTNIDLE 0
#define BTNDEBOUNCING 1
#define BTNPRESSED 2
#define BTNRELEASED 3
#define BTNLONGPRESS 4
#define BTNLONGPRESSREAD 5

#define BTNDEBOUNCETIME 20
#define BTNLONGPRESSTIME 1000

unsigned long buttonEvents[NUMBUTTONS];
byte buttonStatuses[NUMBUTTONS];
byte buttonmap[NUMBUTTONS] = {MODEBUTTON};

void updateButtons() {
  for (byte i = 0; i < NUMBUTTONS; i++) {
    switch(buttonStatuses[i]) {
      case BTNIDLE:
        if (digitalRead(buttonmap[i]) == LOW) {
          buttonEvents[i] = millis();
          buttonStatuses[i] = BTNDEBOUNCING;
        }
      break;

      // TODO this might be inaccurate. If there is a bounce, it will never leave the BTNDEBOUNCING state.
      // Shouldn't there be a check for HIGH state after BTNDEBOUNCETIME?
      // Currently should be more like "may be currently bouncing or idle after a press"
      case BTNDEBOUNCING:
        if (millis() - buttonEvents[i] > BTNDEBOUNCETIME) {
          if (digitalRead(buttonmap[i]) == LOW) {
            buttonStatuses[i] = BTNPRESSED;
          }
        }
      break;

      case BTNPRESSED:
        if (digitalRead(buttonmap[i]) == HIGH) {
          buttonStatuses[i] = BTNRELEASED;
        } else if (millis() - buttonEvents[i] > BTNLONGPRESSTIME) {
            buttonStatuses[i] = BTNLONGPRESS;
        }
      break;

      case BTNRELEASED:
      break;

      case BTNLONGPRESS:
      break;

      case BTNLONGPRESSREAD:
        if (digitalRead(buttonmap[i]) == HIGH) {
          buttonStatuses[i] = BTNIDLE;
        }
      break;
    }
  }
}

byte buttonStatus(byte buttonNum) {

  byte tempStatus = buttonStatuses[buttonNum];
  if (tempStatus == BTNRELEASED) {
    buttonStatuses[buttonNum] = BTNIDLE;
  } else if (tempStatus == BTNLONGPRESS) {
    buttonStatuses[buttonNum] = BTNLONGPRESSREAD;
  }

  return tempStatus;

}
