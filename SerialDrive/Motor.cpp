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

#include "Motor.hpp"
#include "ABotDefinitions.hpp"

#include <Arduino.h>

Motor::Motor() {
    _pin_direction = -1;
    _pin_brake = -1;
    _pin_speed = -1;
}

void Motor::init(int pin_direction, int pin_brake, int pin_speed) {
        _pin_direction = pin_direction;
        _pin_brake = pin_brake;
        _pin_speed = pin_speed;
        pinMode(_pin_direction, OUTPUT);
        pinMode(_pin_brake, OUTPUT);
        forward();
        brake(false);
        speed(0);
}

void Motor::forward() {
    if(_pin_direction < 0) return;
    digitalWrite(_pin_direction, LOW);
}

void Motor::backward() {
    if(_pin_direction < 0) return;
    digitalWrite(_pin_direction, HIGH);
}

void Motor::brake(bool activate) {
    if(_pin_brake < 0) return;
    if(activate) {
        digitalWrite(_pin_brake, HIGH);
    } else {
        digitalWrite(_pin_brake, LOW);
    }
}

void Motor::speed(int speed) {
    if(_pin_speed > ABOT_MOTOR_SPEED_MAX) {
        analogWrite(_pin_speed, ABOT_MOTOR_SPEED_MAX);
    } else {
      analogWrite(_pin_speed, speed);
    }
}

int Motor::direction() {
  if (digitalRead(_pin_brake) == 0)
    return !digitalRead(_pin_direction);
  else
    return -1;
}

int Motor::directionPin() {
    return _pin_direction;
}

int Motor::brakePin() {
    return _pin_brake;
}

int Motor::speedPin() {
    return _pin_speed;
}
