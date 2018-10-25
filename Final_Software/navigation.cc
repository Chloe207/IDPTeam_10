#include "robot.h"

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

void turn_left()  {
    rlink.command(MOTOR_1_GO, 0);                                        			// Stop the robot and wait (stabilise)
    rlink.command(MOTOR_2_GO, 0);
    delay(300);
    
    cout << "Turning left" << endl;                            						// Start the rotation to the left
    
    watch.start();
    while (watch.read() < 1820) {
        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, left_speed*1.2);                                 	// Rotate by 90 degrees to the left
        delay(0.1);
    }
    watch.stop();
    
    cout << "Finished rotation" << endl;
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
    rlink.command(MOTOR_1_GO, 0);                                        			// Stop the robot and wait (stabilise)
    rlink.command(MOTOR_2_GO, 0);
    delay(300);
    
    cout << "Turning right" << endl;                            					// Start the rotation to the right
    
    watch.start();
    while (watch.read() < 1820) {
        rlink.command(MOTOR_1_GO, right_speed*1.2);
        rlink.command(MOTOR_2_GO, right_speed);                                 	// Rotate by 90 degrees to the right
        delay(0.1);
    }
    watch.stop();
    
    cout << "Finished rotation" << endl;
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
}

int pickup() {
    turn_right();																	// Turn right towards the pickup area
    
	watch.start();
	while ((watch.read() < 1500) && (sensor1[front_switch] == 0)) {
		if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 0)) {	// Move towards the docking area
			cout << "lost here" << endl;
			lost_line();
		}
		else { 
			rlink.command(MOTOR_1_GO, left_speed);                    				// If not lost, just move forward
			rlink.command(MOTOR_2_GO, right_speed);
		}
	}
	watch.stop();
	
	rlink.command(MOTOR_1_GO, 0);                        							// Stop the robot, this will be when the switch is activated
    rlink.command(MOTOR_2_GO, 0);		
    delay(100);
    cout << "At package pick-up" << endl;
    pickup_package();
    return 1;
}


int line_follow() {
	int a;		        															// Used to not read the same junction multiple times
    	
	if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 1)) {		
		a = 0;
		rlink.command(MOTOR_1_GO, left_speed);					    				// Line only detected in the middle
		rlink.command(MOTOR_2_GO, right_speed);
	}
			
	if ((sensor1[right] == 1) && (sensor1[left] == 0) && (sensor1[middle] == 1)) {
		a = 0;
		rlink.command(MOTOR_1_GO, left_speed);				    					// Line detected on the right i.e. robot going left
		rlink.command(MOTOR_2_GO, 127 + left_speed * 1.80);
	}
			 
	if ((sensor1[right] == 1) && (sensor1[left] == 0) && (sensor1[middle] == 0)) {
		a = 0;
		rlink.command(MOTOR_1_GO, left_speed);			    						// Line only detected on the right i.e. robot has strongly deviated to the left
		rlink.command(MOTOR_2_GO, 127 + left_speed * 2.00);
	}		
		 
	if ((sensor1[right] == 0) && (sensor1[left] == 1) && (sensor1[middle] == 1)) {
		a = 0;
		rlink.command(MOTOR_1_GO, left_speed * 1.80);			    				// Line detected on the left i.e. robot going right
		rlink.command(MOTOR_2_GO, right_speed);
	}

	if ((sensor1[right] == 0) && (sensor1[left] == 1) && (sensor1[middle] == 0)) {
		a = 0;
		rlink.command(MOTOR_1_GO, left_speed * 2.00);			    				// Line only detected on the left i.e. robot has stongly deviated to the right
		rlink.command(MOTOR_2_GO, right_speed);
	}
		
	if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 0)) {					// Line lost
		a = 0;
		lost_line();
	}
		
	if ((sensor1[right] == 1) && (sensor1[left] == 1) && (sensor1[middle] == 1)) {
		if (a == 0) {
			junction_no = junction_no + 1;				    						// Robot is going over a junction
            cout << "Junction: " << junction_no <<endl;
            a = 1;
            junction_detected = 1;
		}
	}
		
	if (((sensor1[back] == 1) && (junction_no == 6) && (junction_detected == 1)) || ((sensor1[back] == 1) && (junction_no == 12) && (junction_detected== 1)) || ((sensor1[back] == 1) && (junction_no == 13) && (junction_detected == 1))) {
		turn_left();
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


