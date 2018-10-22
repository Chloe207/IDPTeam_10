#include "robot.h"
using namespace std;

junction_no = 0;
lost_no = 0;
junction_detected = 0;
debug = true;


int main () {
    
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
    
        line_follow();
        
    }
}


