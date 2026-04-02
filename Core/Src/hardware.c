#include "hardware.h"

void PS_Enable_on_override (device_struct *dev)
{
	HAL_GPIO_WritePin(PS_ENABLE_GPIO_Port, PS_ENABLE_Pin, GPIO_PIN_SET);
	dev->leds.panel.power.on();
}

void PS_Enable_off_override (device_struct *dev)
{
	HAL_GPIO_WritePin(PS_ENABLE_GPIO_Port, PS_ENABLE_Pin, GPIO_PIN_RESET);
	dev->leds.panel.power.off();
}

bool is_alarm_emergency (void)
{
	return HAL_GPIO_ReadPin (EMERGENCY_GPIO_Port, EMERGENCY_Pin) == GPIO_PIN_RESET;
}

bool is_alarm_keylock (void)
{
	return HAL_GPIO_ReadPin (Keylock_GPIO_Port, Keylock_Pin) == GPIO_PIN_RESET;
}

bool is_alarm_interlock (void)
{
	return HAL_GPIO_ReadPin (INTERLOCK_GPIO_Port, INTERLOCK_Pin) == GPIO_PIN_RESET;
}

bool is_alarm_chiller_interlock(void)
{
	return HAL_GPIO_ReadPin (CHILLER_INTERLOCK_GPIO_Port, CHILLER_INTERLOCK_Pin) == GPIO_PIN_RESET;
}
