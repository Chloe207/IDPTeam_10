#include "robot.h"
using namespace std;

int main () {
    junction_no = 0;
	lost_no = 0;
	junction_detected = 0;
	debug = true;
    
    left_speed = 60;                                                // Rotation speed of motor 1(right)
    right_speed = 127 + left_speed;                                 // Rotation speed of motor 2 to go straight (left)


    watch.start();                                  // start timer
    
    if (!rlink.initialise (ROBOT_NUM)) {            // setup the link
        cout << "Cannot initialise link" << endl;
        rlink.print_errs("  ");
        return -1;
    }
    
    rlink.command (RAMP_TIME, ramp_speed);

    while (watch.read() < 30000) {                  // loop while watch less than 5mins
        
        board_0 = rlink.request(READ_PORT_0);       // Read from board 0
        board_1 = rlink.request(READ_PORT_1);       // Read from board 1
        
        for (int k = 0; k < 4; k++) {               // Output data from IRs
            IR[k] = (board_0 & ( 1 << k )) >> k;
        }
    
        line_follow();
        
    }
}


