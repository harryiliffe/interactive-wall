#include <main.h>

hubs hub[NUM_HUBS];

// int buttonID; int nodeID; bool active; bool state; bool registered;  int parentID[3]; int childID[3]; int parentSectionID; int childSectionID;
// buttons button[NUM_HUBS*NUM_BUTTONS] = {
//   {0, 0, 1, 0, 0, {-1,-1,-1}, {1,-1,-1}, -1, -1},
//   {1, 0, 1, 0, 0, {0,-1,-1}, {2,-1,-1}, -1, -1},
//   {2, 0, 1, 0, 0, {1,-1,-1}, {3,-1,-1}, -1, -1},
//   {3, 0, 1, 0, 0, {2,-1,-1}, {4,-1,-1}, -1, -1},
//   {4, 0, 1, 0, 0, {3,-1,-1}, {5,-1,-1}, -1, -1},
//   {5, 0, 1, 0, 0, {4,-1,-1}, {6,-1,-1}, -1, -1},
//   {6, 0, 1, 0, 0, {5,-1,-1}, {7,-1,-1}, -1, -1},
//   {7, 0, 1, 0, 0, {6,-1,-1}, {8,-1,-1}, -1, -1},
//   {8, 0, 1, 0, 0, {7,-1,-1}, {9,-1,-1}, -1, -1},
//   {9, 0, 1, 0, 0, {8,-1,-1}, {10,-1,-1}, -1, -1},
//   {10, 0, 1, 0, 0, {9,-1,-1}, {11,-1,-1}, -1, -1},
//   {11, 0, 1, 0, 0, {10,-1,-1}, {-1,-1,-1}, -1, -1},
// };

