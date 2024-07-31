#include "LCDKeypadDA.h"

#include "Arduino.h"

uint16_t keyVals[NUMKEYS] = { 30, 150, 360, 535, 760 };           // official values

LCDKeypad::LCDKeypad()            
{
    pinMode(KEYPIN, INPUT);
}

// digital press check, senses KEY_RIGHT only
//
bool LCDKeypad::digPress()
{
    return digitalRead(KEYPIN) == 0;
}

// analog get key
//
int8_t LCDKeypad::getKey()
{
    uint16_t ad = analogRead(KEYPIN);
    int8_t k;
    
    for(k=0; k<NUMKEYS; k++)
        if(ad < keyVals[k])
            break;

    if(k == NUMKEYS)
        k = KEY_NONE;
        
    return k;    
}

bool LCDKeypad::keyPressed(int key){
    
    return getKey() == key;
}

void LCDKeypad::waitKeyRelease()
{
    while(analogRead(KEYPIN) < ADNOKEY)
        ;
}
