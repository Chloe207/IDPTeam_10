#include "robot.h"

int board_0, board_1, left_speed, right_speed, junction_no, parcel_num(), package_type(), package_received(), line_follow(), pickup(),lost_line(), pickup_package();
bool sensor1[7], sensor2[7], junction_detected, package[10];
void turn_left(), turn_right(), turn_around(), rth(), junction(), dropoff();
	
robot_link rlink;                      					// datatype for the robot link
stopwatch watch;                       					// setup watch

int main () {
    junction_no = 0;									// Junction number									
	junction_detected = 0;
    
    left_speed = 60;                                    // Rotation speed of motor 1(right)
    right_speed = 127 + left_speed;                     // Rotation speed of motor 2 to go straight (left)
    
    for (int k = 0; k < 10; k++) {                      // All stations have packages
        package[k] = 1;
    }
    
    watch.start();                                      // start timer
    
    if (!rlink.initialise (ROBOT_NUM)) {                // setup the link
        cout << "Cannot initialise link" << endl;
        rlink.print_errs("  ");
        return -1;
    }
    
    rlink.command (RAMP_TIME, ramp_speed);

    while (watch.read() < 30000) {                      // loop while watch less than 5mins
        
        board_0 = rlink.request(READ_PORT_0);           // Read from board 0
        board_1 = rlink.request(READ_PORT_1);           // Read from board 1
        
        for (int k = 0; k < 7; k++) {                   // Output data from IRs
            sensor1[k] = (board_0 & ( 1 << k )) >> k;
            sensor2[k] = (board_1 & ( 1 << k )) >> k;
        }
        
        sensor1[front_switch] = -sensor1[front_switch]; // Invert switch input
        
        line_follow();
        
    }
}


