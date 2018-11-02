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
#define white_LED 16  // Value to write white LED
#define blue_LED 32   // Value to write blue LED
#define front_switch 6

// Board_1 pin allocations
#define multi_parcel 8
#define delivery_indicator 16
#define lift_actuator 32
#define push_actuator 64

// Package indicators
#define RED 1
#define GREEN 2
#define WHITE 3
#define WOOD 4
#define TRANSPARENT 5

// ADC allocations
#define light_sensor ADC4
#define dist_sensor ADC3

#define ramp_speed 255

int delivery_point, restart, again, with_package, lost_line_reverse(),back_junction, b,junction, package_on, a,board_0, board_1, left_speed, right_speed, junction_no, parcel_num(), package_type(), package_received(), line_follow(),lost_line();
bool sensor1[7], sensor2[7], junction_detected, package[10];
void read_sensors_secondary(), dropoff_truck(), read_sensors(), turn_left(), turn_right(), turn_around(), rth(), dropoff_prepare(), pickup_package(), pickup();
	
robot_link rlink;                      					// datatype for the robot link
stopwatch watch;                       					// setup watch

/*
extern int a,board_0, board_1, left_speed, right_speed, junction_no, parcel_num(), package_type(), package_received(), line_follow(), pickup(),lost_line(), pickup_package();
extern bool sensor1[7], sensor2[7], junction_detected, package[10];
extern void turn_left(), turn_right(), turn_around(), rth(), junction(), dropoff_prepare();
extern robot_link rlink;
extern stopwatch watch;
*/
