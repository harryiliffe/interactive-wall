#include <Arduino.h>
#include <FastLED.h>

#define LEDS_PER_BUTTON 3

class Button {
  public:
    Button(void);
    void light(CRGB colour);
    int nodeID;
    int buttonID;
    bool state;
    bool registered;
    CRGB colours[LEDS_PER_BUTTON];

    int parentID[3];
    int childID[3];
    int parentSectionID;
    int childSectionID;

};
