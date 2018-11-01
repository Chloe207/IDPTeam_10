#include "robot.h"

	
int main() {
	int val_white, val_blue;
	
    if (!rlink.initialise (ROBOT_NUM)) {                // setup the link
        cout << "Cannot initialise link" << endl;
        rlink.print_errs("  ");
        return -1;
    }
    
    cout << "Determining package colour" << endl;
    
    rlink.command(WRITE_PORT_0, white_LED);                 // Turn on white LED
    delay(500);
    val_white = rlink.request(light_sensor);                // Read sensor
    delay(500);
    rlink.command(WRITE_PORT_0, LED_off);                   // Turn off white LED
    
    delay(500);
    rlink.command(WRITE_PORT_0, blue_LED);                 // Turn on blue LED
    delay(500);
    val_blue = rlink.request(light_sensor);                // Read sensor
    delay(500);
    rlink.command(WRITE_PORT_0, LED_off);                 // Turn off blue LED
    
    cout << "White: " << val_white << endl;
    cout << "Blue: " << val_blue << endl;
    
    
    // Threshold values need to be tested and set - should be able to get these with light_test.cc
    // Also includes delivery_point bool so delivery point can be set
    // Green
    if((val_white >0) && (val_white < 30) && (val_blue < 30) && (val_blue > 0)) {
        rlink.command(WRITE_PORT_1, GREEN + multi_parcel + d2);
        delivery_point = 1;
        cout << "Package is green" << endl;
    }
    // Red
    if((val_white < 60) && (val_white > 31) && (val_blue < 60) && (val_blue > 31)){
        rlink.command(WRITE_PORT_1, RED + multi_parcel + d2);
        delivery_point = 1;
        cout << "Package is red" << endl;
    }
    // Wood
    if((val_white > 61) && (val_white < 90) && (val_blue < 90) && (val_blue > 61)) {
        rlink.command(WRITE_PORT_1, WOOD + multi_parcel);
        delivery_point = 0;
        cout << "Package is wood" << endl;
    }
    // White
    if((val_white < 120) && (val_white > 91) && (val_blue < 120) && (val_blue > 91)) {
        rlink.command(WRITE_PORT_1, WHITE + multi_parcel + d2);
        delivery_point = 1;
        cout << "Package is white" << endl;
    }
    // Transparent
    if((val_white < 150) && (val_white > 121) && (val_blue < 150) && (val_blue > 121)) {
        rlink.command(WRITE_PORT_1, TRANSPARENT + multi_parcel);
        delivery_point = 0;
        cout << "Package is transparent" << endl;
    }
    return 0;
}

