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

#define ROBOT_NUM 14   // The id number (see below)
#define middle 0
#define left 1
#define back 2
#define right 3
#define white 4
#define	blue 5
#define	trigger 6
robot_link rlink;      					    				// Datatype for the robot link
using namespace std;
stopwatch watch;

//int motor_1(int motor_demanded) 

int pickup() {
	int motor_1, motor_2;

	motor_1 = 60;												// Rotation speed of motor 1(right)
	motor_2 = 127 + motor_1;									// Rotation speed of motor 2 to go straight (left)
	rlink.command(MOTOR_1_GO, 0);                       		// Stop the robot
	rlink.command(MOTOR_2_GO, 0);
	watch.start();
	while (watch.read() < 5000) { 
		delay(0.1);
	}
	rlink.command(MOTOR_1_GO, motor_1);                        // Stop the robot
	rlink.command(MOTOR_2_GO, motor_2);
	return 1;
}

int lost () {
	int IR_data, motor_1, motor_2;							// Data from IR sensors
	bool IR[7];

	motor_1 = 60;												// Rotation speed of motor 1(right)
    motor_2 = 127 + motor_1;									// Rotation speed of motor 2 to go straight (left)
    
    for (int a = 1; a < 50; a = a + 1) {
		IR_data = rlink.request(READ_PORT_0);	    			    // Read value from IR board
		
		for (int k = 0; k < 7; k++) {
		IR[k] = (IR_data & ( 1 << k )) >> k;
		}
		
		if (a < 20) {
			if (IR[middle] == 1) {
				 return 0;
			}
			else {
				rlink.command(MOTOR_1_GO, motor_1);
				rlink.command(MOTOR_2_GO, motor_2 * 0.8);			 	// Rotate to the left    
			}
		}
    
		if ((a < 40) && (a > 20)) {
			if (IR[middle] == 1) {
				return 0;
			}
			else {
				rlink.command(MOTOR_1_GO, motor_1 * 0.8);
				rlink.command(MOTOR_2_GO, motor_2);			 	        // Rotate to the right
			}
		}
    
		if (a > 40) { 
			if (IR[middle] == 1) {
				return 0;
			}
			else {
				rlink.command(MOTOR_1_GO, motor_2);
				rlink.command(MOTOR_2_GO, motor_1);			 			// Reverse
			}
		}
	}
	return 0;
}

