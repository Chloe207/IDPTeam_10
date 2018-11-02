#include "robot.h"


int package_type() {
    int val_white, val_blue;
	stopwatch watch12;
	    
    cout << "Determining package colour" << endl;
    watch12.start();
    for (int t=0; t<3; t++) {
		rlink.command(WRITE_PORT_0, white_LED);                 // Turn on white LED
		delay(200);
		val_white = rlink.request(light_sensor);                // Read sensor
		delay(200);
		rlink.command(WRITE_PORT_0, LED_off);                   // Turn off white LED
		
		delay(200);
		rlink.command(WRITE_PORT_0, blue_LED);                 // Turn on blue LED
		delay(200);
		val_blue = rlink.request(light_sensor);                // Read sensor
		delay(200);
		rlink.command(WRITE_PORT_0, LED_off);                 // Turn off blue LED
		
		cout << "White: " << val_white << endl;
		cout << "Blue: " << val_blue << endl;
		
		
		// Threshold values need to be tested and set - should be able to get these with light_test.cc
		// Also includes delivery_point bool so delivery point can be set

		
		// Red
		if((val_white < 123) && (val_white > 118) && (val_blue < 31) && (val_blue > 29)){
			rlink.command(WRITE_PORT_1, RED + multi_parcel + d2);
			delivery_point = 1;
			truck = 1;
			cout << "Package is red" << endl;
			return 0;
		}


		// Transparent
		if((val_white < 131) && (val_white > 125) && (val_blue < 53) && (val_blue > 50)) {
			rlink.command(WRITE_PORT_1, TRANSPARENT + multi_parcel);
			delivery_point = 0;
			truck = 2;
			cout << "Package is transparent" << endl;
			return 0;
		}
		
		// White
		if((val_white < 131) && (val_white > 121) && (val_blue < 53) && (val_blue > 42)) {
			rlink.command(WRITE_PORT_1, WHITE + multi_parcel + d2);
			delivery_point = 1;
			truck = 2;
			cout << "Package is white" << endl;
			return 0;
		}
		
		
		// Green
		if((val_white > 119) && (val_white < 125) && (val_blue < 46) && (val_blue > 32)) {
			rlink.command(WRITE_PORT_1, GREEN + multi_parcel + d2);
			delivery_point = 1;
			truck = 2;
			cout << "Package is green" << endl;
			return 0;
		}
		
		// Wood
		if((val_white > 100) && (val_white < 145) && (val_blue < 85) && (val_blue > 41)) {
			rlink.command(WRITE_PORT_1, WOOD + multi_parcel);
			delivery_point = 0;
			truck = 1;
			cout << "Package is wood" << endl;
			return 0;
		}
	}
	cout << "Needed default" << endl;
	truck = 1;
    return 0;
}

void read_sensors() {
    board_0 = rlink.request(READ_PORT_0);                        // Read from board 0    
    for (int k = 0; k < 7; k++) {                                // Output data from IRs
        sensor1[k] = (board_0 & ( 1 << k )) >> k;
    }
}

void read_sensors_secondary() {
    board_1 = rlink.request(READ_PORT_1);                        // Read from board 1
    for (int k = 0; k < 7; k++) {        
         sensor2[k] = (board_1 & ( 1 << k )) >> k;
    }
}

int lost_line () {
    stopwatch watch2;                                            // Will need to adjust to avoid repeated command, but sensitive so test while changing code
    watch2.start();
    if (watch2.read() < 200) {                                    // For 200ms, if line is lost, rotate to the left
        if (sensor1[middle] == 1) {
            return 0;
        }
        else {
            rlink.command(MOTOR_1_GO, left_speed);
            rlink.command(MOTOR_2_GO, right_speed * 0.8);        // Rotate to the left
            delay(0.1);
        }
    }
    
    if ((watch2.read() < 400) && (watch2.read() > 200)) {        // If the above does not work, rotate back in for 200ms and rotate right for 200ms
        if (sensor1[middle] == 1) {
            return 0;
        }
        else {
            rlink.command(MOTOR_1_GO, left_speed * 0.8);
            rlink.command(MOTOR_2_GO, right_speed);                // Rotate to the right
            delay(0.1);
        }
    }
    
    if ((watch2.read() < 800) && (watch2.read() > 400)) {         // For 400ms try moving backwards
        if (sensor1[middle] == 1) {
                return 0;
        }
        else {
            rlink.command(MOTOR_1_GO, right_speed);
            rlink.command(MOTOR_2_GO, left_speed);                // Reverse
            delay(0.1);
        }
    }

    watch2.stop();
    return 0;
}

