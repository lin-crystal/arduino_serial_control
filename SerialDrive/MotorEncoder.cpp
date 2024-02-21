#include "MotorEncoder.hpp"


// Initialize Motor
Motor motor_left = Motor();
Motor motor_right = Motor();

// Initialize pulse counters
int left_wheel_pulse_count = 0;
int right_wheel_pulse_count = 0;

// Initialize wheel directions
// 1 - forward, 0 - backward
int left_wheel_direction = -1;
int right_wheel_direction = -1;

void init_motors() {
    motor_left.init(ABOT_PIN_MOTOR_LEFT_DIRECTION, ABOT_PIN_MOTOR_LEFT_BRAKE, ABOT_PIN_MOTOR_LEFT_SPEED);
    motor_right.init(ABOT_PIN_MOTOR_RIGHT_DIRECTION, ABOT_PIN_MOTOR_RIGHT_BRAKE, ABOT_PIN_MOTOR_RIGHT_SPEED);
    motor_left.brake(true);
    motor_right.brake(true);
}

// Read wheel encoder values
String read_encoder_values()
{
  return String(left_wheel_pulse_count) + String(' ') + String(right_wheel_pulse_count);
}

// Left wheel callback function
void left_wheel_pulse()
{   
    // left wheel direction 
    // 1 - forward
    // 0 - backward
    // -1 - brake
    
    // Read encoder direction value for left wheel
    left_wheel_direction = motor_left.direction(); 

    if(left_wheel_direction == 1)
        left_wheel_pulse_count++;
    else if (left_wheel_direction == 0)
        left_wheel_pulse_count--;
}

// Right wheel callback function
void right_wheel_pulse()
{
    // right wheel direction 
    // 1 - forward,  
    // 0 - backward
    // -1 - brake

    // Read encoder direction value for right wheel
    right_wheel_direction = motor_right.direction();

    if(right_wheel_direction == 1)
        right_wheel_pulse_count++;
    else if (right_wheel_direction == 0)
        right_wheel_pulse_count--;
}

// Set each motor speed from the respective velocity command interface
void set_motor_speeds(double left_wheel_command, double right_wheel_command)
{   

    int left_motor_speed = ceil(left_wheel_command);
    int right_motor_speed = ceil(right_wheel_command);

    // Set motor directions
    if(left_motor_speed == 0) {
        motor_left.brake(true);
    } else if(left_motor_speed > 0) {
        motor_left.brake(false);
        motor_left.forward();
    } else {
      motor_left.brake(false);
        motor_left.backward();
    }

    if(right_motor_speed == 0) {
        motor_right.brake(true);
    } else if(right_motor_speed > 0) {
        motor_right.brake(false);
        motor_right.forward();
    } else {
        motor_right.brake(false);
        motor_right.backward();
    }

    motor_left.speed(abs(left_motor_speed));
    motor_right.speed(abs(right_motor_speed));
}

void handler(int signo)
{
    motor_left.brake(true);
    motor_right.brake(true);

    //exit(0);
}