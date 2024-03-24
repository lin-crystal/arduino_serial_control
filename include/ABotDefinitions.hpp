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

#ifndef _ABOT_DEFINITIONS_HPP_
#define _ABOT_DEFINITIONS_HPP_

#define ABOT_MOTOR_SPEED_MAX    255

#define WIND_UP     200
// the commanded speed in ticks / period (here ticks / 100ms as PID runs with 10 Hz)
// (see comment above: only positive speed is allowed)
#define DESIRED_TICKS_DIFF_LEFT  30
#define DESIRED_TICKS_DIFF_RIGHT 30

#endif // _ABOT_DEFINITIONS_HPP_
