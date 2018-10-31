#include "robot.h"


int package_type() {
	int val_white, val_blue;
    
    cout << "Determining package colour" << endl;
    
    rlink.command(WRITE_PORT_0, white_LED);                 // Turn on white LED
    delay(100);
    val_white = rlink.request(light_sensor);        // Read sensor
    delay(100);
    rlink.command(WRITE_PORT_0, 0);                 // Turn off white LED
    
    delay(100);
    rlink.command(WRITE_PORT_0, blue_LED);                 // Turn on blue LED
    delay(100);
    val_blue = rlink.request(light_sensor);         // Read sensor
    delay(100);
    rlink.command(WRITE_PORT_0, 0);                 // Turn off blue LED
    
    cout << "White: " << val_white << endl;
    cout << "Blue: " << val_blue << endl;
    return 0;
}
void turn_around(void)  {
	stopwatch watch1;
    watch1.start();
    while (watch1.read() < 3640) {
		rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, left_speed*1.2);                                 // Rotate 180 degrees
    }
    watch1.stop();
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
	stopwatch watch2;
    watch2.start();
    if (watch2.read() < 200) {														// For 200ms, if line is lost, rotate to the left
		if (sensor1[middle] == 1) {
			return 0;
		}
		else {
			rlink.command(MOTOR_1_GO, left_speed);
			rlink.command(MOTOR_2_GO, right_speed * 0.8);			 				// Rotate to the left
			delay(0.1);
		}
	}
	
    if ((watch2.read() < 400) && (watch2.read() > 200)) {								// If the above does not work, rotate back in for 200ms and rotate right for 200ms
		if (sensor1[middle] == 1) {
			return 0;
		}
		else {
			rlink.command(MOTOR_1_GO, left_speed * 0.8);
			rlink.command(MOTOR_2_GO, right_speed);			 	        			// Rotate to the right
			delay(0.1);
		}
	}
    
	if ((watch2.read() < 800) && (watch2.read() > 400)) { 							// For 400ms try moving backwards
		if (sensor1[middle] == 1) {
				return 0;
		}
		else {
			rlink.command(MOTOR_1_GO, right_speed);
			rlink.command(MOTOR_2_GO, left_speed);			 						// Reverse
			delay(0.1);
		}
	}
	watch2.stop();
	return 0;
}

int lost_line_reverse() {
	stopwatch watch3;
	
    watch3.start();
    if (watch3.read() < 200) {														// For 200ms, if line is lost, rotate to the left
		if (sensor1[middle] == 1) {
			return 0;
		}
		else {
			rlink.command(MOTOR_1_GO, right_speed);
			rlink.command(MOTOR_2_GO, left_speed * 0.8);			 				// Rotate to the left
			delay(0.1);
		}
	}
	
    if ((watch3.read() < 400) && (watch3.read() > 200)) {								// If the above does not work, rotate back in for 200ms and rotate right for 200ms
		if (sensor1[middle] == 1) {
			return 0;
		}
		else {
			rlink.command(MOTOR_1_GO, right_speed * 0.8);
			rlink.command(MOTOR_2_GO, left_speed);			 	        			// Rotate to the right
		}
	}
    
	if ((watch3.read() < 800) && (watch3.read() > 400)) { 							// For 400ms try moving backwards
		if (sensor1[middle] == 1) {
				return 0;
		}
		else {
			rlink.command(MOTOR_1_GO, left_speed);
			rlink.command(MOTOR_2_GO, right_speed);			 						// Reverse
		}
	}
	watch3.stop();
	return 0;
}

void turn_left()  {
	stopwatch watch4;
	cout << " Turning left" << endl;
    rlink.command(MOTOR_1_GO, 0);                                        			// Stop the robot and wait (stabilise)
    rlink.command(MOTOR_2_GO, 0);
    delay(300);                           						// Start the rotation to the left
    
    watch4.start();
    while (watch4.read() < 2200) {
        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, left_speed*1.2);                                 	// Rotate by 90 degrees to the left
        delay(0.1);
    }
    watch4.stop();
    
    rlink.command(MOTOR_1_GO, 0);                                       			// Stop the robot after the rotation has finished
    rlink.command(MOTOR_2_GO, 0);
    delay(300);
        
    watch4.start();
    while (watch4.read() < 300) {
        rlink.command(MOTOR_1_GO, left_speed);                                		// Move forward a little (this is needed otherwise lost_line() does not work)
        rlink.command(MOTOR_2_GO, right_speed);
        delay(0.1);
    }
    watch4.stop();
}

