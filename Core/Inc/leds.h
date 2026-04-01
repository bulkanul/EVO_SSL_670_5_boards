#ifndef LEDS_H
#define LEDS_H

typedef void (*led_ctrl_t) (void);

typedef struct led_s {
    led_ctrl_t on;
    led_ctrl_t off;
} led_t;

typedef struct board_s {
    led_t red;
    led_t green;
    led_t yellow;
} board_t;

typedef struct panel_s {
    led_t power;
    led_t emission;
    led_t error;
} panel_t;

typedef struct tower_s {
    led_t red;
    led_t green;
    led_t yellow;
} tower_t;

typedef struct leds_s {
    board_t board;
    panel_t panel;
    tower_t tower;
} leds_t;

void leds_init (leds_t* leds);

#endif // LEDS_H
