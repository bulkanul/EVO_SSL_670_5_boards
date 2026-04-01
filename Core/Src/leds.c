#include "leds.h"

#include "main.h"

static void led_red_on (void) { LED_RED_GPIO_Port->BSRR = (uint32_t)LED_RED_Pin; }
static void led_red_off (void) { LED_RED_GPIO_Port->BSRR = (uint32_t)LED_RED_Pin << 16U; }
static void led_green_on (void) { LED_GREEN_GPIO_Port->BSRR = (uint32_t)LED_GREEN_Pin; }
static void led_green_off (void) { LED_GREEN_GPIO_Port->BSRR = (uint32_t)LED_GREEN_Pin << 16U; }
static void led_yellow_on (void) { LED_YELLOW_GPIO_Port->BSRR = (uint32_t)LED_YELLOW_Pin; }
static void led_yellow_off (void) { LED_YELLOW_GPIO_Port->BSRR = (uint32_t)LED_YELLOW_Pin << 16U; }

static void led_power_on (void) { LED_POWER_GPIO_Port->BSRR = (uint32_t)LED_POWER_Pin; }
static void led_power_off (void) { LED_POWER_GPIO_Port->BSRR = (uint32_t)LED_POWER_Pin << 16U; }
static void led_emission_on (void) { LED_EMISSION_GPIO_Port->BSRR = (uint32_t)LED_EMISSION_Pin; }
static void led_emission_off (void) { LED_EMISSION_GPIO_Port->BSRR = (uint32_t)LED_EMISSION_Pin << 16U; }
static void led_error_on (void) { LED_ERROR_GPIO_Port->BSRR = (uint32_t)LED_ERROR_Pin; }
static void led_error_off (void) { LED_ERROR_GPIO_Port->BSRR = (uint32_t)LED_ERROR_Pin << 16U; }

static void led_yellow_tower_on (void) { Yellow_Tower_GPIO_Port->BSRR = (uint32_t)Yellow_Tower_Pin; }
static void led_yellow_tower_off (void) { Yellow_Tower_GPIO_Port->BSRR = (uint32_t)Yellow_Tower_Pin << 16U; }
static void led_green_tower_on (void) { Green_Tower_GPIO_Port->BSRR = (uint32_t)Green_Tower_Pin; }
static void led_green_tower_off (void) { Green_Tower_GPIO_Port->BSRR = (uint32_t)Green_Tower_Pin << 16U; }
static void led_red_tower_on (void) { Red_Tower_GPIO_Port->BSRR = (uint32_t)Red_Tower_Pin; }
static void led_red_tower_off (void) { Red_Tower_GPIO_Port->BSRR = (uint32_t)Red_Tower_Pin << 16U; }

void leds_init (leds_t* leds)
{
    leds->board.red.on = led_red_on;
    leds->board.red.off = led_red_off;
    leds->board.green.on = led_green_on;
    leds->board.green.off = led_green_off;
    leds->board.yellow.on = led_yellow_on;
    leds->board.yellow.off = led_yellow_off;

    leds->panel.power.on = led_power_on;
    leds->panel.power.off = led_power_off;
    leds->panel.emission.on = led_emission_on;
    leds->panel.emission.off = led_emission_off;
    leds->panel.error.on = led_error_on;
    leds->panel.error.off = led_error_off;

    leds->tower.red.on = led_red_tower_on;
    leds->tower.red.off = led_red_tower_off;
    leds->tower.green.on = led_green_tower_on;
    leds->tower.green.off = led_green_tower_off;
    leds->tower.yellow.on = led_yellow_tower_on;
    leds->tower.yellow.off = led_yellow_tower_off;
}