void turn_right() {
	stopwatch watch5;
	cout << "Turning right" << endl;
    rlink.command(MOTOR_1_GO, 0);                                        			// Stop the robot and wait (stabilise)
    rlink.command(MOTOR_2_GO, 0);
    delay(300);                       					// Start the rotation to the right
    
    watch5.start();
    while (watch5.read() < 1940) {
        rlink.command(MOTOR_1_GO, right_speed*1.2);
        rlink.command(MOTOR_2_GO, right_speed);                                 	// Rotate by 90 degrees to the right
        delay(0.1);
    }
    watch5.stop();
    rlink.command(MOTOR_1_GO, 0);                                        			// Stop the robot
    rlink.command(MOTOR_2_GO, 0);
    delay(300);
 
    watch5.start();   
    while (watch5.read() < 300) {
        rlink.command(MOTOR_1_GO, left_speed);                                		// Move forward a little (this is needed otherwise lost_line() does not work)
        rlink.command(MOTOR_2_GO, right_speed);
        delay(0.1);
    } 
    watch5.stop();
    package_on = 0;
}

void dropoff()   {
	stopwatch watch6;
	watch6.start();
	while (watch6.read() < 1000) {
		read_sensors();
		if (watch6.read() < 20) {
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
	watch6.stop();
	rlink.command(MOTOR_1_GO, 0);					    				// Line only detected in the middle
	rlink.command(MOTOR_2_GO, 0);
	delay(500);
	turn_left();
	with_package = 1;
	cout << "Package: " << with_package << endl;
	line_follow();
}

void dropoff_truck() {
	stopwatch watch7;
	cout << "stopping" << endl;
	rlink.command(MOTOR_1_GO, 0);                        		// Stop the robot
	rlink.command(MOTOR_2_GO, 0);
	delay(1000);	
	watch7.start();
	cout << "Approaching truck" << endl;
	while (watch7.read() < 300) { 
		rlink.command(MOTOR_1_GO, left_speed);                  // Stop the robot
		rlink.command(MOTOR_2_GO, right_speed);	
		delay(0.1);
	}
	watch7.stop();
	watch7.start();
	while (watch7.read() < 1000) {
		read_sensors();
		if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 0)) {
			cout << "Adjusting approach" << endl;
			lost_line();
		}
		else { 
			rlink.command(MOTOR_1_GO, left_speed);                        
			rlink.command(MOTOR_2_GO, right_speed);	
		}
	}
	watch7.stop();
	rlink.command(MOTOR_1_GO, 0);                        // Stop the robot
	rlink.command(MOTOR_2_GO, 0);
	delay(1);
	watch7.start();
	while (watch7.read() < 100)  {
		cout << "Arm extended" << endl;
		rlink.command(WRITE_PORT_1, push_actuator+lift_actuator);     // Push actuator forward whilst remaining up
		delay(0.1);
	}
	while ((watch7.read() < 2600) && (watch7.read() > 100)) {
		rlink.command(MOTOR_3_GO, 110);
		delay(0.1);
	}
	while ((watch7.read() < 2700) && (watch7.read() > 2600)) {
		rlink.command(WRITE_PORT_1, 0);
		delay(0.1);
	}
	while ((watch7.read() < 5400) && (watch7.read() > 2700)) {
		rlink.command(MOTOR_3_GO, 238);
		delay(0.1);
	}
	delay(2000);
}

void pickup_package() {
	stopwatch watch8;
	delay(500);
	watch8.stop();
	watch8.start();
	while (watch8.read() < 2500) {
		rlink.command(MOTOR_3_GO, 110);
		delay(0.1);
	}
	while ((watch8.read() < 2700) && (watch8.read() > 2500))  {
		cout << "Arm extended" << endl;
		rlink.command(WRITE_PORT_1, push_actuator+lift_actuator);     // Push actuator forward whilst remaining up
		delay(0.1);
    }
    delay(2000);
    while ((watch8.read() < 5400)&&(watch8.read() > 2700)) {
		rlink.command(MOTOR_3_GO, 238);
		delay(0.1);
	}
	watch8.stop();
    rlink.command(WRITE_PORT_1, 0);                 // Pull back while down
    delay(500);
    
    cout << "Picked up package" << endl;
    delay(500);
    back_junction = 0;
    package_type();
    dropoff();
}

