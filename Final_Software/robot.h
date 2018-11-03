
#include <iostream>

using namespace std;

#include <robot_instr.h>
#include <robot_link.h>
#include <stdlib.h>     //for using the function sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include <cmath>

#define ROBOT_NUM 14   // The id number (see below)

// Board_0 pin allocations
#define middle 0
#define left 1
#define back 2
#define right 3
#define white_LED 0b0100000  // Value to write white LED
#define blue_LED 0b0010000   // Value to write blue LED
#define LED_off 0b0110000    // Turn LEDs off
#define front_switch 6

// Board_1 pin allocations - note LEDs are ON when LOW (=0) and actuators are ON when HIGH (=1)
#define multi_parcel 0b0001111
#define d2           0b0010111
#define lift_actuator 0b0111111
#define push_actuator 0b1011111

// Package indicators
#define RED         0b0011110   // 1
#define GREEN       0b0011101   // 2
#define WHITE       0b0011100   // 3
#define WOOD        0b0011011   // 4
#define TRANSPARENT 0b0011010   // 5

// ADC allocations
#define light_sensor ADC4
#define dist_sensor ADC3

#define ramp_speed 255

int truck, restart, again, with_package, lost_line_reverse(),back_junction, b,junction, package_on, a,board_0, board_1, left_speed, right_speed, junction_no, parcel_num(), package_type(), package_received(), line_follow(),lost_line();
bool sensor1[7], sensor2[7], junction_detected, package[10], delivery_point;
void read_sensors_secondary(), dropoff_truck(), read_sensors(), turn_left(), turn_right(), turn_around(), rth(), dropoff(), pickup_package(), pickup();
	
robot_link rlink;                      					// datatype for the robot link
stopwatch watch10;                       					// setup watch
