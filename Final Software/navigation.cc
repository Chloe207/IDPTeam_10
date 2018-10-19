
#include <robot.h>

void line_follow () {
    
    int val,a,lost_no=0,junction_no=0;                                  // data from microprocessor

    right_speed = 20;                                            // rotation speed of right motor
    left_speed = 127 + right_speed;                              // rotation speed of left motor
    
    for (int k = 0; k < 4; k++) {                                // convert data from I2C into bits
        IR[k] = (board_0 & ( 1 << k )) >> k;
    }
    

            
    if ((IR[right] == 0) && (IR[left] == 0) && (IR[middle] == 1)) {
        rlink.command(MOTOR_1_GO, right_speed);                    // Line only detected in the middle
        rlink.command(MOTOR_2_GO, left_speed);
        if (DEBUG == true) {cout << "moving forward" << endl;}
    }
    
    if ((IR[right] == 1) && (IR[left] == 0) && (IR[middle] == 1)) {
        rlink.command(MOTOR_1_GO, right_speed*0.4);
        rlink.command(MOTOR_2_GO, 127 + right_speed * 1.60);
        if (DEBUG == true) {cout << "moving left" << endl;}
    }
    
    if ((IR[right] == 1) && (IR[left] == 0) && (IR[middle] == 0)) {
        rlink.command(MOTOR_1_GO, right_speed);
        rlink.command(MOTOR_2_GO, 127 + right_speed * 1.80);
        if (DEBUG == true) {cout << "moving very left" << endl;}
    }
    
    if ((IR[right] == 0) && (IR[left] == 1) && (IR[middle] == 1)) {
        rlink.command(MOTOR_1_GO, right_speed * 1.60);
        rlink.command(MOTOR_2_GO, left_speed);
        if (DEBUG == true) {cout << "moving right" << endl;}
    }
            
    if ((IR[right] == 0) && (IR[left] == 1) && (IR[middle] == 0)) {
        rlink.command(MOTOR_1_GO, right_speed * 2);
        rlink.command(MOTOR_2_GO, left_speed);
        if (DEBUG == true) {cout << "moving very right" << endl;}
    }
            
    if ((IR[right] == 0) && (IR[left] == 0) && (IR[middle] == 0)) {
        rlink.command(MOTOR_1_GO, 0);
        rlink.command(MOTOR_2_GO, 0);
        delay(100);
        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, right_speed);
        delay(500);
        lost_no = lost_no + 1;
        if (lost_no >100) {
            return 0;}
        cout << "lost line" << endl;
    }
        
    if ((IR[right] == 1) && (IR[left] == 1) && (IR[middle] == 1)) {
        junction_no = junction_no + 1;
        if (DEBUG == true) {
            cout << "going over junction" << endl;
            cout << junction_no <<endl;
        }
        for (int t=1;t<100;t=t+1){
            rlink.command(MOTOR_1_GO, right_speed);
            rlink.command(MOTOR_2_GO, left_speed);
            if (junction_no == 1) {
                if (IR[front] == 0) {
                    rlink.command(MOTOR_1_GO, right_speed);
                    rlink.command(MOTOR_2_GO, left_speed);}
                else {
                    rlink.command(MOTOR_1_GO, right_speed);
                    rlink.command(MOTOR_2_GO, right_speed);
                    delay(15);
                    if (IR[middle] == 1){
                        return 0;}}
            }
        }
    }
}



int pickup()    {
    
}

int dropoff()   {
    
}

int junction()  {
    
}

int parcel_num(int) {
    
}

void turn_around(void)  {
    
}

void turn_left(void)    {
    
}

void turn_right(void)   {
    
}

void rth(void)  {
    
}
