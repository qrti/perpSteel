// perpSteel V0.4, qrt@qland.de 240731

// KEY_RIGHT 
// press > 1.5 s to enter or exit time and delay settings
//
// KEY_SELECT
// change values in 100..500..1000 ms steps
//
// KEY_LEFT - / KEY_RIGHT +
// delay sensor OFF -> magnet ON
// 
// KEY_DOWN - / KEY_UP +
// magnet ON time
//
// the timing is saved to EEPROM after changes

// display
// line 1: status
//      2: sensor OFF -> magnet ON delay in ms, magnet ON time in ms, change value in ms

// calculation
// sensor ON to OFF time is measured in TIMER4_CAPT and used as reciprocal relative speed (Vr)
// Vr is averaged and weighted, it influences the sensor OFF -> magnet ON delay

// possible ICP pins on Arduino Mega 2560
// PIN      PORT    ICP     DIGITAL PIN     TIMER
// 35	    PL0     4       49              4
// 36	    PL1     5	    48              5

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "LCDKeypadDA.h"

// default values for this machine, might be different for other machines
#define DEFAULT_TDELAY       6800               // sensor OUT -> magent ON delay    in us   
#define DEFAULT_TMAGPO       6000               // magnet ON time
#define DEFAULT_TNAVG       11000               // sensor ON to OFF time (Vr)

#define EESIG          0x18ab11cd               // EEPROM unique ID   

void changePars();

void showTiming();
void saveTiming();
void loadTiming();

void init_tmr4();

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
LCDKeypad keypad;

const int PIN_LED = 13;                         // LED
const int PIN_KEY_IRQ = 2;                      // IRQ0 for keys

const int PIN_INDUCT_SENSE = 49;                // ICP 4, TIMER 4
const int PIN_MAGPOW = 53;                      // magnet power 

#define ON      HIGH
#define OFF     LOW

volatile int32_t tdelay, tmagpo, dt;           // in us
uint32_t tdelayC;

volatile int32_t tsense;
volatile uint8_t f_capture = 0;                // flag

char buf[32];
int32_t tnSum, tnAvg, tnAvc=16;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void setup()
{
    lcd.begin(16, 2);   
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("qrt 240731");

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, OFF);

    pinMode(PIN_INDUCT_SENSE, INPUT);

    pinMode(PIN_MAGPOW, OUTPUT);
    digitalWrite(PIN_MAGPOW, OFF);

    pinMode(A0, INPUT);

    loadTiming();

    delay(1000);
    lcd.clear();
    showTiming();

    init_tmr4();    
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint32_t tmD, tmM;

