#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#include <stdio.h>
#include <libpic30.h>
#include <assert.h>
#include <math.h>

#include <limits.h>

#include "lcd.h"
#include "joystick.h"
#include "table.h"

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT); 

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);  

// update and keep a decaying average by giving a larger weight to the old value and a less weight to the new value
static inline float updateDecayingAverage(float old, float new)
{
    static float fracOld = 0.8;
    return old*fracOld + (1-fracOld) * new;
}

int main() {
	//Init LCD
	__C30_UART=1;	
	lcd_initialize();
	lcd_clear();
	lcd_locate(0,0);
    joystick_init();
    table_init();
    
    // set table to be leveled initially
    table_setX(0.5);
    table_setY(0.5);
    
    // caliberate max value on x-axis of joystick
    while(joystick_getTriggered());
    float xMax;
    while(!joystick_getTriggered()) {
        __delay_ms(10);
        lcd_locate(0,0);
        xMax = joystick_getX();
        lcd_printf("xMax = %07lld", (long long) xMax);
    }
    
    // caliberate min value on x-axis of joystick
    while(joystick_getTriggered());
    float xMin;
    while(!joystick_getTriggered()) {
        __delay_ms(10);
        lcd_locate(0, 1);
        xMin = joystick_getX();
        lcd_printf("xMin = %07lld", (long long) xMin);
    }
    
    // caliberate max value on y-axis of joystick
    while(joystick_getTriggered());
    float yMax;
    while(!joystick_getTriggered()) {
        __delay_ms(10);
        lcd_locate(0, 2);
        yMax = joystick_getY();
        lcd_printf("yMax = %07lld", (long long) yMax);
    }
    
    // caliberate min value on y-axis of joystick
    while(joystick_getTriggered());
    float yMin;
    while(!joystick_getTriggered()) {
        __delay_ms(10);
        lcd_locate(0, 3);
        yMin = joystick_getY();
        lcd_printf("yMin = %07lld", (long long) yMin);
    }
    
    // assert the values make sense
    assert(xMin <= xMax);
    assert(yMin <= yMax);
    
    // jump label
    post_calibrate:
    
    // use x-axis on joystick to control x-axis of the table, stop in place when button is pressed
    while(joystick_getTriggered());
    float x = joystick_getX();
    while(!joystick_getTriggered()) {
        __delay_ms(10);
        x = updateDecayingAverage(x, joystick_getX());
        lcd_locate(0, 4);
        float scale = (x - xMin) / (xMax - xMin);
        scale = scale > 1 ? 1 : scale;
        scale = scale < 0 ? 0 : scale;
        lcd_printf("x = %08.0f", getMsHigh(scale)*1000);
        table_setX(scale);
    }
    

    // use y-axis on joystick to control y-axis of the table, stop in place when button is pressed
    while(joystick_getTriggered());
    float y = joystick_getY();
    while(!joystick_getTriggered()) {
        __delay_ms(10);
        y = updateDecayingAverage(y, joystick_getY());
        lcd_locate(0, 5);
        float scale = (y - yMin) / (yMax - yMin);
        scale = scale > 1 ? 1 : scale;
        scale = scale < 0 ? 0 : scale;
        lcd_printf("y = %08.0f", getMsHigh(scale)*1000);
        table_setY(scale);
    }

    // if button is pressed again, let user control the table with caliberated joystick again.
    while(joystick_getTriggered());
    while(!joystick_getTriggered());
    while(joystick_getTriggered());
    goto post_calibrate;
    
    return 0;
}
