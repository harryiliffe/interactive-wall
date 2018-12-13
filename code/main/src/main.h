#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPR121.h>
#define FASTLED_ALLOW_INTERRUPTS 0 //NEEDED TO PREVENT ESP32 PANICING
#include <FastLED.h>


//Edit these

#define NUM_HUBS 1
#define NUM_BUTTONS 12*NUM_HUBS
#define LEDS_PER_BUTTON 8
#define MAX_PARENT_CHILD 3

#define DEBUG true


#define BUTTONS_PER_HUB 12

#define i2cSDA 19
#define i2cSCL 23

#define LEDPIN1 0
#define LEDPIN2 2
#define LEDPIN3 15
#define LEDPIN4 13

#define CONFIGARATIONPIN 32

//CONFIGURE MODES
#define C_INACTIVE 0
#define C_WAITING 1
#define C_PARENTS 2
#define C_CHILDREN 3

//MOOD DEFINITIONS
#define M_INTERACTIVE 0
#define M_CONFIG 1


//PIN CONFIGARATIONS
int capSensorAddr[4] = {0x5A, 0x5B, 0x5C, 0x5D}; // Default address is 0x5A, if tied to 3.3V its 0x5B. If tied to SDA its 0x5C and if SCL then 0x5D

//Start UP Mode
int mode = M_INTERACTIVE;

//Init the Capacitive Touch Sensors
uint16_t tempCapSensorBits[NUM_HUBS];
Adafruit_MPR121 touchSensor[NUM_HUBS] = Adafruit_MPR121();

//Configuration variables
int configureButton;
int configureParent;
int configureChild;
bool configureAdded;

int configureMode = 0;
bool asked;

//DEFINE STRUCTURES
struct hubs {
  CRGB leds[BUTTONS_PER_HUB*LEDS_PER_BUTTON];
  unsigned long flashTimer[BUTTONS_PER_HUB*LEDS_PER_BUTTON];
};

struct buttons {
  int ID;
  int nodeID;

  bool ledBar;

  bool on;
  bool state;
  bool registered;
  // CRGB colours[LEDS_PER_BUTTON];

  int ledIndex;
  int numLEDS;

  int parentID[MAX_PARENT_CHILD];
  int childID[MAX_PARENT_CHILD];
  int parentBarStart;
  int childBarEnd;
};

struct ColourStates {
  CRGB buttonBefore;
  CRGB buttonAfter;
  CRGB buttonOn;
  CRGB idle;
  CRGB off;
};


void setup();

void initCapSensors();
void initLEDS();

void configure();

void loop();


void getButtonStates();
int checkMode();


void updateButtons(int buttonID, bool pushed);

void lightParent(int buttonID, CRGB colour, bool first, bool iteractive, int calledFrom);
void lightChild(int buttonID, CRGB colour, bool first,bool iteractive, int calledFrom);
void lightButton(int buttonID, CRGB colour);

void lightBar(int buttonID, CRGB colour, int start, int end);

void flashParent(int buttonID, CRGB colour, bool first, bool iteractive);
void flashChild(int buttonID, CRGB colour, bool first, bool iteractive);
void flashButton(int buttonID, CRGB colour);

void fill_all(bool show);