buttons button[NUM_HUBS*NUM_BUTTONS] = {
  {0, 0, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {1, 0, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {2, 0, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {3, 0, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {4, 0, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {5, 0, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {6, 0, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {7, 0, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {8, 0, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {9, 0, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {10, 0, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {11, 0, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},

  {12, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {13, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {14, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {15, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {16, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {17, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {18, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {19, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {20, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {21, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {22, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {23, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1}
};

ColourStates colour = {0x003e00, 0x310031, CRGB::Green, CRGB::MediumPurple, CRGB::White, CRGB::White, CRGB::Black};



void setup(){
  Serial.begin(115200);
  pinMode(CONFIGARATIONPIN, INPUT_PULLDOWN);
  initLEDS();
  initCapSensors();

}

void initLEDS(){
  if(NUM_HUBS>0){
    FastLED.addLeds<WS2812B,LEDPIN1, GRB>(hub[0].leds, (BUTTONS_PER_HUB*LEDS_PER_BUTTON));
    if(DEBUG){Serial.println("Initialising LEDs on HUB 0");}
    for(int i =0;i<BUTTONS_PER_HUB*LEDS_PER_BUTTON;i++){
      // if(DEBUG){Serial.println(i);}
      hub[0].leds[i] = CRGB::Red;
      FastLED.delay(10);
      FastLED.show();
    }
  }
  if(NUM_HUBS>1){
    FastLED.addLeds<WS2812B,LEDPIN2, GRB>(hub[1].leds, (BUTTONS_PER_HUB*LEDS_PER_BUTTON));
    if(DEBUG){Serial.println("Initialising LEDs on HUB 1");}
    for(int i =0;i<BUTTONS_PER_HUB*LEDS_PER_BUTTON;i++){
      hub[1].leds[i] = CRGB::Red;
      FastLED.delay(10);
      FastLED.show();
    }
  }
  if(NUM_HUBS>2){
    FastLED.addLeds<WS2812B,LEDPIN3, GRB>(hub[2].leds, (BUTTONS_PER_HUB*LEDS_PER_BUTTON));
  }
  if(NUM_HUBS>3){
    FastLED.addLeds<WS2812B,LEDPIN4, GRB>(hub[3].leds, (BUTTONS_PER_HUB*LEDS_PER_BUTTON));
  }
  if(NUM_HUBS>3){
    Serial.println("Unable to init more leds due to limitations on the code");
  }
}

void initCapSensors(){
  if(DEBUG){Serial.println("Initialising MPR121 Capacitive Touch sensors");}
  bool flag = false;
  for(int i=0;i<NUM_HUBS;i++){
    if (!touchSensor[i].begin(capSensorAddr[i],i2cSDA,i2cSCL)) {
      if(DEBUG){
        Serial.print("MPR121 at ");
        Serial.print(capSensorAddr[i],16);
        Serial.println(" not found, check wiring?");
      }
      flag=true;
    }
    if(DEBUG){
      Serial.print("MPR121 at ");
      Serial.print(capSensorAddr[i],16);
      Serial.println(" found!");
    }
  }
  while (flag);
}


void loop(){
  // if(DEBUG){Serial.println("LOOPING");}
  delay(200);
  mode = checkMode();
  getButtonStates();
  if(mode == M_CONFIG){
    configure();
  }
}

void getButtonStates(){
  // Get the currently touched buttons
  for(int i=0;i<NUM_HUBS;i++){
    tempCapSensorBits[i] = touchSensor[i].touched();
  }
  for(int l=0; l<NUM_HUBS;l++){
    for (int i=0; i<BUTTONS_PER_HUB; i++) {
      //set button state to currently touched buttons
      button[(l*BUTTONS_PER_HUB)+i].state =  (tempCapSensorBits[l] & _BV(i) );

      if (button[(l*BUTTONS_PER_HUB)+i].state && !button[(l*BUTTONS_PER_HUB)+i].registered ) {
        if(DEBUG){Serial.print((l*BUTTONS_PER_HUB)+i); Serial.println(" touched");}
        button[(l*BUTTONS_PER_HUB)+i].registered = true;

        updateButtons((l*BUTTONS_PER_HUB)+i, true);
      }
      if (!button[(l*BUTTONS_PER_HUB)+i].state && button[(l*BUTTONS_PER_HUB)+i].registered ) {
        if(DEBUG){Serial.print((l*BUTTONS_PER_HUB)+i); Serial.println(" released");}

        updateButtons((l*BUTTONS_PER_HUB)+i, false);
        button[(l*BUTTONS_PER_HUB)+i].registered = false;
      }
    }
  }
}

int checkMode(){
  if(digitalRead(CONFIGARATIONPIN)){
    return  M_CONFIG;
  } else {
    return M_INTERACTIVE;
  }
}

void configure(){
  // Serial.println("CONFIG");
  switch(configureMode){
    case C_INACTIVE:
      configureMode = C_WAITING;
      asked = false;
      configureButton = -1;
      configureChild = -1;
      configureParent = -1;
      break;
    case C_WAITING:
      if(!asked){
        Serial.println("Click the button you would like to configure.");
        asked = true;
      } else if(configureButton != -1){
        Serial.print("Now Configuring button: ");
        Serial.println(configureButton);
        configureMode = C_PARENTS;
        asked = false;
      }
      break;
    case C_PARENTS:
      if(!asked){
        Serial.println("Click the parent buttons now. And click the main button to confirm.");
        asked = true;
      } else if (configureParent != -1){
        configureAdded = false;
        Serial.print("Parent Array: [");
        for(int i = 0; i<MAX_PARENT_CHILD; i++){
          if (button[configureButton].parentID[i] == configureParent){ //remove button if pushed twice
            button[configureButton].parentID[i] = -1;
            configureChild = -1;
          } else if (button[configureButton].parentID[i] == -1 && !configureAdded){ //addd buttonID to parentArray
            configureAdded = true;
            button[configureButton].parentID[i] = configureParent;
            configureParent = -1;
          }
          Serial.print(button[configureButton].parentID[i]);
          Serial.print(", ");
        }
        Serial.println("]");
      }
      break;
    case C_CHILDREN:
      if(!asked){
        Serial.println("Click the child buttons now. And click the main button to confirm.");
        asked = true;
      } else if (configureChild != -1){
        configureAdded = false;
        Serial.print("Child Array: [");
        for(int i = 0; i<MAX_PARENT_CHILD; i++){
          if (button[configureButton].childID[i] == configureChild){ //remove button if pushed twice
            button[configureButton].childID[i] = -1;
            configureChild = -1;
          } else if (button[configureButton].childID[i] == -1 && !configureAdded){ //addd buttonID to parentArray
            configureAdded = true;
            button[configureButton].childID[i] = configureChild;
            configureChild = -1;
          }
          Serial.print(button[configureButton].childID[i]);
          Serial.print(", ");
        }
        Serial.println("]");
      }
      break;
  }
}


void updateButtons(int buttonID, bool pushed){
  switch(mode){
    case M_INTERACTIVE:
      if(pushed){
        lightButton(buttonID, colour.buttonOn);
        lightParent(buttonID, colour.buttonBefore, true);
        lightChild(buttonID, colour.buttonAfter, true);
      } else {
        lightButton(buttonID, colour.off);
        lightParent(buttonID, colour.off, true);
        lightChild(buttonID, colour.off, true);
      }
      break;
    case M_CONFIG:
      if(pushed){
        switch(configureMode){
          case C_WAITING:
            configureButton = buttonID;
            break;
          case C_PARENTS:
            if(buttonID == configureButton){ //confirm
              configureMode = C_CHILDREN;
              asked = false;
              Serial.println("Finished Selecting Parents.");
            } else {
              configureParent = buttonID;
            }
            break;
          case C_CHILDREN:
            if(buttonID == configureButton){ //confirm
              configureMode = C_INACTIVE;
              asked = false;
              Serial.println("Finished Selecting Children.");
            } else {
              configureChild = buttonID;
            }
            break;
          }
        }
      break;
  }
}

void lightParent(int buttonID, CRGB colour, bool first){
  if(!first){lightButton(buttonID, colour);
  if(DEBUG){Serial.print("Lighting Parent: ");Serial.println(buttonID);}} //light current button
  for(int l=0; l<MAX_PARENT_CHILD;l++){
    if (button[buttonID].parentID[l] != -1){
      lightParent(button[button[buttonID].parentID[l]].ID, colour, false);
    }
  }
}

void lightChild(int buttonID, CRGB colour, bool first){
  if(!first){lightButton(buttonID, colour);
  if(DEBUG){Serial.print("Lighting Child: ");Serial.println(buttonID);}} //light current button
  for(int l=0; l<MAX_PARENT_CHILD;l++){
    if (button[buttonID].childID[l] != -1){
      lightChild(button[button[buttonID].childID[l]].ID, colour, false);
    }
  }
}

void lightButton(int buttonID, CRGB colour){
  if(DEBUG){Serial.print("Lighting Button: ");Serial.print(buttonID);Serial.print(".   LED NUM: ");Serial.println((buttonID%BUTTONS_PER_HUB));}
  // fill_solid(&(hub[button[buttonID].nodeID].leds[buttonID%BUTTONS_PER_HUB]), LEDS_PER_BUTTON, colour);
  for(int i=0;i<LEDS_PER_BUTTON;i++){
    hub[button[buttonID].nodeID].leds[(buttonID%BUTTONS_PER_HUB)+i] = colour;
  }
  FastLED.show();
}
