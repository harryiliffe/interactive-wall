---
title: "Build Log"
layout: default

slider0: [paper1.jpg, paper2.jpg]
slider1: [paper6.jpg, paper3.jpg, paper4.jpg, paper5.jpg, paper7.jpg, paper8.jpg]
slider2: [buttonfelt.jpg, buttonshiny.jpg, buttoncopper.jpg]
---

# Build Log

What follows will hopefully be an unpretty flow of ideas that will eventually come together to form an interactive wall. Fingers crossed :)

## Goals

Some key goals for the project.
The finished project will
  * Communicates the sessions available
  * flexible, and easy to upgrade and change around in the future
  * be supported by full documentation, allowing for anyone to create their own interactive wall
  * be installed at Fab Lab WGTN

The interactive wall will be allow people to learn more about Fab Lab WGTN in an engaging yet comfortable way.

## User Journey

One of this projects main goals is to create a wall that allows people to learn more about Fab Lab WGTN in an engaging yet comfortable way.
Here at Fab Lab WGTN we see a lot of new people every year and these people, don't know the correct way to get involved and enter the space. The interactive wall aims to bridge that gap by laying out visibly some of the things you can do in the space.

## Paper Prototyping

<!-- ![paper1]({{site.imageurl}}/paper1.jpg){: .img-fluid}
![paper2]({{site.imageurl}}/paper2.jpg){: .img-fluid} -->
{% include imageslider.html id="slider0" url=site.imageurl images=page.slider0 %}


Above is some images of the paper prototyping we did.
Both Liz and I quite like the layout shown in the photo as it can be nicely divided into different sections.

### Button Style

Trying out different button styles.

{% include imageslider.html id="slider1" url=site.imageurl images=page.slider1 %}

The small dark circles are the pushable buttons.
A more interesting layout, much less grid like.
To add more information I could add a flap system.


### Full scale model



### More inviting

After talking to Wendy (Director of Fab Lab WGTN), we decided to rethink the button module. One issue I was having is that there is not enough capacitive change for the sensor to detect after covering the copper pad with vinyl.
We are going to re-explore the materials used, starting with a conductive material, ripstock.
Although the ripstock fused well to felt and created a more "pushable" button, we felt it looked a little tacky.
Stu presented us with some conduction shiny material, which, whilst cool, was a little bit too much for our plywood feel.
We settled on exposing the copper underneath the vinyl. This will solve to conductivity issues, whilst highlighting the buttons more.

{% include imageslider.html id="slider2" url=site.imageurl images=page.slider2 %}

### Image resizing

I have been resizing the images for this blog using sips on my mac.
I'm putting this command here so I don't lose it again. :)
> sips --resampleHeight 1000 *.jpg
> sips -p 1000 1333 --padColor ffffff *.jpg 
