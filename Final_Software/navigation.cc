
#include "robot.h"

void turn_around(void)  {
    
}

void turn_left()    {
    
    if (debug == true) {
        cout << "turning left" << endl;
    }
    for (int t = 0; t < 1500/left_speed; t++) {
        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, left_speed);
    }
    
}

void turn_right()   {
    
    if (debug == true) {
        cout << "turning right" << endl;
    }
    for (int t = 0; t < 1500/left_speed; t++) {
        rlink.command(MOTOR_1_GO, right_speed);
        rlink.command(MOTOR_2_GO, right_speed);
    }
}

void rth(void)  {
}

void junction()  {
    
    if (junction_no == 1) {
        rlink.command(MOTOR_1_GO, 0);                        // Stop the robot
        rlink.command(MOTOR_2_GO, 0);
        delay(100);
        
        
        if (debug == true) {
            cout << "Turning at junction" << junction_no << endl;
        }
        
        turn_left();
    }
}

void line_follow () {

    right_speed = 30;                                            // rotation speed of right motor
    left_speed = 127 + right_speed;                              // rotation speed of left motor
    
    for (int k = 0; k < 4; k++) {                                // convert data from I2C into bits
        IR[k] = (board_0 & ( 1 << k )) >> k;
    }
    

            
    if ((IR[right] == 0) && (IR[left] == 0) && (IR[middle] == 1)) {
        junction_detected = 0;
        rlink.command(MOTOR_1_GO, right_speed);                    // Line only detected in the middle
        rlink.command(MOTOR_2_GO, left_speed);
        if (debug == true) {cout << "moving forward" << endl;}
    }
    
    if ((IR[right] == 1) && (IR[left] == 0) && (IR[middle] == 1)) {
        junction_detected = 0;
        rlink.command(MOTOR_1_GO, right_speed*0.4);
        rlink.command(MOTOR_2_GO, 127 + right_speed * 1.60);
        if (debug == true) {cout << "moving left" << endl;}
    }
    
    if ((IR[right] == 1) && (IR[left] == 0) && (IR[middle] == 0)) {
        junction_detected = 0;
        rlink.command(MOTOR_1_GO, right_speed);
        rlink.command(MOTOR_2_GO, 127 + right_speed * 1.80);
        if (debug == true) {cout << "moving very left" << endl;}
    }
    
    if ((IR[right] == 0) && (IR[left] == 1) && (IR[middle] == 1)) {
        rlink.command(MOTOR_1_GO, right_speed * 1.60);
        rlink.command(MOTOR_2_GO, left_speed);
        if (debug == true) {cout << "moving right" << endl;}
    }
            
    if ((IR[right] == 0) && (IR[left] == 1) && (IR[middle] == 0)) {
        junction_detected = 0;
        rlink.command(MOTOR_1_GO, right_speed * 2);
        rlink.command(MOTOR_2_GO, left_speed);
        if (debug == true) {cout << "moving very right" << endl;}
    }
            
    if ((IR[right] == 0) && (IR[left] == 0) && (IR[middle] == 0)) {
        junction_detected = 0;
        rlink.command(MOTOR_1_GO, 0);
        rlink.command(MOTOR_2_GO, 0);
        delay(100);
        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, right_speed);
        delay(500);
        lost_no = lost_no + 1;
        if (lost_no >100) {
            return;
        }
        cout << "lost line" << endl;
    }
        
    if ((IR[right] == 1) && (IR[left] == 1) && (IR[middle] == 1)) {     // Going over junction
        if (junction_detected == 0) {
            
            junction_no += 1;
            junction_detected = 1;
            
            if (debug == true) {
                cout << "going over junction" << endl;
                cout << junction_no <<endl;
            }
        }
    }
    
    if (IR[back] == 1) {
        junction();
        
        if (debug == true) {
            cout << "Back triggered" << endl;
        }
    }
}



void pickup()    {

}

void dropoff()   {

}



int parcel_num(int) {
    return 0;
}

