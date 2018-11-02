#include "robot.h"

	
int main() {
	int val_white, val_blue;
	stopwatch watch12;
	
    if (!rlink.initialise (ROBOT_NUM)) {                // setup the link
        cout << "Cannot initialise link" << endl;
        rlink.print_errs("  ");
        return -1;
    }
    
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
			cout << "Package is red" << endl;
			return 0;
		}


		// Transparent
		if((val_white < 131) && (val_white > 125) && (val_blue < 53) && (val_blue > 50)) {
			rlink.command(WRITE_PORT_1, TRANSPARENT + multi_parcel);
			delivery_point = 0;
			cout << "Package is transparent" << endl;
			return 0;
		}
		
		// White
		if((val_white < 131) && (val_white > 121) && (val_blue < 53) && (val_blue > 42)) {
			rlink.command(WRITE_PORT_1, WHITE + multi_parcel + d2);
			delivery_point = 1;
			cout << "Package is white" << endl;
			return 0;
		}
		
		
		// Green
		if((val_white > 119) && (val_white < 125) && (val_blue < 46) && (val_blue > 32)) {
			rlink.command(WRITE_PORT_1, GREEN + multi_parcel + d2);
			delivery_point = 1;
			cout << "Package is green" << endl;
			return 0;
		}
		
		// Wood
		if((val_white > 100) && (val_white < 145) && (val_blue < 85) && (val_blue > 41)) {
			rlink.command(WRITE_PORT_1, WOOD + multi_parcel);
			delivery_point = 0;
			cout << "Package is wood" << endl;
			return 0;
		}
	}
    return 0;
}

