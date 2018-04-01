# hexbug-wemos
An hexbug toy hack with wemos D1 mini based "brain".

## Why

The classic toy comes with an infrared controller with just forward/backward and steering features.
I wanted to experiment with ultrasound sensor to enable autonomous navigaton features and I wanted to challenge myself in packing all the electronics in a well designed head, while keeping dimensions as compact as possible.

## Features

* Rechargeable battery
* Web based interface over WiFi
* Ultrasound sensor for obstacle detection
* RGB led for status & fun

## Components

* Hexbug spider toy (https://www.amazon.com/HEXBUG-451-1652-Hexbug-Spider-Red/dp/B004OLKANI?th=1)
* Wemos D1 Mini controller
* Wemos Motor Shield
* Wemos Battery Shield
* HC-SR04 Ultrasound sensor 
* RGB 5mm Led
* A piece of perfboard
* Rechargeable LiPo battery (1400mAh)
* Small 2mm self tapping screws 

## Tools required

* Solder iron and basic soldering skills and consumables
* 3D printer or a service (makerspace, etc)
* key tools such as screwdrivers, pliers, ...
* Arduino IDE


# Project status

## Comments & Known issues

Documentation is barely minimal.

### Electronics

You will need to make the cables coming out from the motors longer. Mark them (white=+, black=gnd).

### Software

Really basic, just useful for testing all components, Web interface can be barely given this name.
Non optimal handling of sync web request in the loop together with obstacle avoidance.

### 3D printed parts

Parts are quite fragile. 
Internal frame screws hard to reach and mount properly, very tigth internal dimensions. 

## Changelog

2018-04-01 First alfa release on GitHub. Everything is in test! 

