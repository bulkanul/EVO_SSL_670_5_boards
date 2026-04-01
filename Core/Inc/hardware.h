#ifndef HARDWARE_H
#define HARDWARE_H

#include "global.h"

#include <stdbool.h>

void power_on (device_struct *dev);
void power_off (device_struct *dev);

bool is_alarm_phase (void);
bool is_alarm_emergency (void);
bool is_alarm_keylock (void);
bool is_alarm_interlock_1 (void);
bool is_alarm_interlock_2 (void);
bool is_alarm_stop (void);

void stop_on (void);
void stop_off (void);

#endif // HARDWARE_H
