#include <Wire.h>
#include <FastLED.h>
#include <EEPROM.h>


#define NUM_SECTIONS 18
#define NUM_LEDS 129
#define DATA_PIN 5
#define NUM_BUTTONS 16
#define DEFAULTDELAY 3
#define ttp229 (0xAF>>1)

uint16_t data_out = 0;
uint16_t data1, data2;

int idleTime = 30 * 1000;
long timeSinceButtonPressed;
int idleButtonCalc;
bool idle = true;

CRGB leds[NUM_LEDS];

int parentID;
int parentBreakPointTemp;

struct ColourStates {
  CRGB before;
  CRGB after;
  CRGB buttonBefore;
  CRGB buttonAfter;
  CRGB buttonOn;
  CRGB idle;
};

//ColourStates colour = {CRGB::MediumAquamarine, CRGB::Blue, CRGB::PaleGreen, CRGB::Red, CRGB::Purple};
ColourStates colour = {0x003e00, 0x310031, CRGB::Green, CRGB::MediumPurple, CRGB::White, CRGB::White};


struct Button {
  int id;
  bool state;
  bool lit;
  bool held;
  int sectionParent;
  int sectionChild;
  int ledIndex;
};

Button button[NUM_BUTTONS] = {
  {0, 0, 0, 0, 0, 1, 10},
  {1, 0, 0, 0, 2, 99, 61},
  {2, 0, 0, 0, 3, 99, 63},
  {3, 0, 0, 0, 4, 99, 65},
  {4, 0, 0, 0, 1, 5, 36},
  {5, 0, 0, 0, 6, 8, 55},
  {6, 0, 0, 0, 8, 99, 59},
  {7, 0, 0, 0, 11, 12, 67},
  {8, 0, 0, 0, 7, 9, 44},
  {9, 0, 0, 0, 9, 99, 49},
  {10, 0, 0, 0, 10, 13, 116},
  {11, 0, 0, 0, 16, 17, 87},
  {12, 0, 0, 0, 12, 99, 80},
  {13, 0, 0, 0, 14, 15, 124},
  {14, 0, 0, 0, 17, 99, 91},
  {15, 0, 0, 0, 15, 99, 128},


};

struct Section {
  int id;
  bool state;
  int parentID;
  int parentBreakPoint;
  int childID[5];
  int numLEDS;
  int ledIndex;
  int leftButtonID;
  int rightButtonID;
};

Section sections[NUM_SECTIONS] = {
  {0, 0, 99, 99, {1, 99, 99, 99, 99}, 10, 0, 99, 0},
  {1, 0, 0, 10, {2, 3, 4, 10, 5}, 25, 11, 0, 4},
  {2, 0, 1, 6, {99, 99, 99, 99, 99},  1, 60, 99, 1},
  {3, 0, 1, 12, {99, 99, 99, 99, 99},  1, 62, 99, 2},
  {4, 0, 1, 18, {99, 99, 99, 99, 99},  1, 64, 99, 3},
  {5, 0, 1, 25, {6, 7, 99, 99, 99},  1, 37, 4, 99},
  {6, 0, 5, 1, {8, 99, 99, 99, 99},  5, 50, 99, 5},
  {7, 0, 5, 1, {9, 99, 99, 99, 99},  6, 38, 99, 8},
  {8, 0, 6, 5, {99, 99, 99, 99, 99},  3, 56, 5, 6},
  {9, 0, 7, 6, {99, 99, 99, 99, 99},  4, 45, 8, 9},
  {10, 0, 1, 23, {11, 13, 99, 99, 99},  24, 92, 99, 10},
  {11, 0, 10, 11, {12, 99, 99, 99, 99},  1, 66, 99, 7},
  {12, 0, 11, 1, {99, 99, 99, 99, 99},  12, 68, 7, 12},
  {13, 0, 10, 24, {16, 14, 99, 99, 99},  1, 117, 10, 99},
  {14, 0, 13, 1, {15, 99, 99, 99, 99},  6, 118, 99, 13},
  {15, 0, 14, 6, {99, 99, 99, 99, 99},  3, 125, 13, 15},
  {16, 0, 13, 1, {17, 99, 99, 99, 99},  6, 81, 99, 11},
  {17, 0, 16, 6, {99, 99, 99, 99, 99},  3, 88, 11, 14},
};

int idleChainLED = 0;
int idleTemp = 9;

bool triggered;

void setup() {
  Wire.begin();
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  //  FastLED.setBrightness(20);
  Serial.begin(115200);
  FastLED.show();
}


void loop() {
  ReadWire();
  checkIfIdle();
  if (idle) {
    IdleState();
  } else {
    CheckButtons();
  }
}

void checkIfIdle() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    idleButtonCalc += button[i].state;
  }
  if (idleButtonCalc == 0) {
    if (!triggered) {
      triggered = true;
      timeSinceButtonPressed = millis();
    }
  } else {
    idle = false;
    timeSinceButtonPressed = millis();
  }
  Serial.println("Idle Timer: " + String(millis() - timeSinceButtonPressed));
  if (millis() - timeSinceButtonPressed > 5000 and !idle) {
    idle = true;
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    Serial.println("IDLING");
  }
}

