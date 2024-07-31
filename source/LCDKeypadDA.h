#pragma once

#include <inttypes.h>

// A0, ADC0, PF0, pin 97, analog pin 0, digital pin 54
#define KEYPIN     A0

#define NUMKEYS     5
#define KEY_NONE   -1
#define KEY_RIGHT   0
#define KEY_UP      1
#define KEY_DOWN    2
#define KEY_LEFT    3
#define KEY_SELECT  4

#define ADNOKEY     1023

#define KEYLONG     150                 // in 10 ms

class LCDKeypad
{
public:
    LCDKeypad();
  
    bool digPress();
    int8_t getKey();

    bool keyPressed(int key);
    void waitKeyRelease();
};
