# joystick_caliberation

## Introduction
The goal of the project to use a joystick to control a table to balance a ball on top of it. The table is controlled with servo motor to adjust its angle in y-axis and x-axis. 
Before using the joystick to control the table, the joystick will be caliberated first by setting max value in x-axis, min value in x-axis, max value in y-axis, min value in y-axis in that order. 
To set a min/max value, the user should move the joystick in that direction, the current value will be displaced on a LCD screen, when the max/min position is reach, the user should press the button on the joystick 
and the current value of the joystick axis will stay on the LCD screen, and recorded as the max/min value. This value will be later used to determin the corresponding position to set for the servo controlling the table to a joystick position.

After caliberating the joystick, the user then can use the joystick to control the servo to balance the table by first adjusting the angle of the table on x-axis, 
press the button to indicate that they are done and the angle on axis will be fixed at that position, and then perform the same procedure on y-axis.

After the angles on both axises are fixed, pressing the button again can reset the table control and allow the user to set table angle again repeatedly.

![components](/img/table.bmp)

## physical components

- digital signal controller: dsPIC33F board
- LCD Screen
- servo-controlled table
- joystick

## code structure
--main.c
 -lcd
 -table
 -types.h
 -joystick