int lost_line_reverse() {
    stopwatch watch3;
    
    watch3.start();
    if (watch3.read() < 200) {                                    // For 200ms, if line is lost, rotate to the left
        if (sensor1[middle] == 1) {
            return 0;
        }
        else {
            rlink.command(MOTOR_1_GO, right_speed);
            rlink.command(MOTOR_2_GO, left_speed * 0.8);        // Rotate to the left
            delay(0.1);
        }
    }
    
    if ((watch3.read() < 400) && (watch3.read() > 200)) {        // If the above does not work, rotate back in for 200ms and rotate right for 200ms
        if (sensor1[middle] == 1) {
            return 0;
        }
        else {
            rlink.command(MOTOR_1_GO, right_speed * 0.8);
            rlink.command(MOTOR_2_GO, left_speed);                // Rotate to the right
        }
    }
    
    if ((watch3.read() < 800) && (watch3.read() > 400)) {         // For 400ms try moving backwards
        if (sensor1[middle] == 1) {
                return 0;
        }
        else {
            rlink.command(MOTOR_1_GO, left_speed);
            rlink.command(MOTOR_2_GO, right_speed);                 // Reverse
        }
    }
    watch3.stop();
    return 0;
}

void turn_left()  {
    stopwatch watch4;
    cout << " Turning left" << endl;
    rlink.command(MOTOR_1_GO, 0);                                // Stop the robot and wait (stabilise)
    rlink.command(MOTOR_2_GO, 0);
    delay(300);                                                   // Start the rotation to the left
    
    watch4.start();
	rlink.command(MOTOR_1_GO, left_speed);
	rlink.command(MOTOR_2_GO, left_speed*1.2);					// Rotate by 90 degrees to the left
    while (watch4.read() < 1380) {
		;
	}
    watch4.stop();
    
    rlink.command(MOTOR_1_GO, 0);                                // Stop the robot after the rotation has finished
    rlink.command(MOTOR_2_GO, 0);
    delay(300);
        
    watch4.start();
    rlink.command(MOTOR_1_GO, left_speed);                        // Move forward a little (this is needed otherwise lost_line() does not work)
    rlink.command(MOTOR_2_GO, right_speed);
    while (watch4.read() < 300) {
        ;
    }
    watch4.stop();
}

void turn_right() {
    stopwatch watch5;
    cout << "Turning right" << endl;
    rlink.command(MOTOR_1_GO, 0);                                // Stop the robot and wait (stabilise)
    rlink.command(MOTOR_2_GO, 0);
    delay(300);                                                   // Start the rotation to the right
    
    watch5.start();
    rlink.command(MOTOR_1_GO, right_speed);
    rlink.command(MOTOR_2_GO, right_speed);                        // Rotate by 90 degrees to the right
    while (watch5.read() < 1750) {
        ;
    }
    watch5.stop();

    rlink.command(MOTOR_1_GO, 0);                                // Stop the robot
    rlink.command(MOTOR_2_GO, 0);
    delay(300);
 
    watch5.start(); 
    rlink.command(MOTOR_1_GO, left_speed);                        // Move forward a little (this is needed otherwise lost_line() does not work)
    rlink.command(MOTOR_2_GO, right_speed);
    while (watch5.read() < 300) {
        ;
    } 
    watch5.stop();
    package_on = 0;
}

