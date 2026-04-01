#include "hardware.h"

void power_on (device_struct *dev)
{
	HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_SET);
	dev->leds.panel.power.on();
}

void power_off (device_struct *dev)
{
	HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_RESET);
	dev->leds.panel.power.off();
}

bool is_alarm_phase (void)
{
	return HAL_GPIO_ReadPin (Phase_OK_GPIO_Port, Phase_OK_Pin) == GPIO_PIN_RESET;
}

bool is_alarm_emergency (void)
{
	return HAL_GPIO_ReadPin (EMERGENCY_GPIO_Port, EMERGENCY_Pin) == GPIO_PIN_RESET;
}

bool is_alarm_keylock (void)
{
	return HAL_GPIO_ReadPin (Keylock_GPIO_Port, Keylock_Pin) == GPIO_PIN_RESET;
}

bool is_alarm_interlock_1 (void)
{
	return HAL_GPIO_ReadPin (Interlock1_GPIO_Port, Interlock1_Pin) == GPIO_PIN_RESET;
}

bool is_alarm_interlock_2 (void)
{
	return HAL_GPIO_ReadPin (Interlock2_GPIO_Port, Interlock2_Pin) == GPIO_PIN_RESET;
}

bool is_alarm_stop (void)
{
	return HAL_GPIO_ReadPin (STOP_GPIO_Port, STOP_Pin) == GPIO_PIN_RESET;
}

void stop_on (void)
{
	return HAL_GPIO_WritePin (STOP_GPIO_Port, STOP_Pin, GPIO_PIN_RESET);
}

void stop_off (void)
{
	return HAL_GPIO_WritePin (STOP_GPIO_Port, STOP_Pin, GPIO_PIN_SET);
}
