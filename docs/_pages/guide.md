---
title: "Assembly Guide"
layout: default
---
## Assembly Guide

### Hardware

#### Casings

##### Compute Hub
##### Capacitive Hub
##### Button Modules
##### Led Bar

#### Circuit Boards
To create the circuit boards you will need access to a soldering iron.
We recommend getting the Button Module boards out-sourced, especially if you want 40 of them.
##### Compute Hub

To assemble the compute hub you will need
2x 13 pin female headers
1x DC Barrel Jack (2.1mm)
1x SMD Slide Switch
1x WEMOS ESP32 Lolin32 Lite
4x 6 pin male JST connector

Cut the circuit board using a mill and solder it all according to the eagle schematic.

##### Capacitive Hub
To assemble a Capacitive hub you will need

1x MPR121 Breakout Boards
1x 0 ohm resistor
1x 6 pin female header
1x 12 pin female header
12x 5 pin male JST connector
1x 6 pin male JST connector

Cut the circuit board using a mill and solder it all according to the eagle schematic.

##### Button Modules
To assemble a Button Module you will need

6x WS2812B LEDs
1x 5 pin male JST connector

Using a fabricated circuit board, solder on the components.
Note: Check the schematic/board layout to ensure you correctly orientate the LEDs.

##### Led Bar Modules

Each led bar has 8 circuit boards, each with a single LED on it, joint by wires.

Solder the LEDs onto each board and then wire them together.

The first module has the male JST connector on it to connect it back to the capacitive hub.

Pay special attention to the data out wire that comes from the last led back to the beginning.
#### Connectors
The connectors run between each of the modules and hubs. We recommend using the correct core cable to reduce cable clutter,

Cut and crimp the wires.

 **At this point you have all the hardware complete**. We recommend assembling it all now before moving on to the software.

### Software

The software is designed to run on a **Wemos Lolin32 Lite**. It is compiled through **atom/platform.io**.

Download the (code)[https://github.com/harryiliffe/interactive-wall/tree/master/code], and configure it to suit your interactive wall setup.

Key variables to change, such as the number of capacitive hubs, etc. are all in the main.h header file.

Flash the ESP using platform.io (atom) and plug it in the compute hub, as shown below.

Once plugged in and power via the 5v power supply. You should see all the LEDs light up very briefly when powering on the system. This indicates the boot has finished and you can begin using the interactive wall.

### Configuring

Through configuring the interactive wall you can create a tree like structure which highlights the information you intend.

Slide the switch just above the ESP on the Compute Hub. This will place you in Configuration Mode, as indicated the the blue led on the ESP.
Select a button you wish to configure and touch it. It should light up white.

Each button can have parents and children (with parents coming before and children coming after). This allows you to create a hierarchy.
When in configuration mode, we tell each button who there parents are by touching them. When you assign a parent it will go green.
To un-assign a parent from a button just touch the parent again.

Once you have selected the buttons parents, touch the button again to confirm.

Once you confirm, you will go back into configuration mode, so don't forget to flick that switch on the compute hub to go back into interactive mode.

When you first setup the wall, you'll want to go through each of the buttons, assigning their parents and creating that hierarchy.
