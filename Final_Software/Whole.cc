#include <iostream>
#include <robot_instr.h>
#include <robot_link.h>
#include <stdlib.h>     //for using the function sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include <cmath>

using namespace std;

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

int with_package, lost_line_reverse(),back_junction, b,junction, package_on, a,board_0, board_1, left_speed, right_speed, junction_no, parcel_num(), package_type(), package_received(), line_follow(),lost_line();
bool sensor1[7], sensor2[7], junction_detected, package[10];
void read_sensors_secondary(), dropoff_truck(), read_sensors(), turn_left(), turn_right(), turn_around(), rth(), dropoff(), pickup_package(), pickup();
	
robot_link rlink;                      					// datatype for the robot link
stopwatch watch;                       					// setup watch

void turn_around(void)  {
    watch.start();
    while (watch.read() < 3640) {
		rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, left_speed*1.2);                                 // Rotate 180 degrees
    }
    watch.stop();
}

void rth(void)  {
}

void read_sensors() {
	board_0 = rlink.request(READ_PORT_0);           // Read from board 0
        
    for (int k = 0; k < 7; k++) {                   // Output data from IRs
        sensor1[k] = (board_0 & ( 1 << k )) >> k;
    }
}

void read_sensors_secondary() {
	board_1 = rlink.request(READ_PORT_1);           // Read from board 1
	for (int k = 0; k < 7; k++) {        
		 sensor2[k] = (board_1 & ( 1 << k )) >> k;
	}
}

int lost_line () {
    watch.start();
    if (watch.read() < 200) {														// For 200ms, if line is lost, rotate to the left
		if (sensor1[middle] == 1) {
			return 0;
		}
		else {
			rlink.command(MOTOR_1_GO, left_speed);
			rlink.command(MOTOR_2_GO, right_speed * 0.8);			 				// Rotate to the left
			delay(0.1);
		}
	}
	
    if ((watch.read() < 400) && (watch.read() > 200)) {								// If the above does not work, rotate back in for 200ms and rotate right for 200ms
		if (sensor1[middle] == 1) {
			return 0;
		}
		else {
			rlink.command(MOTOR_1_GO, left_speed * 0.8);
			rlink.command(MOTOR_2_GO, right_speed);			 	        			// Rotate to the right
		}
	}
    
	if ((watch.read() < 800) && (watch.read() > 400)) { 							// For 400ms try moving backwards
		if (sensor1[middle] == 1) {
				return 0;
		}
		else {
			rlink.command(MOTOR_1_GO, right_speed);
			rlink.command(MOTOR_2_GO, left_speed);			 						// Reverse
		}
	}
	return 0;
}

int lost_line_reverse() {
    watch.start();
    if (watch.read() < 200) {														// For 200ms, if line is lost, rotate to the left
		if (sensor1[middle] == 1) {
			return 0;
		}
		else {
			rlink.command(MOTOR_1_GO, right_speed);
			rlink.command(MOTOR_2_GO, left_speed * 0.8);			 				// Rotate to the left
			delay(0.1);
		}
	}
	
    if ((watch.read() < 400) && (watch.read() > 200)) {								// If the above does not work, rotate back in for 200ms and rotate right for 200ms
		if (sensor1[middle] == 1) {
			return 0;
		}
		else {
			rlink.command(MOTOR_1_GO, right_speed * 0.8);
			rlink.command(MOTOR_2_GO, left_speed);			 	        			// Rotate to the right
		}
	}
    
	if ((watch.read() < 800) && (watch.read() > 400)) { 							// For 400ms try moving backwards
		if (sensor1[middle] == 1) {
				return 0;
		}
		else {
			rlink.command(MOTOR_1_GO, left_speed);
			rlink.command(MOTOR_2_GO, right_speed);			 						// Reverse
		}
	}
	return 0;
}

void turn_left()  {
	cout << " Turning left" << endl;
    rlink.command(MOTOR_1_GO, 0);                                        			// Stop the robot and wait (stabilise)
    rlink.command(MOTOR_2_GO, 0);
    delay(300);                           						// Start the rotation to the left
    
    watch.start();
    while (watch.read() < 2200) {
        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, left_speed*1.2);                                 	// Rotate by 90 degrees to the left
        delay(0.1);
    }
    watch.stop();
    
    rlink.command(MOTOR_1_GO, 0);                                       			// Stop the robot after the rotation has finished
    rlink.command(MOTOR_2_GO, 0);
    delay(300);
        
    watch.start();
    while (watch.read() < 300) {
        rlink.command(MOTOR_1_GO, left_speed);                                		// Move forward a little (this is needed otherwise lost_line() does not work)
        rlink.command(MOTOR_2_GO, right_speed);
        delay(0.1);
    }
    watch.stop();
}

