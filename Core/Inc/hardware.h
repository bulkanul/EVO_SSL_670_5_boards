#ifndef HARDWARE_H
#define HARDWARE_H

#include "global.h"

#include <stdbool.h>
bool is_alarm_chiller_interlock(void);
bool is_alarm_interlock (void);
bool is_alarm_keylock (void);
bool is_alarm_emergency (void);
void PS_Enable_off_override (device_struct *dev);
void PS_Enable_on_override (device_struct *dev);

#endif // HARDWARE_H
