#ifndef ROBOT_H
#define ROBOT_H
#include "robot.h"
#endif




void turn_around(void)  {
    
}


void rth(void)  {
}

int lost_line () {
    int a;

		if (a < 20) {
			if (IR[middle] == 1) {
				return 0;
			}
			else {
				rlink.command(MOTOR_1_GO, left_speed);
				rlink.command(MOTOR_2_GO, right_speed * 0.8);			 	// Rotate to the left
			}
		}
    
		if ((a < 40) && (a > 20)) {
			if (IR[middle] == 1) {
				return 0;
			}
			else {
				rlink.command(MOTOR_1_GO, left_speed * 0.8);
				rlink.command(MOTOR_2_GO, right_speed);			 	        // Rotate to the right
			}
		}
    
		if (a > 40) { 
			if (IR[middle] == 1) {
				return 0;
			}
			else {
				rlink.command(MOTOR_1_GO, right_speed);
				rlink.command(MOTOR_2_GO, left_speed);			 			// Reverse
			}
		}
	return 0;
}

void turn_left(junction_detected)  {
	cout << "Rotating for junction" << endl;			// Debugging comment
	rlink.command(MOTOR_1_GO, 0);						// Stop the robot
	rlink.command(MOTOR_2_GO, 0);
	delay(500);										// Wait
		
	for (int t=1; t<260; t = t+1) {
		rlink.command(MOTOR_1_GO, left_speed);
		rlink.command(MOTOR_2_GO, left_speed*1.2);			 	// Rotate by 90 degrees
	}	
	junction_detected = 0;
	cout << "Finished rotation" << endl;
    rlink.command(MOTOR_1_GO, 0);                        // Stop the robot
    rlink.command(MOTOR_2_GO, 0);
	delay(500);	
	lost_line();
}

int pickup() {
	rlink.command(MOTOR_1_GO, 0);										// Stop the robot
	rlink.command(MOTOR_2_GO, 0);
	delay(500);															
	cout << "Start pick-up rotation" << endl;							// Start the rotation towards the pick-up point
	watch.start();
	while (watch.read() < 1820) { 
		rlink.command(MOTOR_1_GO, right_speed*1.2);
		rlink.command(MOTOR_2_GO, right_speed);			 					// Rotate by 90 degrees
		delay(0.1);		
	}
	cout << "Finished pick-up rotation" << endl;
    rlink.command(MOTOR_1_GO, 0);                        				// Stop the robot
    rlink.command(MOTOR_2_GO, 0);
	delay(500);	
	watch.stop();
	watch.start();
	while (watch.read() < 300) { 
		rlink.command(MOTOR_1_GO, left_speed);                        		// Move forward a little (this is needed otherwise lost_line() does not work)
		rlink.command(MOTOR_2_GO, right_speed);
		delay(0.1);
	}
	watch.stop();
	watch.start();
	while (watch.read() < 1500) {
		if ((IR[right] == 0) && (IR[left] == 0) && (IR[middle] == 0)) {	// Move towards the docking area, this will need to be changed when the switch is functional
			cout << "lost here" << endl;
			lost_line();
		}
		else { 
			rlink.command(MOTOR_1_GO, left_speed);                    		// If not lost, just move forward
			rlink.command(MOTOR_2_GO, right_speed);
		}
	}
	rlink.command(MOTOR_1_GO, 0);                        				// Stop the robot, this will be when the switch is activated
    rlink.command(MOTOR_2_GO, 0);		
    cout << "At package pick-up" << endl;
    return 0;	       													// Disable the pick up option, remember to name this pick-on later		
}


int line_follow() {
	int a;		        				// Data from IR sensors
    
	for (int k = 0; k < 10; k++) {
		package[k] = 1;
		}


		if ((IR[right] == 0) && (IR[left] == 0) && (IR[middle] == 1)) {		
			a = 0;
			rlink.command(MOTOR_1_GO, left_speed);					    // Line only detected in the middle
			rlink.command(MOTOR_2_GO, right_speed);
			}
			
		if ((IR[right] == 1) && (IR[left] == 0) && (IR[middle] == 1)) {
			a = 0;
			rlink.command(MOTOR_1_GO, left_speed);				    // Line detected on the right i.e. robot going left
			rlink.command(MOTOR_2_GO, 127 + left_speed * 1.80);
		 }
			 
		if ((IR[right] == 1) && (IR[left] == 0) && (IR[middle] == 0)) {
			a = 0;
			rlink.command(MOTOR_1_GO, left_speed);			    // Line only detected on the right i.e. robot has strongly deviated to the left
			rlink.command(MOTOR_2_GO, 127 + left_speed * 2.00);
		 }		
		 
		if ((IR[right] == 0) && (IR[left] == 1) && (IR[middle] == 1)) {
			a = 0;
			rlink.command(MOTOR_1_GO, left_speed * 1.80);			    // Line detected on the left i.e. robot going right
			rlink.command(MOTOR_2_GO, right_speed);
		}

		if ((IR[right] == 0) && (IR[left] == 1) && (IR[middle] == 0)) {
			a = 0;
			rlink.command(MOTOR_1_GO, left_speed * 2.00);			    // Line only detected on the left i.e. robot has stongly deviated to the right
			rlink.command(MOTOR_2_GO, right_speed);
		}
		
		if ((IR[right] == 0) && (IR[left] == 0) && (IR[middle] == 0)) {
			a = 0;
			lost_line();
		}
		
		if ((IR[right] == 1) && (IR[left] == 1) && (IR[middle] == 1)) {
			if (a == 0) {
				junction_no = junction_no + 1;				    	// Robot is going over a junction
                cout << "Junction" << junction_no <<endl;
                a = 1;
                junction_detected = 1;
			}
		}
		
		if (((IR[back] == 1) && (junction_no == 6) && (junction_detected == 1)) || ((IR[back] == 1) && (junction_no == 12) && (junction_detected== 1)) || ((IR[back] == 1) && (junction_no == 13) && (junction_detected == 1))) {
            
			turn_left(junction_detected);
		}
		
		if (package[junction_no] == 1) {
			pickup();
		}
	return 0;
}

void dropoff()   {

}

int parcel_num(int) {
    return 0;
}


