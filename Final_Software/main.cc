#include "robot.h"

int a,board_0, board_1, left_speed, right_speed, junction_no, parcel_num(), package_type(), package_received(), line_follow(), pickup(),lost_line(), pickup_package();
bool sensor1[7], sensor2[7], junction_detected, package[10];
void turn_left(), turn_right(), turn_around(), rth(), junction(), dropoff();
	
robot_link rlink;                      					// datatype for the robot link
stopwatch watch;                       					// setup watch

void error() {
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
}
	

int main () {
    junction_no = 0;									// Junction number									
	junction_detected = 0;
    
    left_speed = 60;                                    // Rotation speed of motor 1(right)
    right_speed = 127 + left_speed;                     // Rotation speed of motor 2 to go straight (left)
    rlink.command (RAMP_TIME, ramp_speed);
    
    for (int k = 0; k < 10; k++) {                      // All stations have packages
		/*if (k<=) {
			package[k] = 0;
		}*/
		//else {
			package[k] = 0;
		//}
    }
        
    if (!rlink.initialise (ROBOT_NUM)) {                // setup the link
        cout << "Cannot initialise link" << endl;
        rlink.print_errs("  ");
        return -1;
    }

    for (int t=1; t< 300000; t = t+1) {                      // loop while watch less than 5mins
		error();       
        board_0 = rlink.request(READ_PORT_0);           // Read from board 0
        board_1 = rlink.request(READ_PORT_1);           // Read from board 1
        
        for (int k = 0; k < 7; k++) {                   // Output data from IRs
            sensor1[k] = (board_0 & ( 1 << k )) >> k;
            sensor2[k] = (board_1 & ( 1 << k )) >> k;
        }
        
        //sensor1[front_switch] = -sensor1[front_switch]; // Invert switch input
        
        line_follow();
        
    }
}


