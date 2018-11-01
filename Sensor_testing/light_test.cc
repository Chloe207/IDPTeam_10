#include "robot.h"

	
int main() {
	int val_white, val_blue;
	
    if (!rlink.initialise (ROBOT_NUM)) {                // setup the link
        cout << "Cannot initialise link" << endl;
        rlink.print_errs("  ");
        return -1;
    }
    
    cout << "Determining package colour" << endl;
    rlink.command(WRITE_PORT_0, 0);                 // Turn on white LED
    rlink.command(WRITE_PORT_1,0b00110011);
    cout << rlink.request(READ_PORT_0) << endl;
    delay(100);
    val_white = rlink.request(light_sensor);        // Read sensor
    delay(100);
    rlink.command(WRITE_PORT_0, 0);                 // Turn off white LED
    
    delay(100);
    rlink.command(WRITE_PORT_0, blue_LED);                 // Turn on blue LED
    delay(100);
    val_blue = rlink.request(light_sensor);         // Read sensor
    delay(100);
    rlink.command(WRITE_PORT_0, 0);                // Turn off blue LED
    
    cout << "White: " << val_white << endl;
    cout << "Blue: " << val_blue << endl;
	return 0;
}
