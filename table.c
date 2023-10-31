#include "joystick.h"
#include <p33Fxxxx.h>
#include <assert.h>
#include <limits.h>
#include "types.h"
#include <libpic30.h>
#include "table.h"

/// APIs that control a physical table supported by servos
/// table_setX, table_setY sets the table's angle on x and y axis respectively by turning the servos

static unsigned int ccFromMs(double ms, unsigned int prescaler)
{
    double cc = ms*1e-3*12.8e6/prescaler; // compute period in clock cycles
    assert(cc < UINT_MAX);
    return (unsigned int) cc;
}

#define MS_MAX_LEN 2.1
#define MS_MIN_LEN 0.9

static int prescaler = 64;
static int msPeriod = 20;

void table_init()
{
    assert(MS_MIN_LEN <= MS_MAX_LEN);
    
    //setup Timer 2
    CLEARBIT(T2CONbits.TON);    // Disable Timer
    CLEARBIT(T2CONbits.TCS);    // Select internal instruction cycle clock
    CLEARBIT(T2CONbits.TGATE);  // Disable Gated Timer mode
    assert(prescaler == 64);
    T2CONbits.TCKPS = 0b10;     // Select 1:64 Prescaler
    CLEARBIT(IFS0bits.T2IF);    // Clear Timer2 interrupt status flag
    CLEARBIT(IEC0bits.T2IE);    // Disable Timer2 interrupt enable control bit
    PR2 = ccFromMs(msPeriod, prescaler);   // Set timer period
    TMR2 = 0;
    SETBIT(T2CONbits.TON);

    //setup OC8
    CLEARBIT(TRISDbits.TRISD7);   /* Set OC8 as output */
    OC8CON = 0x0006;              /* Set OC8: PWM, no fault check, Timer2 */
    
    //setup OC7
    CLEARBIT(TRISDbits.TRISD6);   /* Set OC8 as output */
    OC7CON = 0x0006;              /* Set OC8: PWM, no fault check, Timer2 */
}

double getMsHigh(float scale)
{
    assert(scale >= 0);
    assert(scale <= 1);
    return msPeriod - (MS_MIN_LEN + scale*(MS_MAX_LEN - MS_MIN_LEN));
}

void table_setX(float scale)
{
    double msHigh = getMsHigh(scale);
    
    OC8R = ccFromMs(msHigh, prescaler);    /* Set the initial duty cycle to 5ms*/
    OC8RS = ccFromMs(msHigh, prescaler);   /* Load OCRS: next pwm duty cycle */
}

void table_setY(float scale)
{
    double msHigh = getMsHigh(scale);
    
    OC7R = ccFromMs(msHigh, prescaler);    /* Set the initial duty cycle to 5ms*/
    OC7RS = ccFromMs(msHigh, prescaler);   /* Load OCRS: next pwm duty cycle */
}
