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
  init_motors();
  Serial.begin(9600);
  Serial.setTimeout(33);
}

void loop() {
  inStr = Serial.readStringUntil('\r');
  int idx_0, idx_1;
  
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
    }
  }
  left_wheel_pulse();
  right_wheel_pulse();
}