int main () {
	int IR_data,motor_1,motor_2,a,pick_on;		        				// Data from IR sensors
	int junction_no=0, junction;	                         	// Data from microprocessor
	bool IR[7],package[11];
	
	for (int k = 0; k < 11; k++) {
		if ((k == 0) || (k == 1)) {
			package[k] = 0;
		}
		else {	 
			package[k] = 1;
		}		
	}

    motor_1 = 60;												// Rotation speed of motor 1(right)
    motor_2 = 127 + motor_1;									// Rotation speed of motor 2 to go straight (left)
    
	if (!rlink.initialise (ROBOT_NUM)) { 						// Setup the link
		cout << "Cannot initialise link" << endl;
		rlink.print_errs("  ");
		return -1;
	}
	else {
		cout << "Initialised"<< endl;
	}
	
	for (int t=1; t<30000; t = t+1) {
		IR_data = rlink.request(READ_PORT_0);	    			    // Read value from IR board

		for (int k = 0; k < 7; k++) {
			IR[k] = (IR_data & ( 1 << k )) >> k;
		}
		rlink.command (RAMP_TIME, 255);							    // Default ramp time
		if ((IR[right] == 0) && (IR[left] == 0) && (IR[middle] == 1)) {		
			a = 0;
			rlink.command(MOTOR_1_GO, motor_1);					    // Line only detected in the middle
			rlink.command(MOTOR_2_GO, motor_2);
			}
			
		if ((IR[right] == 1) && (IR[left] == 0) && (IR[middle] == 1)) {
			a = 0;
			rlink.command(MOTOR_1_GO, motor_1);				    // Line detected on the right i.e. robot going left
			rlink.command(MOTOR_2_GO, 127 + motor_1 * 1.80);
			//cout << "moving left" << endl;						// Debugging comment
		 }
			 
		if ((IR[right] == 1) && (IR[left] == 0) && (IR[middle] == 0)) {
			a = 0;
			rlink.command(MOTOR_1_GO, motor_1);			    // Line only detected on the right i.e. robot has strongly deviated to the left
			rlink.command(MOTOR_2_GO, 127 + motor_1 * 2.00);
			//cout << "moving very left" << endl;					// Debugging comment
		 }		
		 
		if ((IR[right] == 0) && (IR[left] == 1) && (IR[middle] == 1)) {
			a = 0;
			rlink.command(MOTOR_1_GO, motor_1 * 1.80);			    // Line detected on the left i.e. robot going right
			rlink.command(MOTOR_2_GO, motor_2);
			//cout << "moving right" << endl;						// Debugging comment
		}

		if ((IR[right] == 0) && (IR[left] == 1) && (IR[middle] == 0)) {
			a = 0;
			rlink.command(MOTOR_1_GO, motor_1 * 2.00);			    // Line only detected on the left i.e. robot has stongly deviated to the right
			rlink.command(MOTOR_2_GO, motor_2);
			//cout << "moving very right" << endl;				    // Debugging comment
		}
		
		if ((IR[right] == 0) && (IR[left] == 0) && (IR[middle] == 0)) {
			a = 0;
			lost();
		}
		
		if ((IR[right] == 1) && (IR[left] == 1) && (IR[middle] == 1)) {
			if (a == 0) {
				junction_no = junction_no + 1;				    	// Robot is going over a junction
                cout << "Junction" << junction_no <<endl;
                a = 1;
                junction = 1;
                
                if (package[junction_no - 1] == 1) {
					pick_on = pickup();
					package[junction_no-1] = 0;
				}
			}
		}
		
		if (((IR[back] == 1) && (junction_no == 6) && (junction == 1)) || ((IR[back] == 1) && (junction_no == 12) && (junction == 1)) || ((IR[back] == 1) && (junction_no == 13) && (junction == 1))) {
			cout << "Rotating for junction" << endl;			// Debugging comment
			rlink.command(MOTOR_1_GO, 0);						// Stop the robot
			rlink.command(MOTOR_2_GO, 0);
			delay(500);										// Wait
				
			for (int t=1; t<260; t = t+1) {
				rlink.command(MOTOR_1_GO, motor_1);					
				rlink.command(MOTOR_2_GO, motor_1*1.2);			 	// Rotate by 90 degrees
			}	
			junction = 0;
			cout << "Finished rotation" << endl;
            rlink.command(MOTOR_1_GO, 0);                        // Stop the robot
            rlink.command(MOTOR_2_GO, 0);
			delay(500);	
			lost();	
		}
		/*if ((IR[back] == 1) && (pick_on == 1) && (IR[trigger] == 0)) {
			cout << "Rotating for pickup" << endl;			// Debugging comment
			rlink.command(MOTOR_1_GO, 0);						// Stop the robot
			rlink.command(MOTOR_2_GO, 0);
			delay(500);										// Wait
				
			for (int t=1; t<220; t = t+1) {
				rlink.command(MOTOR_1_GO, motor_2*1.2);					
				rlink.command(MOTOR_2_GO, motor_2);			 	// Rotate by 90 degrees
			}	
			cout << "Finished pick-up rotation" << endl;
            rlink.command(MOTOR_1_GO, 0);                        // Stop the robot
            rlink.command(MOTOR_2_GO, 0);
			delay(500);	
			lost();
            rlink.command(MOTOR_1_GO, motor_1);                        // Stop the robot
            rlink.command(MOTOR_2_GO, motor_2);	
            delay(2000);
            rlink.command(MOTOR_1_GO, 0);                        // Stop the robot
            rlink.command(MOTOR_2_GO, 0);		
            cout << "done with IDP" << endl;
            //return 0;
			//pick_on = 0;	
		}*/
		
		/*if (IR[trigger] == 1) {
			rlink.command(MOTOR_1_GO, 0);                        // Stop the robot
            rlink.command(MOTOR_2_GO, 0);
		}*/
	}
	
}