void turn_right() {
	cout << "Turning right" << endl;
    rlink.command(MOTOR_1_GO, 0);                                        			// Stop the robot and wait (stabilise)
    rlink.command(MOTOR_2_GO, 0);
    delay(300);                       					// Start the rotation to the right
    
    watch.start();
    while (watch.read() < 1920) {
        rlink.command(MOTOR_1_GO, right_speed*1.2);
        rlink.command(MOTOR_2_GO, right_speed);                                 	// Rotate by 90 degrees to the right
        delay(0.1);
    }
    watch.stop();
    rlink.command(MOTOR_1_GO, 0);                                        			// Stop the robot
    rlink.command(MOTOR_2_GO, 0);
    delay(300);
 
    watch.start();   
    while (watch.read() < 300) {
        rlink.command(MOTOR_1_GO, left_speed);                                		// Move forward a little (this is needed otherwise lost_line() does not work)
        rlink.command(MOTOR_2_GO, right_speed);
        delay(0.1);
    } 
    watch.stop();
    package_on = 0;
}

void dropoff()   {
	watch.start();
	while (watch.read() < 1000) {
		read_sensors();
		if (watch.read() < 20) {
			rlink.command(MOTOR_1_GO, right_speed);					    				// Line only detected in the middle
			rlink.command(MOTOR_2_GO, left_speed);
		}
		if (sensor1[back] == 1) {
			break;
		}
		if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 1)) {	
			b = 0;
			rlink.command(MOTOR_1_GO, right_speed);					    				// Line only detected in the middle
			rlink.command(MOTOR_2_GO, left_speed);
		}
		else {
			b = 0;
			lost_line_reverse();
		}
		if ((sensor1[right] == 1) && (sensor1[left] == 1) && (sensor1[middle] == 1)) {
			if (b == 0) {
				back_junction = back_junction + 1;				    	// Robot is going over a junction
				b = 1;
			}
		}
	}
	rlink.command(MOTOR_1_GO, 0);					    				// Line only detected in the middle
	rlink.command(MOTOR_2_GO, 0);
	delay(500);
	turn_left();
	with_package = 1;
	cout << "Package: " << with_package << endl;
	line_follow();
}

void dropoff_truck() {
	rlink.command(MOTOR_1_GO, 0);                        		// Stop the robot
	rlink.command(MOTOR_2_GO, 0);
	delay(500);	
	watch.start();
	while (watch.read() < 300) { 
		rlink.command(MOTOR_1_GO, left_speed);                  // Stop the robot
		rlink.command(MOTOR_2_GO, right_speed);	
		delay(0.1);
	}
	watch.stop();
	watch.start();
	while (watch.read() < 800) {
		read_sensors();
		if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 0)) {
			lost_line();
		}
		else { 
			rlink.command(MOTOR_1_GO, left_speed);                        
			rlink.command(MOTOR_2_GO, right_speed);	
		}
	}
	watch.stop();
	rlink.command(MOTOR_1_GO, 0);                        // Stop the robot
	rlink.command(MOTOR_2_GO, 0);
}

void pickup_package() {
	delay(500);
	watch.start();
	while (watch.read() < 2500) {
		rlink.command(MOTOR_3_GO, 110);
		delay(0.1);
	}
	while ((watch.read() < 2700) && (watch.read() > 2500))  {
		rlink.command(WRITE_PORT_1, push_actuator+lift_actuator);     // Push actuator forward whilst remaining up
		delay(0.1);
    }
    delay(2000);
    while ((watch.read() < 5400)&&(watch.read() > 2700)) {
		rlink.command(MOTOR_3_GO, 238);
		delay(0.1);
	}
	watch.stop();
    rlink.command(WRITE_PORT_1, 0);                 // Pull back while down
    delay(500);
    
    cout << "Picked up package" << endl;
    delay(500);
    back_junction = 0;
    dropoff();
}

void pickup() {
	cout << package_on << endl;
	if (package_on == 1) {
		turn_right();
		rlink.command(MOTOR_1_GO, 0);                        		// Stop the robot
		rlink.command(MOTOR_2_GO, 0);
		delay(500);	
		watch.start();
		while (watch.read() < 300) { 
			cout << "pickup 1 "<< endl;
			rlink.command(MOTOR_1_GO, left_speed);                  // Stop the robot
			rlink.command(MOTOR_2_GO, right_speed);	
			delay(0.1);
		}
		watch.stop();
		watch.start();
		while (watch.read() < 1000) {
			cout << "pickup 2 " << endl;
			read_sensors();
			if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 0)) {
				lost_line();
			}
			else { 
				rlink.command(MOTOR_1_GO, left_speed);                        
				rlink.command(MOTOR_2_GO, right_speed);	
			}
		}
		watch.stop();
		cout << "Stop motors" << endl;
		rlink.command(MOTOR_1_GO, 0);                        // Stop the robot
		rlink.command(MOTOR_2_GO, 0);
	}
	package_on = 0;
	pickup_package();  	
}


