#ifndef __ABOT_HPP__
#define __ABOT_HPP__

#include <Arduino.h>

#include "ABotConfiguration.hpp"
#include "ABotDefinitions.hpp"
#include "Motor.hpp"


void init_encoders();
void init_motors();
void cal_left_wheel_pulse();
void cal_right_wheel_pulse();
void set_motor_speeds(double left_wheel_command, double right_wheel_command);
String read_encoder_values();

void handler(int signo);
void set_pid_values(int pid_p, int pid_i, int pid_d);
void process_pid_controller();
#endif //__MABOT_HPP__
