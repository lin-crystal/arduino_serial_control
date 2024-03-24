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
int k_p, k_d, k_i;
char inChar;
String inStr;


void setup() {
  init_encoders();
  init_motors();
  Serial.begin(9600);
  Serial.setTimeout(35);

  attachInterrupt(digitalPinToInterrupt(ABOT_PIN_MOTOR_LEFT_IN_A), cal_left_wheel_pulse, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ABOT_PIN_MOTOR_RIGHT_IN_B), cal_right_wheel_pulse, CHANGE);

  // initialize timer 5 to trigger the PID controller
  initTimer5();
}

void loop() {
  inStr = Serial.readStringUntil('\r');
  int idx_0, idx_1, idx_2;
  
  if (inStr.length() == 0) {
    Serial.println();
  } else {
    inChar = inStr[0];    
    if (inChar == 'e') {
      Serial.println(read_encoder_values());
    } else if (inChar == 'm') {
      idx_0 = inStr.indexOf(' ');
      idx_1 = inStr.indexOf(' ', idx_0 + 1);
      left_comd = inStr.substring(idx_0 + 1, idx_1).toDouble();
      right_cmd = inStr.substring(idx_1 + 1, inStr.length()).toDouble();
      set_motor_speeds(left_comd, right_cmd);
    } else if (inChar == 'u') {
      idx_0 = inStr.indexOf(' ');
      idx_1 = inStr.indexOf(':', idx_0 + 1);
      idx_2 = inStr.indexOf(':', idx_1 + 1);
      k_p = inStr.substring(idx_0 + 1, idx_1).toInt();
      k_i = inStr.substring(idx_1 + 1, idx_2).toInt();
      k_d = inStr.substring(idx_2 + 1, inStr.length()).toInt();
      set_pid_values(k_p, k_i, k_d);
    }
  }
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