void dropoff_prepare()   {
	cout << "Dropping off" << endl;
	stopwatch watch6;
	watch6.start();

	rlink.command(MOTOR_1_GO, right_speed);													// Line only detected in the middle
	rlink.command(MOTOR_2_GO, left_speed);
	delay(1000);
	while ((watch6.read() < 1000) && (watch6.read() > 50)) {
		cout << watch6.read() << endl;
		read_sensors();
		if (sensor1[back] == 1) {
			cout << "Found junction again while moving back" << endl;
			break;
		}
		else if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 1)) {
			cout << "Reversing" << endl;
			if (again == 0) {
				b = 0;
				rlink.command(MOTOR_1_GO, right_speed);					    				// Line only detected in the middle
				rlink.command(MOTOR_2_GO, left_speed);
				again = 1;
			}
			else{
				;
			}
		}
		else {
			b = 0;
			cout << "Lost moving back" << endl;
			lost_line_reverse();
			again = 0;
		}
		if ((sensor1[right] == 1) && (sensor1[left] == 1) && (sensor1[middle] == 1)) {
			if (b == 0) {
				back_junction = back_junction + 1;				    						// Robot is going over a junction
				b = 1;
			}
		}
	}
	watch6.stop();

	rlink.command(MOTOR_1_GO, 0);					    									
	rlink.command(MOTOR_2_GO, 0);
	delay(500);
	turn_left();
	cout << "Going for dropoff" << endl;
	with_package = 1;
	cout << "Package: " << with_package << endl;
	line_follow();
}

