---
title: "Electronics and Code"
layout: default
---

# Electronics

### Selecting a method
#### Node Based Micro-controllers
To allow for modularity the wall could be designed into sections each stemming off a node. Each node will contain a micro-controller and communicate with the base node.

![electronics3]({{site.imageurl}}/electronics3.jpg){: .img-fluid}
##### Proof of Concept
For a proof of concept, I used 3 Arduino Unos and wrote some [code (basic i2c)](https://github.com/harryiliffe/interactive-wall/tree/master/code/testing/basic%20i2c).
The Arduinos communicate through i2c which allows for 2 way communication for the LEDs and buttons.
![electronics1]({{site.imageurl}}/electronics1.jpg){: .img-fluid}
![electronics2]({{site.imageurl}}/electronics2.jpg){: .img-fluid}

The proof of concept worked really well and with a little bit more work I will be able to create an easy to read library for communicating between the Central Node and the Nodes.

#### Hub Based Micro-controllers
A different, cheaper way of designing the system would be to have a single micro-controller in the hub with wires running to each button to allow for the Buttons and LEDs. Although this will mean a lot of wires running from the hub it will make a number of things easier. Namely, reducing programming complexity, reducing costs and making the man

##### Proof of Concept
A major concern with this design is that the capacitive touch and LEDs will not work at the maximum distance. To test this I got a 4.5m wire.
![electronics4]({{site.imageurl}}/electronics4.jpg){: .img-fluid}
The Capacitive touch worked well. And the LEDs had no visible delay.

** The method we have selected is the hub based method **
We have a number of things we need to work out namely:
 * Where will all the wires we routed
 * Will there be too much voltage drop on the LED data line

Also in our testing, I discovered that the library for the proposed capacitive touch sensor (MPR121) does not work with the Wemos Lolin32 lite.
I have updated the library to allow compatibility.

### Working prototype

**We have selected the Hub Based Micro-controllers.**
I have made a fully realised breadboard version.
It contains and demonstrates all the necessary functions for the final circuit board such as:
* Registering and handling button presses and releases.
* Lighting LEDs which correspond to the buttons pushed.
* A configuration mode to allow for rapid reorganisation of the final.
* Multiply i2c capacitive touch modules that can be added for expandability.
* A mess of wires.

The next step is making a circuit board prototype.


## Button Circuit board
The button circuit board needs to:
* house the LEDs
* have a jst connector for modularity
* have a pad that we can attach the capacitive touch pad too.

A particular challenge has been deciding between a large single circuit board or multiply smaller ones. We knew that we wanted the LEDs at the top and bottom as well as around the circle in the middle but were stuck on how to achieve this. One option is to create a circuit board that covers the whole area but this uses a lot of unnecessary material. The other option is to create smaller circuit boards and join them with wires but this will take a much longer time.

## Control Module
The control modules needs to:

The first one was designed to use a esp32 dev board (specifically, the lolin32 lite). However, I think it would be better to use an atmega328p instead.

!(control board)[https://github.com/harryiliffe/interactive-wall/tree/master/boards/compute%20hub%20-%20atmega/exports/computehub.png]

{% include image.html id="control board" images="https://github.com/harryiliffe/interactive-wall/tree/master/boards/compute%20hub%20-%20atmega/exports/computehub.png" caption="The control board circuit" %} 
