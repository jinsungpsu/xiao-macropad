/*
jinsungpsu.com
https://jinsungpsu.com/macropad-custom-keyboard-using-arduino-seeeduino-xiao/

Summary
This is code for the Seeduino XIAO microcontroller, using Arduino/Keyboard.h library

Wiring details
  NO RESISTORS NEEDED (using Xiao's built in pullup input mode)

  Using 2 pin momentary buttons - tested using cherry MX mechanical keyboard switches
  One pin from each button wired to ground

  One pin from each button wired to a different input

  If you need more than the number of inputs in the Xiao, you can wire as a matrix instead
  https://www.dairequinlan.com/2020/12/the-keyboard-part-2-the-matrix/


Other information:
  LED_BUILTIN (low means one, high means off)
  When checking for logic throughout code, LOW means that input is detected

  Prototype built using: https://www.thingiverse.com/thing:4602619

*/

#include "Keyboard.h"

// Struct to encapsulate key mapping information
struct KeyMap {
  int keyCode = 0;
  String name = "Unused";
};

// enum to map to the keycaps used for reference/easy to read code from here on out
// These map to 0 through 10, corresponding to pins A0 to A11
enum keys{
  ONE,    //0 - bottom left     (A0)
  TWO,    //1 - bottom mid      (A1)
  THREE,  //2 - bottom right    (A2)
  FOUR,   //3 - mid left        (A3)
  FIVE,   //4 - mid mid         (A4)
  SIX,    //5 - mid right       (A5)
  NA1,    //6 - unused          (A6)
  NA2,    //7 - unused          (A7)
  SEVEN,  //8 - top left        (A8)
  EIGHT,  //9 - top mid         (A9)
  NINE    //10 - top right      (A10)
};

// global variables

// Configurable options
const int DELAY_BETWEEN_KEYPRESS = 75;

// simultaneously pressing down these TWO button combinations turns keyboard on/off
const int ENABLE_BTN1 = ONE;
const int ENABLE_BTN2 = THREE;
const int DISABLE_BTN1 = SEVEN;
const int DISABLE_BTN2 = NINE;

// constants/globals used - do not touch
const int NUM_INPUTS = 11;
bool enableKeyboard = false;       // keyboaord is off by default
struct KeyMap mappings[NUM_INPUTS];

void initializeMappings() {
  /* 
  mapping info

  For "special" keys, check https://github.com/arduino-libraries/Keyboard/blob/master/src/Keyboard.h
  For example:
  mappings[FOUR].keyCode = KEY_LEFT_ARROW;
  mappings[FOUR].name = "Left Arrow";
  
  For "regular" keys, use a character in single quotes - for example:
  mappings[ONE].keyCode = 'a';
  mappings[ONE].name = "A key";
  */
  mappings[FOUR].keyCode = KEY_LEFT_ARROW;
  mappings[FOUR].name = "Left Arrow";

  mappings[FIVE].keyCode = KEY_DOWN_ARROW;
  mappings[FIVE].name = "Down Arrow";

  mappings[SIX].keyCode = KEY_RIGHT_ARROW;
  mappings[SIX].name = "Right Arrow";

  mappings[EIGHT].keyCode = KEY_UP_ARROW;
  mappings[EIGHT].name = "Up Arrow";
}

void checkKeyboardStatus() {
  if (digitalRead(ENABLE_BTN1) == LOW && digitalRead(ENABLE_BTN2) == LOW) {
    enableKeyboard = true;
    digitalWrite(LED_BUILTIN, LOW);
  } else if (digitalRead(DISABLE_BTN1) == LOW && digitalRead(DISABLE_BTN2) == LOW) {
    enableKeyboard = false;
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void sendKeyPress() {
  for (int i = 0; i < NUM_INPUTS; i++) {
    if (digitalRead(i) == LOW) {
      Serial.println(mappings[i].name);
      Keyboard.write(mappings[i].keyCode);
    }
  }
}

void setup() {

  initializeMappings();

  // initialize built in LED to output mode
  pinMode(LED_BUILTIN, OUTPUT);
  
  for (int i = 0; i < 11; i++) {
    pinMode(i, INPUT_PULLUP);     // Initialize all inputs into pullup input mode
  }

  Keyboard.begin();
  Serial.begin(9600);
}

void loop() {
  checkKeyboardStatus();

  if (enableKeyboard) {
    sendKeyPress();
  }

  delay(DELAY_BETWEEN_KEYPRESS);
}
