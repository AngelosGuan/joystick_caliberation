#include <p33Fxxxx.h>
#define FCY 12800000UL 
#include <stdio.h>
#include <libpic30.h>
#include "lcd.h"
#include "types.h"

#include "joystick.h"


void joystick_init()
{
    // INIT AD2C
    CLEARBIT(AD2CON1bits.ADON);
    SETBIT(TRISBbits.TRISB4);
    SETBIT(TRISBbits.TRISB5);
    CLEARBIT(AD2PCFGLbits.PCFG4); //set TRISE RE8 to input
    CLEARBIT(AD2PCFGLbits.PCFG5); //set TRISE RE8 to input
    SETBIT(AD2CON1bits.AD12B); //set 10b Operation Mode
    AD2CON1bits.FORM = 0b10; // 0b10 == float; 0b01 == int; 0b00 == unsigned int
    AD2CON1bits.SSRC = 0x7; //set automatic conversion
    AD2CON2 = 0; //not using scanning sampling
    CLEARBIT(AD2CON3bits.ADRC);
    AD2CON3bits.SAMC = 0x1F; //sample-to-conversion clock = 31Tad
    AD2CON3bits.ADCS = 0x2; //Tad = 3Tcy (Time cycles)
    SETBIT(AD2CON1bits.ADON);
    
    // setup trigger
    AD1PCFGHbits.PCFG20 = 1;
    SETBIT(TRISEbits.TRISE8);
}

// check if button is pressed by reading register
static bool joystick_getTriggeredRaw()
{    
    return PORTEbits.RE8 == 0;
}


// debouncing step
bool joystick_getTriggered()
{
    int i;
    bool pressed = true;
    // check for 5 consecutive pressed signal
    for (i = 0; i < 5; i++) {
        if (!joystick_getTriggeredRaw()) {
            pressed = false;
        }
        __delay_ms(1);
    }
    return pressed;
}

// read output from joystick via ADC
float static readFromAD2(int foo)
{
    AD2CHS0bits.CH0SA = foo;
    SETBIT(AD2CON1bits.SAMP);
    while(!AD2CON1bits.DONE);
    CLEARBIT(AD2CON1bits.DONE);
    return ADC2BUF0;
}

// read x-axis output from joystick at 0x004 bit
float joystick_getX()
{
    return readFromAD2(0x004);
}

// read y-axis output from joystick at 0x005 bit
float joystick_getY()
{
    return readFromAD2(0x005);
}
