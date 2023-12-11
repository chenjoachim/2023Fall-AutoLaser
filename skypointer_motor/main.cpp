#include "mbed.h"
#include "Stepper.h"
PinName A4988STEP = D10;
PinName A4988DIR = D0;
PinName MS1 = D2;
PinName MS2 = D3;
PinName MS3 = D4;
PinName EN = D1;
int a = 0;

int main() {
    Stepper base_stepper(EN, MS1, MS2, MS3, A4988STEP, A4988DIR);
    float speed = 0;
    int microstep = 16;
    while (true) {
        base_stepper.enable();
        if (a < 200) // sweep 200 step in dir 1
        {
            a++;
            speed = 100.0;
            base_stepper.step(microstep, 0, speed);
        } 
        else {
            a++;
            speed = 250.0;
            base_stepper.step(microstep, 1, speed);

            if (a > 400) // sweep 200 in dir 2
            {
                a = 0;
                base_stepper.disable();
                wait_us(1000000);
            }
        }
    }
}