void IdleState() {

  if (idleChainLED == sections[0].numLEDS + 1) {
    idleChainLED = 0;
  }
  leds[idleChainLED] = colour.idle;
  leds[idleTemp] = CRGB::Black;
  idleTemp = idleChainLED;
  idleChainLED += 1;
  FastLED.show();
  delay(200);
}

void CheckButtons() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    if (button[i].state == 0 and button[i].held == 1) {
      button[i].held = 0;
    }
    if (button[i].state == 1 and button[i].lit == false  and button[i].held == 0) {
      button[i].held = 1;
      for (int q = 0; q < NUM_BUTTONS; q++) {
        button[q].lit = false;
      }
      button[i].lit = true;
      fill_solid(leds, NUM_LEDS, CRGB::Black);    //Turn LEDS OFF

      leds[button[i].ledIndex] = colour.buttonOn;  //TURN ON BUTTON PUSHED LED
      FastLED.show();

      parentID = button[i].sectionParent;
      parentBreakPointTemp = sections[button[i].sectionParent].numLEDS;
      while (parentID != 99) {
        for (int x = 0; x < 18; x++) {
          if (parentID == sections[x].id) {
            FillSection(sections[parentID].ledIndex, parentBreakPointTemp != 99 ? parentBreakPointTemp : sections[parentID].numLEDS, colour.before, true, DEFAULTDELAY); //TURN ON SECTION LEDS BEFORE

            if (sections[parentID].leftButtonID != 99) {
              leds[button[sections[parentID].leftButtonID].ledIndex] = colour.buttonBefore; //TURN ON BUTTON LEDS BEFORE
              FastLED.show();
            }

            parentBreakPointTemp = sections[parentID].parentBreakPoint;
            parentID = sections[parentID].parentID;
            break;
          }
        }
      }

      int NumChildren = 0;
      int readAddr = 0;

      for (int y = 0; y < 100; y++) {
        EEPROM.write(y, 99);
      }

      int childID = button[i].sectionChild;

      if (childID != 99) {

        FillSection(sections[childID].ledIndex, sections[childID].numLEDS, colour.after, false, DEFAULTDELAY);
        if (sections[childID].rightButtonID != 99) {
          leds[button[sections[childID].rightButtonID].ledIndex] = colour.buttonAfter;
          FastLED.show();
        }

        for ( int l = 0; l < 5; l++) {

          if (sections[childID].childID[l] != 99) {
            EEPROM.write(readAddr, sections[childID].childID[l]);
            Serial.println("wrote child:" + String(sections[childID].childID[l]) + " to: " + String(readAddr));
            NumChildren += 1;
            readAddr += 1;
          }
        }

        readAddr = 0;
        for (int i = 0; i < NumChildren; i++) {

          childID = EEPROM.read(readAddr); //get new child
          EEPROM.write(readAddr, 99);
          Serial.println(String(readAddr) + ": read EEPROM, Lighting child " + String(childID));
          Serial.println("Total num of children: " + String(NumChildren));
          Serial.println("i: " + String(i));

          FillSection(sections[childID].ledIndex, sections[childID].numLEDS, colour.after, false, DEFAULTDELAY);
          if (sections[childID].rightButtonID != 99) {
            leds[button[sections[childID].rightButtonID].ledIndex] = colour.buttonAfter;
            FastLED.show();
          }
          readAddr += 1;
          int addrOffset = 0;

          for ( int l = 0; l < 5; l++) {
            if (sections[childID].childID[l] != 99) {
              Serial.println("found extra child: " + String(sections[childID].childID[l]));
              while (EEPROM.read(addrOffset) != 99) {
                addrOffset += 1;
              }
              EEPROM.write(addrOffset, sections[childID].childID[l]);
              Serial.println("wrote child: " + String(sections[childID].childID[l]) + " to: " + String(addrOffset));
              NumChildren += 1;
              addrOffset += 1;
            }
          }
          readAddr = 0;
          while (EEPROM.read(readAddr) == 99) {
            Serial.println("moving Address" + String(readAddr));
            readAddr += 1;
            if (readAddr > 20) {
              break;
            }
          }
        }
      }

    } else if (button[i].state == 1 and button[i].lit == true and button[i].held == 0) {  //TURN OFF LEDS WHEN SAME BUTTON PUSHED TWICE
      button[i].held = 1;
      button[i].lit = false;
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
    }
  }
}

void FillSection(int index, int num, CRGB colour, bool reverse, int delayTime) {
  for (int l = 0; l < num; l++) {
    leds[reverse ? index + num - 1 - l : index + l] = colour;
    delay(delayTime);
    FastLED.show();
  }
}

void ReadWire() {
  Wire.requestFrom(ttp229, 2, true);
  while (Wire.available())
  {
    data1 = Wire.read();
    data2 = Wire.read();
    data_out = (data1 << 8) | data2;
    //    for (int i = NUM_BUTTONS - 1; i > -1; i--) {
    for ( int i = 0; i < NUM_BUTTONS; i++) {
      button[i].state = bitRead(data_out, (NUM_BUTTONS - 1) - i);
      Serial.print(button[i].state);
    }
  }
  Serial.println("");
}

