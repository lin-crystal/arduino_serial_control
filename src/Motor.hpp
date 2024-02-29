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

#ifndef _MOTOR_HPP_
#define _MOTOR_HPP_

class Motor {
    private:
        int _pin_direction;
        int _pin_brake;
        int _pin_speed;

    public:
        Motor();

        void init(int pin_direction, int pin_brake, int pin_speed);

        void forward();
        void backward();
        void brake(bool activate);
        void speed(int speed);
        
        int direction();
        int directionPin();
        int brakePin();
        int speedPin();
};

#endif // _MOTOR_HPP_