int line_follow() {
	if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 1)) {	
		//cout << "Middle" << endl;	
		a = 0;
		rlink.command(MOTOR_1_GO, left_speed);					    				// Line only detected in the middle
		rlink.command(MOTOR_2_GO, right_speed);
	}
			
	if ((sensor1[right] == 1) && (sensor1[left] == 0) && (sensor1[middle] == 1)) {
		//cout << "Right" << endl;
		a = 0;
		rlink.command(MOTOR_1_GO, left_speed);				    					// Line detected on the right i.e. robot going left
		rlink.command(MOTOR_2_GO, 127 + left_speed * 1.80);
	}
			 
	if ((sensor1[right] == 1) && (sensor1[left] == 0) && (sensor1[middle] == 0)) {
		//cout << "Very right" << endl;
		a = 0;
		rlink.command(MOTOR_1_GO, left_speed);			    						// Line only detected on the right i.e. robot has strongly deviated to the left
		rlink.command(MOTOR_2_GO, 127 + left_speed * 2.00);
	}		
		 
	if ((sensor1[right] == 0) && (sensor1[left] == 1) && (sensor1[middle] == 1)) {
		//cout << "Left" << endl;
		a = 0;
		rlink.command(MOTOR_1_GO, left_speed * 1.80);			    				// Line detected on the left i.e. robot going right
		rlink.command(MOTOR_2_GO, right_speed);
	}

	if ((sensor1[right] == 0) && (sensor1[left] == 1) && (sensor1[middle] == 0)) {
		//cout << "Very left" << endl;
		a = 0;
		rlink.command(MOTOR_1_GO, left_speed * 2.00);			    				// Line only detected on the left i.e. robot has stongly deviated to the right
		rlink.command(MOTOR_2_GO, right_speed);
	}
		
	if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 0)) {					// Line lost
		//cout << "Lost" << endl;
		a = 0;
		lost_line();
	}
		
	if ((sensor1[right] == 1) && (sensor1[left] == 1) && (sensor1[middle] == 1)) {
		if ((a == 0) && (package_on == 0)) {
			junction_no = junction_no + 1;				    	// Robot is going over a junction
            cout << "Junction" << junction_no <<endl;
            a = 1;
            junction_detected = 1;
              
            if ((package[junction_no - 1] == 1) && (with_package == 0)&& (with_package == 0))  {
				package_on = 1;
				package[junction_no-1] = 0;
			}
			
		}
	}
	
	if ((junction_no == 16) && (junction_detected == 1)) {
		cout << "calling drop off" << endl;
		dropoff();
	}
		
	if (((sensor1[back] == 1) && (junction_no == 6) && (junction_detected == 1)) || ((sensor1[back] == 1) && (junction_no == 12) && (junction_detected== 1)) || ((sensor1[back] == 1) && (junction_no == 13) && (junction_detected == 1))) {
		cout << "This is an angle" << endl;
		turn_left();
		junction_detected = 0;
	}
	
	if ((package[junction_no] == 1) && (sensor1[back] == 1) && (with_package == 0)) {
		cout << "This junction has a package" << endl;
		package_on = 1;
		read_sensors_secondary();				// Check functionality of this part
		pickup();
	}

	return 0;
}

int parcel_num(int) {
    return 0;
}

/*void error() {
	int stat;
	if (rlink.fatal_err()) { 							// check for errors
		rlink.print_errs(); 							// print them out
		rlink.reinitialise(); 							// flush the link
	}
	if (watch.read() > 300000) {
		//rlink.command (SHUTDOWN ROBOT, 0) 				// If the robot has been running for too long, switch everything off
		stat = rlink.request (STATUS); 					// Returns the contents of  the microprocessor's general status register
		cout << stat << endl;
	}
	
	rlink.command (STOP_IF_HIGH, 0x377);
	rlink.command (STOP_SELECT, 0);	
}*/
	

int main () {
    junction_no = 0;									// Junction number									
	junction_detected = 0;
    
    left_speed = 60;                                    // Rotation speed of motor 1(right)
    right_speed = 127 + left_speed;                     // Rotation speed of motor 2 to go straight (left)
    rlink.command (RAMP_TIME, ramp_speed);

    
    for (int k = 0; k < 10; k++) {                      // All stations have packages
		if (k==3) {
			package[k] = 1;
		}
		else {
			package[k] = 0;
		}
    }
        
    if (!rlink.initialise (ROBOT_NUM)) {                // setup the link
        cout << "Cannot initialise link" << endl;
        rlink.print_errs("  ");
        return -1;
    }
    
    rlink.command(WRITE_PORT_1, 0);
    rlink.command(WRITE_PORT_0, 0);
    
    for (int t=1; t< 300; t = t+1) {                      // loop while watch less than 5mins
//		error();      
		cout << watch.read() << endl;
		watch.stop();
		watch.start();
        read_sensors();
        with_package == 0;       
        //sensor1[front_switch] = -sensor1[front_switch]; // Invert switch input
        line_follow();
        
    }
}

