/*
  BlinkRGB.cpp - Library for RGB visual signal.
  Created by Edwin Kestler, May 29, 2018.
  Released into the public domain.
*/

#include "Arduino.h"
#include "BlinkRGB.h"

BlinkRGB::BlinkRGB(int pin){
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void BlinkRGB::On(){
  digitalWrite(_pin, HIGH);
}

void BlinkRGB::Off(){
  digitalWrite(_pin, LOW);
}

void BlinkRGB::Flash(){
  digitalWrite(_pin, HIGH);
  delay(250);
  digitalWrite(_pin, LOW);
}



