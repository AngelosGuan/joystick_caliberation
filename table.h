#ifndef TABLE_H
#define TABLE_H

/*
 * Uses: timer 2, pin OC8/TRISD7,
 * 
 * Sets that pin high for msHigh milliseconds every msPeriod milliseconds.
 */
void table_init();

void table_setX(float scale);
void table_setY(float scale);
double getMsHigh(float scale);

#endif
