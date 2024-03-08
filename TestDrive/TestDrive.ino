// Copyright (C) 2021 Christian Müller, Christian Schlegel
//
// christian.schlegel@thu.de
//
// Servicerobotics Ulm
// Technische Hochschule Ulm
// Prittwitzstrasse 10
// 89075 Ulm, Germany
//
// This program and the accompanying materials are made available under the terms
// of the BSD-3-Clause which is available at https://opensource.org/licenses/BSD-3-Clause
//
// SPDX-License-Identifier: BSD-3-Clause

#include "MotorEncoder.hpp"

double left_comd, right_cmd;
char inChar;
String inStr;


void setup() {
  init_encoders();
  init_motors();
  Serial.begin(9600);
  //Serial.setTimeout(33);
  set_pid_values(2, 1, 0);

  attachInterrupt(digitalPinToInterrupt(ABOT_PIN_MOTOR_LEFT_IN_A), left_wheel_pulse, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ABOT_PIN_MOTOR_RIGHT_IN_B), right_wheel_pulse, CHANGE);

  // initialize timer 5 to trigger the PID controller
  initTimer5();
}

void loop() {

}


void initTimer5(void) {
  TCCR5A = 0;
  TCCR5B = 0;

  TCCR5B  = (1<<WGM52) | (1<<CS52);
  OCR5A   = 6250;
  TIMSK5 |= (1<<OCIE5A);
  sei();
} 

ISR(TIMER5_COMPA_vect) {
  process_pid_controller();
}