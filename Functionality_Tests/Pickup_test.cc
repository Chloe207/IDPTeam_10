// Unit test for pickup

#include "Pickup_test.h"


int pickup_package() {
    
    if (!rlink.initialise (ROBOT_NUM)) {                // setup the link
        cout << "Cannot initialise link" << endl;
        rlink.print_errs("  ");
        return -1;
    }
    
    rlink.command(MOTOR_3_GO, 30);                  // Lift up
    delay(50);
    rlink.command(MOTOR_3_GO, 10);
    delay(500);
    rlink.command(WRITE_PORT_1, push_actuator);     // Push actuator forward whilst remaining up
    delay(500);
    rlink.command(MOTOR_3_GO, 0);                   // Drop box whilst remaining forwards
    delay(500);
    rlink.command(WRITE_PORT_1, 0);                 // Pull back while down
    delay(500);
        
    cout << "Picked up package" << endl;
    
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
    delay(100)
    rlink.command(WRITE_PORT_0, 0);                 // Turn off blue LED
    
    cout << "White: " << val_white << endl;
    cout << "Blue: " << val_blue << endl;
    
    if(val_white < 30 && val_blue < 30) {
        rlink.command(WRITE_PORT_1, GREEN);
    }
    return 0;
}
