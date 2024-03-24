#include "MotorEncoder.hpp"


// Initialize Motor
Motor motor_left = Motor();
Motor motor_right = Motor();

int left_motor_speed, right_motor_speed;

// Initialize pulse counters
volatile int left_wheel_pulse_count = 0;
volatile int left_wheel_pulse_count_old = LOW;
volatile int right_wheel_pulse_count = 0;
volatile int right_wheel_pulse_count_old = LOW;

// variables used by the PID controller
volatile int previousTicksLeft  = 0;
volatile int e_old_left         = 0;
volatile int integral_left      = 0;

volatile int previousTicksRight = 0;
volatile int e_old_right        = 0;
volatile int integral_right     = 0;

int KP_LEFT=1.5, KI_LEFT=1, KD_LEFT=0, KP_RIGHT=1.5, KI_RIGHT=1, KD_RIGHT=0;


void init_encoders() {
  pinMode(ABOT_PIN_MOTOR_LEFT_IN_A, INPUT);
  pinMode(ABOT_PIN_MOTOR_LEFT_IN_B, INPUT);

  pinMode(ABOT_PIN_MOTOR_RIGHT_IN_A, INPUT);
  pinMode(ABOT_PIN_MOTOR_RIGHT_IN_B, INPUT);

  
}

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
void cal_left_wheel_pulse()
{   
    // left wheel direction 
    // 1 - forward
    // 0 - backward
    int a;
    int b;

    a = digitalRead(ABOT_PIN_MOTOR_LEFT_IN_A);
    b = digitalRead(ABOT_PIN_MOTOR_LEFT_IN_B);

    if (b != left_wheel_pulse_count_old) {
      // forward
      left_wheel_pulse_count += 1;
    } else {
      // backward
      left_wheel_pulse_count -= 1;
    }
    left_wheel_pulse_count_old = a;
}

// Right wheel callback function
void cal_right_wheel_pulse()
{
    // right wheel direction 
    // 1 - forward,  
    // 0 - backward

    int a;
    int b;

    a = digitalRead(ABOT_PIN_MOTOR_RIGHT_IN_A);
    b = digitalRead(ABOT_PIN_MOTOR_RIGHT_IN_B);

    if (b != right_wheel_pulse_count_old) {
      // forward
      right_wheel_pulse_count += 1;
    } else {
      // backward
      right_wheel_pulse_count -= 1;
    }
    right_wheel_pulse_count_old = a;
}

// Set each motor speed from the respective velocity command interface
void set_motor_speeds(double left_wheel_command, double right_wheel_command)
{   

    left_motor_speed = ceil(left_wheel_command * 18);
    right_motor_speed = ceil(right_wheel_command * 18);

    // Set motor directions
    if(int(left_motor_speed) == 0) {
        motor_left.brake(true);
    } else if(left_motor_speed > 0) {
        motor_left.brake(false);
        motor_left.forward();
    } else {
        motor_left.brake(false);
        motor_left.backward();
    }

    if(int(right_motor_speed) == 0) {
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

    exit(0);
}

void set_pid_values(int pid_p, int pid_i, int pid_d) {
  KP_LEFT = pid_p;
  KI_LEFT = pid_i;
  KD_LEFT = pid_d;
  KP_RIGHT = pid_p;
  KI_RIGHT = pid_i;
  KD_RIGHT = pid_d;
}

void process_pid_controller() {
  int actTicksDiffLeft;
  int readTicksLeft;
  int e_left;
  int r_mot_left;

  int actTicksDiffRight;
  int readTicksRight;
  int e_right;
  int r_mot_right; 

  //Serial.print("  readTicksLeft = ");Serial.print(left_wheel_pulse_count);
  //Serial.print(", readTicksRight = ");Serial.print(right_wheel_pulse_count);
  
  // determine ticks since last cycle
  readTicksLeft     = left_wheel_pulse_count;
  actTicksDiffLeft  = readTicksLeft - previousTicksLeft;
  previousTicksLeft = readTicksLeft;

  readTicksRight    = right_wheel_pulse_count;
  actTicksDiffRight = readTicksRight - previousTicksRight;
  previousTicksRight = readTicksRight;
  
  //Serial.print("  actTicksDiffLeft = ");Serial.print(actTicksDiffLeft);
  //Serial.print(", actTicksDiffRight = ");Serial.print(actTicksDiffRight);
  
  e_left = DESIRED_TICKS_DIFF_LEFT - abs(actTicksDiffLeft);

  // integrate error sum and handle wind-up effect
  integral_left += e_left;
  integral_left = max(integral_left, -WIND_UP);
  integral_left = min(integral_left,  WIND_UP);

  // now do PID control
  r_mot_left = KP_LEFT * e_left
            + KI_LEFT * integral_left
            + KD_LEFT * (e_left - e_old_left);

  // remember the values for the next cycle
  e_old_left = e_left;

  // limit the motor output to the range allowed with analogWrite()
  if (r_mot_left > 255) r_mot_left = 255;
  if (r_mot_left < 0)   r_mot_left = 0;  

  // control right wheel
  e_right = DESIRED_TICKS_DIFF_RIGHT - abs(actTicksDiffRight);

  // integrate error sum and handle wind-up effect
  integral_right += e_right;
  integral_right   = max(integral_right, -WIND_UP);
  integral_right   = min(integral_right,  WIND_UP);

  // now do PID control
  r_mot_right = KP_RIGHT * e_right
              + KI_RIGHT * integral_right
              + KD_RIGHT * (e_right - e_old_right);

  // remember the values for the next cycle
  e_old_right = e_right;

  // limit the motor output to the range allowed with analogWrite()
  if (r_mot_right > 255) r_mot_right = 255;
  if (r_mot_right < 0)   r_mot_right = 0;
  
  //Serial.print(";  r_mot_left = ");Serial.print(r_mot_left);
  //Serial.print(", r_mot_right = ");Serial.println(r_mot_right);

  // now set new motor values
  if (abs(left_motor_speed) < r_mot_left) motor_left.speed(r_mot_left);
  if (abs(right_motor_speed) < r_mot_right) motor_right.speed(r_mot_right);
}
