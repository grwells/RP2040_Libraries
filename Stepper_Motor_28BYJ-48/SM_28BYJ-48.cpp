#include "SM_28BYJ-48.h"
#include <pico/stdlib.h>
#include <stdio.h>

/*
 * Construct a new object with pin definitions and default states.
 */
SM_28BYJ_48::SM_28BYJ_48(int in1, int in2, int in3, int in4){
    IN1 = in1;
    IN2 = in2;
    IN3 = in3;
    IN4 = in4;

    state = -1; // set out of bounds initially
    direction = true; // default is CW
    step_size = 1;

    // init gpio
    gpio_init(IN1);
    gpio_init(IN2);
    gpio_init(IN3);
    gpio_init(IN4);
    // set all pins as outputs
    gpio_set_dir(IN1, true);
    gpio_set_dir(IN2, true);
    gpio_set_dir(IN3, true);
    gpio_set_dir(IN4, true);
    // set all pins LOW
    gpio_put(IN1, false);
    gpio_put(IN2, false);
    gpio_put(IN3, false);
    gpio_put(IN4, false);
}

/*
 * Base step function. Uses direction, step size set by other step(direction) and warp_speed_mr_sulu(direction)
 */
void SM_28BYJ_48::step(void){
    if(state > 7 || state < 0){ // out of bounds so reset
        if(!direction){
            state = 0;
        }else{
            state = 7;
        }
    }

    //printf("[STEPPER] setting state = %i\n", state);
    switch(STATE[state]){
        case 0x08:
            gpio_put(IN4, 1);
            gpio_put(IN3, 0);
            gpio_put(IN2, 0);
            gpio_put(IN1, 0);
            break;

        case 0x0C:
            gpio_put(IN4, 1);
            gpio_put(IN3, 1);
            gpio_put(IN2, 0);
            gpio_put(IN1, 0);
            break;

        case 0x04:
            gpio_put(IN4, 0);
            gpio_put(IN3, 1);
            gpio_put(IN2, 0);
            gpio_put(IN1, 0);
            break;

        case 0x06:
            gpio_put(IN4, 0);
            gpio_put(IN3, 1);
            gpio_put(IN2, 1);
            gpio_put(IN1, 0);
            break;

        case 0x02:
            gpio_put(IN4, 0);
            gpio_put(IN3, 0);
            gpio_put(IN2, 1);
            gpio_put(IN1, 0);
            break;

        case 0x03:
            gpio_put(IN4, 0);
            gpio_put(IN3, 0);
            gpio_put(IN2, 1);
            gpio_put(IN1, 1);
            break;

        case 0x01:
            gpio_put(IN4, 0);
            gpio_put(IN3, 0);
            gpio_put(IN2, 0);
            gpio_put(IN1, 1);
            break;

        case 0x09:
            gpio_put(IN4, 1);
            gpio_put(IN3, 0);
            gpio_put(IN2, 0);
            gpio_put(IN1, 1);
            break;
    }

    state += direction ? -1*step_size : step_size;
    offset_since_epoch += direction ? -1*step_size : step_size; // net distance traveled from home starting pos

}

/*
 * Tell the motor to take one step in a specific direction, does not change speed.
 */
void SM_28BYJ_48::step(Direction dir){
    direction = dir;
    step();
}

/*
 * Allows setting the direction to either clockwise(CW) or counter-clockwise(CCW)
 *  WARNING: resets the step size to 1
 */
void SM_28BYJ_48::turtle_speed(Direction dir){
    direction = dir;
    step_size = 1;
    step();
}

/*
 * Config option that sets the step size to 2, and sets the direction.
 */
void SM_28BYJ_48::warp_speed_mr_sulu(Direction dir){
    step_size = 2;
    direction = dir;
}

/*
 * Get the state/phase of the motor, which corresponds to the pin activation sequence
 */
int SM_28BYJ_48::get_state(void){
    return this->state;
}