void pickup() {
	stopwatch watch9;
	cout << package_on << endl;
	if (package_on == 1) {
		turn_right();
		rlink.command(MOTOR_1_GO, 0);                        		// Stop the robot
		rlink.command(MOTOR_2_GO, 0);
		delay(500);	
		watch9.start();
		rlink.command(MOTOR_1_GO, left_speed);                  // Stop the robot
		rlink.command(MOTOR_2_GO, right_speed);	
		while (watch9.read() < 300) { 
			//cout << "pickup 1 "<< endl;	
			;
		}
		watch9.stop();
		cout << "start stopwatch" << endl;
		watch9.start();
		
		while (watch9.read() < 1200) {
			cout << watch.read() << endl;
			//cout << "pickup 2 " << endl;
			read_sensors();
			if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 0)) {
				lost_line();
				rlink.command(MOTOR_1_GO, left_speed);                        
				rlink.command(MOTOR_2_GO, right_speed);	
				//cout << "Correcting itself" << endl;
			}
		}
		watch9.stop();
		cout << "Stop motors" << endl;
		rlink.command(MOTOR_1_GO, 0);                        // Stop the robot
		rlink.command(MOTOR_2_GO, 0);
	}
	
	package_on = 0;
	pickup_package();  	
}


int line_follow() {
	if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 1)) {	
		if (again == 1) {
			delay(0.1);
		}
		else {
			//cout << "Middle" << endl;	
			a = 0;
			again = 1;
			rlink.command(MOTOR_1_GO, left_speed);					    				// Line only detected in the middle
			rlink.command(MOTOR_2_GO, right_speed);
			delay(0.1);
		}
	}
			
	if ((sensor1[right] == 1) && (sensor1[left] == 0) && (sensor1[middle] == 1)) {
		//cout << "Right" << endl;
		a = 0;
		again = 0;
		rlink.command(MOTOR_1_GO, left_speed);				    					// Line detected on the right i.e. robot going left
		rlink.command(MOTOR_2_GO, 127 + left_speed * 1.80);
		delay(0.1);
	}
			 
	if ((sensor1[right] == 1) && (sensor1[left] == 0) && (sensor1[middle] == 0)) {
		//cout << "Very right" << endl;
		a = 0;
		again = 0;
		rlink.command(MOTOR_1_GO, left_speed);			    						// Line only detected on the right i.e. robot has strongly deviated to the left
		rlink.command(MOTOR_2_GO, 127 + left_speed * 2.00);
		delay(0.1);
	}		
		 
	if ((sensor1[right] == 0) && (sensor1[left] == 1) && (sensor1[middle] == 1)) {
		//cout << "Left" << endl;
		a = 0;
		again = 0;
		rlink.command(MOTOR_1_GO, left_speed * 1.80);			    				// Line detected on the left i.e. robot going right
		rlink.command(MOTOR_2_GO, right_speed);
		delay(0.1);
	}

	if ((sensor1[right] == 0) && (sensor1[left] == 1) && (sensor1[middle] == 0)) {
		//cout << "Very left" << endl;
		a = 0;
		again = 0;
		rlink.command(MOTOR_1_GO, left_speed * 2.00);			    				// Line only detected on the left i.e. robot has stongly deviated to the right
		rlink.command(MOTOR_2_GO, right_speed);
		delay(0.1);
	}
		
	if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 0)) {					// Line lost
		//cout << "Lost" << endl;
		a = 0;
		again = 0;
		lost_line();
	}
		
	if ((sensor1[right] == 1) && (sensor1[left] == 1) && (sensor1[middle] == 1)) {
		again = 0;
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
	
	if ((junction_no == 11) && (junction_detected == 1)) {
		cout << "calling drop off" << endl;
		dropoff_truck();
	}
		
	if (((sensor1[back] == 1) && (junction_no == 6) && (junction_detected == 1)) || ((sensor1[back] == 1) && (junction_no == 9) && (junction_detected == 1))) {
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
	stopwatch watch10;
	
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
    watch10.start();
    while (watch10.read() < 300000) {                      // loop while watch less than 5mins
//		error();      
        read_sensors();
        with_package == 0;       
        //sensor1[front_switch] = -sensor1[front_switch]; // Invert switch input
        line_follow();
        
    }
}