void dropoff_truck() {
	int again1;
	stopwatch watch7;

	again1 = 0;

	cout << "Stopping" << endl;

	rlink.command(MOTOR_1_GO, 0);                        		// Stop the robot
	rlink.command(MOTOR_2_GO, 0);
	delay(1000);

	watch7.start();
	cout << "Approaching truck" << endl;

	rlink.command(MOTOR_1_GO, left_speed);
	rlink.command(MOTOR_2_GO, right_speed);
	while (watch7.read() < 400) { 
		;
	}
	watch7.stop();

	watch7.start();
	while (watch7.read() < 1500) {
		read_sensors();
		if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 0)) {
			cout << "Adjusting approach" << endl;
			again1 = 0;
			lost_line();
		}
		else { 
			if (again1 == 0) {
				rlink.command(MOTOR_1_GO, left_speed);
				rlink.command(MOTOR_2_GO, right_speed);
				again1 = 1;
			}
			else {
				;
			}
		}
	}
	watch7.stop();

	rlink.command(MOTOR_1_GO, 0);                        // Stop the robot
	rlink.command(MOTOR_2_GO, 0);
	delay(10);

	watch7.start();
	rlink.command(WRITE_PORT_1, 64+32);     // Push actuator forward whilst remaining up
	cout << "Arm extended" << endl;
	while (watch7.read() < 100)  {
		;
	}

	rlink.command(MOTOR_3_GO, 110);
	while ((watch7.read() < 2700) && (watch7.read() > 100)) {
		;
	}
	watch7.stop();
	delay(1000);
	watch7.start();
	rlink.command(MOTOR_1_GO, right_speed);													// Line only detected in the middle
	rlink.command(MOTOR_2_GO, left_speed);
	while (watch7.read() < 20) {
		;
	}
	again1 = 0;
	watch7.stop();
	watch7.start();
	while (watch7.read() < 700) {
		read_sensors();
		cout << "Inside here" << endl;
		if ((sensor1[right] == 1) && (sensor1[left] == 1) && (sensor1[middle] == 1)) {
			rlink.command(MOTOR_1_GO, right_speed);													// Line only detected in the middle
			rlink.command(MOTOR_2_GO, left_speed);
			delay(200);
			cout << "Ready to rotate again" << endl;
			break;
		}
		cout << "Here 2" << endl;
		if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 1)) {
			if (again1 == 0) {
				b = 0;
				cout << "Here 1" << endl;
				rlink.command(MOTOR_1_GO, right_speed);					    				// Line only detected in the middle
				rlink.command(MOTOR_2_GO, left_speed);
				again1 = 1;
			}
			else {
				cout << "Here 3" << endl;
				;
			}
		}
		else {
			b = 0;
			lost_line_reverse();
			again1 = 0;
		}
	}
	
	watch7.stop();
	
	watch7.start();
	while (watch7.read() < 500) {
		cout << "Reversing" << endl;
		rlink.command(MOTOR_1_GO, right_speed);													// Line only detected in the middle
		rlink.command(MOTOR_2_GO, left_speed);
	}
	rlink.command(MOTOR_1_GO, 0);								// Stop the robot and wait (stabilise)
    rlink.command(MOTOR_2_GO, 0);
	
	watch7.start();
	rlink.command(WRITE_PORT_1, 0);
	while (watch7.read() < 100) {
		;
	}
	rlink.command(MOTOR_3_GO, 238);
	while ((watch7.read() > 100) && (watch7.read() < 2600)) {
		;
	}
	watch7.stop();
	
	cout << " Finding path again" << endl;
    delay(300);                           						// Start the rotation to the left

    
    watch7.start();
    while (watch7.read() < 2000) {
		read_sensors();
		if ((sensor1[middle] == 0) && (sensor1[right] == 1)) {
			cout << "Adjusting 1" << endl;
			rlink.command(MOTOR_1_GO, left_speed*1.6);
			rlink.command(MOTOR_2_GO, right_speed*0.1);
		}
		if ((sensor1[middle] == 0) && (sensor1[left] == 1)) {
			cout << "Adjusting 2" << endl;
			rlink.command(MOTOR_1_GO, left_speed*0.1);
			rlink.command(MOTOR_2_GO, right_speed*1.6);
		}
		else {
			cout << "Adjusting 3" << endl;
			rlink.command(MOTOR_1_GO, left_speed);
			rlink.command(MOTOR_2_GO, right_speed);                    // Rotate by 90 degrees to the left
		}
	}
    watch7.stop();
    rlink.command(MOTOR_1_GO, 0);                                // Stop the robot after the rotation has finished
    rlink.command(MOTOR_2_GO, 0);
    delay(300);
        
    watch7.start();
    cout << "Here 5" << endl;
    rlink.command(MOTOR_1_GO, left_speed);                        // Move forward a little (this is needed otherwise lost_line() does not work)
    rlink.command(MOTOR_2_GO, right_speed);
    while (watch7.read() < 300) {
        ;
    }
    watch7.stop();
    restart = 1;
    line_follow();
    
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
    watch8.stop();
    watch8.start();
    cout << "Arm extended" << endl;
    while (watch8.read() < 100)  {
		cout << watch8.read() << endl;
        rlink.command(WRITE_PORT_1, 64+32);     // Push actuator forward whilst remaining up
        cout <<rlink.request(READ_PORT_1) << endl;;
        delay(0.1);
    }
    delay(500);
    watch8.stop();
    watch8.start();
    while (watch8.read() < 2500) {
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
    dropoff_prepare();
}