void loop() 
{
    lcd.setCursor(0, 0);
    sprintf(buf, "%6luus   ", tnAvg);
    lcd.print(buf);

    tmD = tmagpo / 1000;
    tmM = tmagpo % 1000;

    f_capture = 0;

    while(!f_capture && !keypad.digPress())
        ;

    if(keypad.digPress())
        changePars();

    if(!(TIFR4 & 1<<TOV4)){
        tdelayC = tdelay + (tsense - tnAvg) * tdelay / tnAvg;

        delay(tdelayC / 1000);
        delayMicroseconds(tdelayC % 1000);
        digitalWrite(PIN_MAGPOW, ON);

        delay(tmD);
        delayMicroseconds(tmM);
        digitalWrite(PIN_MAGPOW, OFF);

        tnSum += tsense - tnAvg;
        tnAvg = tnSum / tnAvc;        

        lcd.setCursor(0, 0);
        lcd.print("wait ...");
    }
    // else{
    //     lcd.setCursor(0, 0);
    //     lcd.print("fail    ");       
    // }

    delay(100);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void init_tmr4()
{  
    TCCR4A = 0;                                         // reset timer registers
    TCCR4B = 0;                                         //

    TCCR4B = 1<<ICNC4 | 0<<ICES4 | 1<<CS41;             // input noise canceler on, input capture negative edge, start T4 prescaler 8 (2 MHz)
    TIMSK4 = 1<<ICIE4;                                  // input capture IR enable 
}

ISR(TIMER4_CAPT_vect)
{
    if(TCCR4B & 1<<ICES4){                              // positive edge
        tsense = ICR4 >> 1;                             // low period in us
        TCCR4B = 1<<ICNC4 | 0<<ICES4 | 1<<CS41;         // negative edge next 
        f_capture = 1;                                  // flag capture
    }
    else{                                               // negative edge
        TCNT4 = 0;                                      // reset T4 counter
        TCCR4B = 1<<ICNC4 | 1<<ICES4 | 1<<CS41;         // positive edge next
        TIFR4 |= 1<<TOV4;                               // reset T4 OV IR
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void changePars()
{
    uint8_t pc = 0;                                     // 10 ms press counter

    while(keypad.digPress() && pc<KEYLONG){             // wait for key long press
        pc++;                                           //
        delay(10);                                      //
    }                                                   //                      

    if(pc < 150)                                        // if press was too short
        return;                                         // return

    lcd.setCursor(0, 0);                                // display message
    lcd.print("edit timing");                           //

    while(keypad.digPress())                            // wait until key is released
        delay(10);                                      //

    delay(250);                                         // debounce delay 

// - - - - - - - - - - - - - - - - - - -

    int8_t key=KEY_NONE, ckey;                          // key variables    
    uint8_t rc = 0;                                     // 10 ms release counter
    pc = 0;                                             //       press

    while(true){                                        // loop start
        ckey = keypad.getKey();                         // get current key

        if(ckey != KEY_NONE){                           // key pressed
            key = ckey;                                 // store key
            rc = 0;                                     // reset release counter
            if(pc < 255) pc++;                          // advance and restrict press counter
        }
        else{
            if(pc > 5){                                 // key released, min press time
                if(key == KEY_LEFT){
                    tdelay -= dt;
                    if(tdelay < 0) tdelay = 0;
                }
                else if(key == KEY_RIGHT){
                    tdelay += dt;
                }
                else if(key == KEY_DOWN){
                    tmagpo -= dt;
                    if(tmagpo < 0) tmagpo = 0;
                }
                else if(key == KEY_UP){
                    tmagpo += dt;
                }
                else if(key == KEY_SELECT){
                    if(dt == 100)   
                        dt = 500;
                    else if(dt == 500)
                        dt = 1000;
                    else if(dt == 1000)
                        dt = 100;
                }

                showTiming();
            }
            
            key = KEY_NONE;
            pc = 0;                                     // reset press counter
            if(rc < 255) rc++;                          // advance and restrict release counter
        }

        if(ckey==KEY_RIGHT && pc>KEYLONG)               // if key right long press
            break;                                      // exit loop

        delay(10);
    }

    lcd.setCursor(0, 0);                                // clear display
    lcd.print("save timing");                           //

    while(keypad.digPress())                            // wait until key is released
        delay(10);                                      //

    delay(250);                                         // debounce delay 

    saveTiming();                                       // save timing
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void showTiming()
{
    static char bufi[32];

    dtostrf((float)tdelay/1000., 5, 1, bufi);                 // float number, min width, precision, target string
    strcat(bufi, " ");
    dtostrf((float)tmagpo/1000., 5, 1, &bufi[strlen(bufi)]);   
    strcat(bufi, " ");
    dtostrf((float)dt, 4, 0, &bufi[strlen(bufi)]);  
    strcat(bufi, " ");

    lcd.setCursor(0, 1);
    lcd.print(bufi);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int eead;

void eePut(uint32_t v)
{
    EEPROM.put(eead, v);
    eead += sizeof(int32_t);
}

void saveTiming()
{   
    eead = 0;

    eePut(EESIG);
    eePut(tdelay);
    eePut(tmagpo);
    eePut(dt);
    eePut(tnAvg);
}

uint32_t eeGet()
{
    uint32_t v;

    EEPROM.get(eead, v);
    eead += sizeof(int32_t);
    return v;
}

void loadTiming()
{
    eead = 0;

    if(eeGet() == EESIG){
        tdelay = eeGet();
        tmagpo = eeGet();
        dt = eeGet();
        tnAvg = eeGet();
    }
    else{
        tdelay = DEFAULT_TDELAY;
        tmagpo = DEFAULT_TMAGPO;
        tnAvg = DEFAULT_TNAVG;
        dt = 500;            
    }
    
    tnSum = tnAvg * tnAvc;  
}
