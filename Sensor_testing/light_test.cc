#include "robot.h"


int package_type() {
	int val_white, val_blue;
    
    cout << "Press enter to begin test..." << endl;
    //if(cin.get() == 1) {
    
    cout << "Testing" << endl;
    
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
    //}
}