void pickup() {
	stopwatch watch9;
	cout << package_on << endl;
	rlink.command(MOTOR_1_GO, right_speed);					    				// Line only detected in the middle
	rlink.command(MOTOR_2_GO, left_speed);
	delay(200);
	if (package_on == 1) {
		turn_right();
		rlink.command(MOTOR_1_GO, 0);                        		// Stop the robot
		rlink.command(MOTOR_2_GO, 0);
		delay(500);	
		watch9.start();
		rlink.command(MOTOR_1_GO, left_speed);                  // Stop the robot
		rlink.command(MOTOR_2_GO, right_speed);	
		while (watch9.read() < 300) { 
			;
		}
		watch9.stop();
		cout << "Start stopwatch" << endl;
		watch9.start();
		
		while (watch9.read() < 1400) {
			//cout << watch.read() << endl;
			read_sensors();
			//cout << rlink.request(dist_sensor) << endl;         // Read distance sensor
			if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 0)) {
				lost_line();
				cout << "In lost line will approaching package" << endl;
				rlink.command(MOTOR_1_GO, left_speed);                        
				rlink.command(MOTOR_2_GO, right_speed);	
				//cout << "Correcting itself" << endl;
			}	
			
			if ((sensor1[right] == 1) && (sensor1[left] == 0) && (sensor1[middle] == 1)) {
				//cout << "Right" << endl;
				rlink.command(MOTOR_1_GO, left_speed);				    					// Line detected on the right i.e. robot going left
				rlink.command(MOTOR_2_GO, 127 + left_speed * 1.20);
			}
				 
			if ((sensor1[right] == 1) && (sensor1[left] == 0) && (sensor1[middle] == 0)) {
				rlink.command(MOTOR_1_GO, left_speed);			    						// Line only detected on the right i.e. robot has strongly deviated to the left
				rlink.command(MOTOR_2_GO, 127 + left_speed * 1.30);
			}		
			 
			if ((sensor1[right] == 0) && (sensor1[left] == 1) && (sensor1[middle] == 1)) {
				rlink.command(MOTOR_1_GO, left_speed * 1.20);			    				// Line detected on the left i.e. robot going right
				rlink.command(MOTOR_2_GO, right_speed);
			}

			if ((sensor1[right] == 0) && (sensor1[left] == 1) && (sensor1[middle] == 0)) {
				rlink.command(MOTOR_1_GO, left_speed * 1.30);			    				// Line only detected on the left i.e. robot has stongly deviated to the right
				rlink.command(MOTOR_2_GO, right_speed);
			}
			rlink.command(MOTOR_1_GO, left_speed);                        
			rlink.command(MOTOR_2_GO, right_speed);	
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
            rlink.command(MOTOR_1_GO, left_speed);                                        // Line only detected in the middle
            rlink.command(MOTOR_2_GO, right_speed);
            delay(0.1);
        }
    }
            
    if ((sensor1[right] == 1) && (sensor1[left] == 0) && (sensor1[middle] == 1)) {
        //cout << "Right" << endl;
        a = 0;
        again = 0;
        rlink.command(MOTOR_1_GO, left_speed);                                        // Line detected on the right i.e. robot going left
        rlink.command(MOTOR_2_GO, 127 + left_speed * 1.20);
        delay(0.1);
    }
             
    if ((sensor1[right] == 1) && (sensor1[left] == 0) && (sensor1[middle] == 0)) {
        //cout << "Very right" << endl;
        a = 0;
        again = 0;
        rlink.command(MOTOR_1_GO, left_speed);                                        // Line only detected on the right i.e. robot has strongly deviated to the left
        rlink.command(MOTOR_2_GO, 127 + left_speed * 1.30);
        delay(0.1);
    }        
         
    if ((sensor1[right] == 0) && (sensor1[left] == 1) && (sensor1[middle] == 1)) {
        //cout << "Left" << endl;
        a = 0;
        again = 0;
        rlink.command(MOTOR_1_GO, left_speed * 1.20);                                // Line detected on the left i.e. robot going right
        rlink.command(MOTOR_2_GO, right_speed);
        delay(0.1);
    }

    if ((sensor1[right] == 0) && (sensor1[left] == 1) && (sensor1[middle] == 0)) {
        //cout << "Very left" << endl;
        a = 0;
        again = 0;
        rlink.command(MOTOR_1_GO, left_speed * 1.30);                                // Line only detected on the left i.e. robot has stongly deviated to the right
        rlink.command(MOTOR_2_GO, right_speed);
        delay(0.1);
    }
        
    if ((sensor1[right] == 0) && (sensor1[left] == 0) && (sensor1[middle] == 0)) {                    // Line lost
        //cout << "Lost" << endl;
        a = 0;
        again = 0;
        lost_line();
    }
        
    if ((sensor1[right] == 1) && (sensor1[left] == 1) && (sensor1[middle] == 1)) {
        again = 0;
        if ((a == 0) && (package_on == 0)) {
            junction_no = junction_no + 1;                        // Robot is going over a junction
            cout << "Junction" << junction_no <<endl;
            a = 1;
            junction_detected = 1;
              
            if ((package[junction_no - 1] == 1) && (with_package == 0)&& (with_package == 0))  {
                package_on = 1;
                package[junction_no-1] = 0;
            }
        if (junction_no == 2) {
            restart = 0;
        }
            
        }
    }
    
    if ((junction_no == 16) && (junction_detected == 1) && (sensor1[back] == 1) && (watch10.read() > 250000)) {
        cout << "Returning home" << endl;
        turn_right();
        rlink.command(MOTOR_1_GO, 0);                                // Line detected on the left i.e. robot going right
        rlink.command(MOTOR_2_GO, 0);
        delay(30000);

    }
    
    if ((junction_no == 12) && (junction_detected == 1) && (sensor1[back] == 1)&& (truck == 1)) {
        cout << "Restarting" << endl;
        cout << "Restart turning left" << endl;
        turn_left();
        
        restart = 0;
        with_package = 0;
        junction_no = 2;
    }
    
    if ((junction_no == 16) && (junction_detected == 1) && (sensor1[back] == 1)&& (truck == 1)) {
        cout << "Restarting" << endl;
        cout << "Restart turning left" << endl;
        turn_left();
        
        restart = 0;
        with_package = 0;
        junction_no = 2;
    }
    
    if ((sensor1[back] == 1) && (junction_no == 10) && (junction_detected == 1) && (restart == 0) && (truck == 2)) {
		turn_right();
    }
    
    if ((sensor1[back] == 1) && (junction_no == 12) && (junction_detected == 1) && (restart == 0) && (truck == 2)) {
        turn_left();
    }
    
    if ((junction_no == 11) && (junction_detected == 1) && (restart == 0) && (truck == 1)) {
        cout << "Calling drop off" << endl;
        dropoff_truck();
    }  
    
    if ((junction_no == 13) && (junction_detected == 1) && (restart == 0) && (truck == 2)) {
		cout << "Calling drop off" << endl;
        dropoff_truck();
    }

    if (((sensor1[back] == 1) && (junction_no == 6) && (junction_detected == 1)) || ((sensor1[back] == 1) && (junction_no == 9) && (junction_detected == 1))) {
        cout << "This is an angle" << endl;
        cout << "Angle turning left" << endl;
        turn_left();
        junction_detected = 0;
    }

    if ((package[junction_no] == 1) && (sensor1[back] == 1) && (with_package == 0)) {
        cout << "This junction has a package" << endl;
        package_on = 1;
        read_sensors_secondary();                // Check functionality of this part
        package[junction_no] = 0;
        pickup();
    }

    return 0;
}

