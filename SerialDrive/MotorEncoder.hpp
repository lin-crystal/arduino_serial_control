#ifndef __ABOT_HPP__
#define __ABOT_HPP__

#include <Arduino.h>

#include "ABotConfiguration.hpp"
#include "ABotDefinitions.hpp"
#include "Motor.hpp"

void init_motors();
void left_wheel_pulse();
void right_wheel_pulse();
void set_motor_speeds(double left_wheel_command, double right_wheel_command);
void handler(int signo);
String read_encoder_values();

#endif //__MABOT_HPP__
