//#ifndef ROBOT_NUM
#include "robot.h"
//#endif



void turn_around(void)  {
    
}

/*void turn_left()    {
    
    if (debug == true) {
        cout << "turning left" << endl;
    }
    for (int t = 0; t < 1500/left_speed; t++) {
        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, left_speed);
    }
    
}

void turn_right()   {
    
    if (debug == true) {
        cout << "turning right" << endl;
    }
    for (int t = 0; t < 1500/left_speed; t++) {
        rlink.command(MOTOR_1_GO, right_speed);
        rlink.command(MOTOR_2_GO, right_speed);
    }
}*/

void rth(void)  {
}

int lost () {
	int IR_data, motor_1, motor_2;							// Data from IR sensors
	bool IR[4];

	motor_1 = 60;												// Rotation speed of motor 1(right)
    motor_2 = 127 + motor_1;									// Rotation speed of motor 2 to go straight (left)
    
    for (int a = 1; a < 50; a = a + 1) {
		IR_data = rlink.request(READ_PORT_0);	    			    // Read value from IR board
		
		for (int k = 0; k < 4; k++) {
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

void junction_r(int junction)  {
	int motor_1;
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

int pickup() {
	rlink.command(MOTOR_1_GO, 0);										// Stop the robot
	rlink.command(MOTOR_2_GO, 0);
	delay(500);															
	cout << "Start pick-up rotation" << endl;							// Start the rotation towards the pick-up point
	watch.start();
	while (watch.read() < 1820) { 
		rlink.command(MOTOR_1_GO, motor_2*1.2);					
		rlink.command(MOTOR_2_GO, motor_2);			 					// Rotate by 90 degrees
		delay(0.1);		
	}
	cout << "Finished pick-up rotation" << endl;
    rlink.command(MOTOR_1_GO, 0);                        				// Stop the robot
    rlink.command(MOTOR_2_GO, 0);
	delay(500);	
	watch.stop();
	watch.start();
	while (watch.read() < 300) { 
		rlink.command(MOTOR_1_GO, motor_1);                        		// Move forward a little (this is needed otherwise lost() does not work) 
		rlink.command(MOTOR_2_GO, motor_2);	
		delay(0.1);
	}
	watch.stop();
	watch.start();
	while (watch.read() < 1500) {
		if ((IR[right] == 0) && (IR[left] == 0) && (IR[middle] == 0)) {	// Move towards the docking area, this will need to be changed when the switch is functional
			cout << "lost here" << endl;
			lost();
		}
		else { 
			rlink.command(MOTOR_1_GO, motor_1);                    		// If not lost, just move forward    
			rlink.command(MOTOR_2_GO, motor_2);	
		}
	}
	rlink.command(MOTOR_1_GO, 0);                        				// Stop the robot, this will be when the switch is activated
    rlink.command(MOTOR_2_GO, 0);		
    cout << "At package pick-up" << endl;
    return 0;	       													// Disable the pick up option, remember to name this pick-on later		
}


int line_follow() {
	int IR_data,motor_1,motor_2,a;		        				// Data from IR sensors
	int junction_no=0, junction;	                         	// Data from microprocessor
	bool IR[4],package[10];
	
	for (int k = 0; k < 10; k++) {
		package[k] = 1;
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

		for (int k = 0; k < 4; k++) {
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
		 }
			 
		if ((IR[right] == 1) && (IR[left] == 0) && (IR[middle] == 0)) {
			a = 0;
			rlink.command(MOTOR_1_GO, motor_1);			    // Line only detected on the right i.e. robot has strongly deviated to the left
			rlink.command(MOTOR_2_GO, 127 + motor_1 * 2.00);
		 }		
		 
		if ((IR[right] == 0) && (IR[left] == 1) && (IR[middle] == 1)) {
			a = 0;
			rlink.command(MOTOR_1_GO, motor_1 * 1.80);			    // Line detected on the left i.e. robot going right
			rlink.command(MOTOR_2_GO, motor_2);
		}

		if ((IR[right] == 0) && (IR[left] == 1) && (IR[middle] == 0)) {
			a = 0;
			rlink.command(MOTOR_1_GO, motor_1 * 2.00);			    // Line only detected on the left i.e. robot has stongly deviated to the right
			rlink.command(MOTOR_2_GO, motor_2);
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
			}
		}
		
		if (((IR[back] == 1) && (junction_no == 6) && (junction == 1)) || ((IR[back] == 1) && (junction_no == 12) && (junction == 1)) || ((IR[back] == 1) && (junction_no == 13) && (junction == 1))) {
			junction_r(junction);
		}
		
		if (package[junction_no] == 1) {
			pickup();
		}
	}
	return 0;
}

void dropoff()   {

}

int parcel_num(int) {
    return 0;
}


