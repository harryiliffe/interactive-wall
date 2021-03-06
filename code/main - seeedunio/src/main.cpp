#include <main.h>

hubs hub[NUM_HUBS];

// int buttonID; int nodeID; bool active; bool state; bool registered;  int parentID[3]; int childID[3]; int parentSectionID; int childSectionID;


buttons button[NUM_HUBS*NUM_BUTTONS] = {
  {0, 0, 1, 0, 0, 0, 0, 8, {3,-1,-1}, {1,-1,-1}, -1, -1},
  {1, 0, 0, 0, 0, 0, 8, 6, {0,-1,-1}, {2,-1,-1}, 4, 4},
  {2, 0, 0, 0, 0, 0, 14, 6, {1,-1,-1}, {-1,-1,-1}, -1, -1},
  {3, 0, 0, 0, 0, 0, 20, 6, {-1,-1,-1}, {0,-1,-1}, 0, 0},
  {4, 0, 0, 0, 0, 0, 26, 6, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {5, 0, 0, 0, 0, 0, 32, 6, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {6, 0, 0, 0, 0, 0, 38, 6, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {7, 0, 0, 0, 0, 0, 44, 6, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {8, 0, 0, 0, 0, 0, 50, 6, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {9, 0, 0, 0, 0, 0, 56, 6, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {10, 0, 0, 0, 0, 0, 62, 6, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  {11, 0, 0, 0, 0, 0, 68, 6, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  //
  // {12, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  // {13, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  // {14, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  // {15, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  // {16, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  // {17, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  // {18, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  // {19, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  // {20, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  // {21, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  // {22, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1},
  // {23, 1, 1, 0, 0, {-1,-1,-1}, {-1,-1,-1}, -1, -1}
};

//  ColourStates CRGB buttonBefore; CRGB buttonAfter;  CRGB buttonOn;  CRGB idle; CRGB off;
ColourStates colour = {CRGB::Green, CRGB::Blue, CRGB::White, CRGB::White, CRGB::Black};



void setup(){
  Serial.begin(115200);
  pinMode(CONFIGARATIONPIN, INPUT_PULLUP);

  initCapSensors();
  initLEDS();
}

void initLEDS(){
  FastLED.setBrightness(255);
  if(NUM_HUBS>0){
    FastLED.addLeds<WS2812B,LEDPIN1, GRB>(hub[0].leds, (BUTTONS_PER_HUB*LEDS_PER_BUTTON));
    if(DEBUG){Serial.println("Initialising LEDs on HUB 0");}
    for(int i =0;i<BUTTONS_PER_HUB*LEDS_PER_BUTTON;i++){
      // if(DEBUG){Serial.println(i);}
      hub[0].leds[i] = CRGB::Red;
      FastLED.delay(20);
      hub[0].leds[i] = CRGB::Black;
      FastLED.show();
    }
  }
  if(NUM_HUBS>1){
    FastLED.addLeds<WS2812B,LEDPIN2, GRB>(hub[1].leds, (BUTTONS_PER_HUB*LEDS_PER_BUTTON));
    if(DEBUG){Serial.println("Initialising LEDs on HUB 1");}
    for(int i =0;i<BUTTONS_PER_HUB*LEDS_PER_BUTTON;i++){
      hub[1].leds[i] = CRGB::Red;
      FastLED.delay(20);
      hub[1].leds[i] = CRGB::Black;
      FastLED.show();
    }
  }
  if(NUM_HUBS>2){
    FastLED.addLeds<WS2812B,LEDPIN3, GRB>(hub[2].leds, (BUTTONS_PER_HUB*LEDS_PER_BUTTON));
  }
  if(NUM_HUBS>3){
    FastLED.addLeds<WS2812B,LEDPIN4, GRB>(hub[3].leds, (BUTTONS_PER_HUB*LEDS_PER_BUTTON));
  }
  if(NUM_HUBS>4){
    Serial.println("Unable to init more leds due to limitations on the code");
  }
}

void initCapSensors(){
  if(DEBUG){Serial.println("Initialising MPR121 Capacitive Touch sensors");}
  bool flag = false;
  for(int i=0;i<NUM_HUBS;i++){
    if (!touchSensor[i].begin(capSensorAddr[i])) {
      if(DEBUG){
        Serial.print("MPR121 at ");
        Serial.print(capSensorAddr[i],16);
        Serial.println(" not found, check wiring?");
      }
      flag=true;
    } else if(DEBUG){
      Serial.print("MPR121 at ");
      Serial.print(capSensorAddr[i],16);
      Serial.println(" found!");
    }
  }
  while (flag);
}


void loop(){
  // if(DEBUG){Serial.println("LOOPING");}
  delay(40);
  mode = checkMode();
  getButtonStates();
  if(mode == M_CONFIG){
    configure();
  }
  FastLED.show();
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
    digitalWrite(22, HIGH);
    return  M_CONFIG;
  } else {
    digitalWrite(22, LOW);
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
      fill_all(true);
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
        lightParent(configureButton, colour.buttonBefore, true, false, -1);
        lightButton(configureButton, colour.buttonOn);

      } else if (configureParent == configureButton){
        configureMode = C_INACTIVE;
        asked = false;
        Serial.println("Finished Selecting Parents.");
      } else if (configureParent != -1){
        Serial.print("Parent Array: [");
        configureAdded = false;
        for(int i = 0; i<MAX_PARENT_CHILD; i++){
          if (button[configureButton].parentID[i] == configureParent && button[configureButton].parentID[i] != -1){ //remove button if pushed twice
            for(int l = 0; l<MAX_PARENT_CHILD; l++){
              if (button[configureParent].childID[l] == configureParent){
                button[configureParent].childID[l] = -1;
                break;
              }
            }
            lightButton(configureParent, colour.off);
            button[configureButton].parentID[i] = -1;
            configureAdded = true;
          }
        }
        for(int i = 0; i<MAX_PARENT_CHILD; i++){
          if (button[configureButton].parentID[i] == -1 && !configureAdded){ //add buttonID to parentArray
            for(int l = 0; l<MAX_PARENT_CHILD; l++){
              if (button[configureParent].childID[l] == -1){
                button[configureParent].childID[l] = configureButton;
                break;
              }
            }

            button[configureButton].parentID[i] = configureParent;
            lightParent(configureButton, colour.buttonBefore, true, false, -1);
            configureAdded = true;
          }
          Serial.print(button[configureButton].parentID[i]);
          Serial.print(", ");
        }

        configureParent = -1;
        Serial.println("]");
      }
      // lightButton(configureButton, colour.buttonOn);
      // lightParent(configureButton, colour.buttonBefore, true, false);
      break;
  }
}


void updateButtons(int buttonID, bool pushed){
  switch(mode){
    case M_INTERACTIVE:
      if(pushed){
        fill_all(false);
        button[buttonID].on = true;
        lightButton(buttonID, colour.buttonOn);
        lightParent(buttonID, colour.buttonBefore, true, true, -1);
        lightChild(buttonID, colour.buttonAfter, true, true, -1);
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
              configureMode = C_INACTIVE;
              asked = false;
              Serial.println("Finished Selecting Parents.");
            } else {
              configureParent = buttonID;
            }
            break;
        }
      break;
    }
  }
}

void lightParent(int buttonID, CRGB colour, bool first, bool iteritive, int calledFrom){
  if(!first){ //light current button
    if(button[buttonID].ledBar){
      lightBar(buttonID, colour, -1, button[calledFrom].childBarEnd);
    } else {
      lightButton(buttonID, colour);
    }
    // if(DEBUG){Serial.print("Lighting Parent: ");Serial.println(buttonID);}
  }
  for(int l=0; l<MAX_PARENT_CHILD;l++){
    if (button[buttonID].parentID[l] != -1){
      if(iteritive){
        lightParent(button[button[buttonID].parentID[l]].ID, colour, false, true, buttonID);
      } else {
        lightButton(button[button[buttonID].parentID[l]].ID, colour);
      }
    }
  }
}

void lightChild(int buttonID, CRGB colour, bool first, bool iterative, int calledFrom){
  if(!first){  //light current button
    if(button[buttonID].ledBar){
      lightBar(buttonID, colour, button[calledFrom].parentBarStart, -1);
    } else {
      lightButton(buttonID, colour);
    }
    // if(DEBUG){Serial.print("Lighting Child: ");Serial.println(buttonID);}
  }
  for(int l=0; l<MAX_PARENT_CHILD;l++){
    if (button[buttonID].childID[l] != -1){
      if(iterative){
        lightChild(button[button[buttonID].childID[l]].ID, colour, false, true, buttonID);
      } else {
        lightButton(button[button[buttonID].childID[l]].ID, colour);
        if(button[button[buttonID].childID[l]].ledBar){
          lightBar(buttonID, colour, button[buttonID].parentBarStart, -1);
        }
      }
    }
  }
}

void lightButton(int buttonID, CRGB colour){
  if(DEBUG){Serial.print("Lighting Button: ");Serial.println(buttonID);}
  for(int i=0;i<button[buttonID].numLEDS;i++){
    hub[button[buttonID].nodeID].leds[button[buttonID].ledIndex+i] = colour;
  }
  FastLED.show();
}

void lightBar(int buttonID, CRGB colour, int start, int end){
  if(end==-1){
    for(int l=0;l<MAX_PARENT_CHILD;l++){
      if (button[buttonID].childID[l] != -1 && button[button[buttonID].childID[l]].childBarEnd>end){
        end = button[button[buttonID].childID[l]].childBarEnd;
      }
    }
  }
  if(start==-1){
    for(int l=0;l<MAX_PARENT_CHILD;l++){
      if (button[buttonID].parentID[l] != -1 && button[button[buttonID].parentID[l]].parentBarStart>start){
        start = button[button[buttonID].parentID[l]].parentBarStart;
      }
    }
  }
  if(DEBUG){Serial.print("Lighting Bar: ");Serial.print(buttonID);Serial.print("   From ");Serial.print(start);Serial.print(" to ");Serial.println(end);}
  for(int i=start;i<end;i++){
    hub[button[buttonID].nodeID].leds[button[buttonID].ledIndex+i] = colour;
  }
  FastLED.show();
}

void flashParent(int buttonID, CRGB colour, bool first, bool iteritive){
  if(!first){ //light current button
    lightButton(buttonID, colour);
    // if(DEBUG){Serial.print("Lighting Parent: ");Serial.println(buttonID);}
  }
  for(int l=0; l<MAX_PARENT_CHILD;l++){
    if (button[buttonID].parentID[l] != -1){
      if(iteritive){
        flashParent(button[button[buttonID].parentID[l]].ID, colour, false, true);
      } else {
        flashButton(button[button[buttonID].parentID[l]].ID, colour);
      }
    }
  }
}

void flashChild(int buttonID, CRGB colour, bool first, bool iterative){
  if(!first){   //light current button
    lightButton(buttonID, colour);
    // if(DEBUG){Serial.print("Lighting Child: ");Serial.println(buttonID);}
  }
  for(int l=0; l<MAX_PARENT_CHILD;l++){
    if (button[buttonID].childID[l] != -1){
      if(iterative){
        flashChild(button[button[buttonID].childID[l]].ID, colour, false, true);
      } else {
        flashButton(button[button[buttonID].childID[l]].ID, colour);
      }
    }
  }
}

void flashButton(int buttonID, CRGB colour){
  for(int i=0;i<button[buttonID].numLEDS;i++){
    if(millis() - hub[button[buttonID].nodeID].flashTimer[button[buttonID].ledIndex+i] > 300){
      hub[button[buttonID].nodeID].flashTimer[button[buttonID].ledIndex+i] = millis();
      if(hub[button[buttonID].nodeID].leds[button[buttonID].ledIndex+i] == colour){
        hub[button[buttonID].nodeID].leds[button[buttonID].ledIndex+i] = CRGB::Black;
      } else {
        hub[button[buttonID].nodeID].leds[button[buttonID].ledIndex+i] = colour;
      }
    }
  }
  FastLED.show();
}

void fill_all(bool show){
  for(int i=0;i<NUM_HUBS;i++){
    fill_solid(hub[i].leds, NUM_BUTTONS*LEDS_PER_BUTTON, CRGB::Black);
  }
  if(show){FastLED.show();}
}