void error() {
    if (rlink.fatal_err()) {                             // check for errors
        rlink.print_errs();                             // print them out
        rlink.reinitialise();                             // flush the link   
		rlink.command (STOP_IF_HIGH, 0x377);
     	rlink.command (STOP_SELECT, 0);    
	}	
}
    

int main () {
    
    junction_no = 0;                                    // Junction number                                    
    junction_detected = 0;
    again = 0;
    
    left_speed = 90;                                    // Rotation speed of motor 1(right)
    right_speed = 127 + left_speed;                     // Rotation speed of motor 2 to go straight (left)
    rlink.command (RAMP_TIME, ramp_speed);

    
    for (int k = 0; k < 10; k++) {                      // All stations have packages
        if (k<3) {
            package[k] = 0;
        }
        else {
            package[k] = 1;
        }
    }
        
    if (!rlink.initialise (ROBOT_NUM)) {                // setup the link
        cout << "Cannot initialise link" << endl;
        rlink.print_errs("  ");
        return -1;
    }

    rlink.command(WRITE_PORT_1, 0);
    rlink.command(WRITE_PORT_0, LED_off);
    watch10.start();
    while (watch10.read() < 300000) {                      // loop while watch less than 5mins
//        error();    
        read_sensors();    
        //sensor1[front_switch] = -sensor1[front_switch]; // Invert switch input
        line_follow();
        
    }
}

