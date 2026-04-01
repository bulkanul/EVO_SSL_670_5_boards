#ifndef GLOBAL_DEF
#define GLOBAL_DEF

#include "main.h"
#include "stdio.h"
#include "string.h"
#ifndef TB_DEF
#include "params_func.h"
#endif

#include "setting_project.h"
#include "../../board_prj_driver_lib/drivers/HPLD_1000.h"
#include "../../board_prj_driver_lib/drivers/TEC_Driver_3_drv.h"

#include "leds.h"

#include <assert.h>

#ifdef DEBUG_ENABLE 
	#if DEBUG_MESSAGE_FORMAT == 0
	#define debug_message(flag,buf,...) \
		do{if(flag){sprintf(buf,__VA_ARGS__); \
		debug_mess(buf);} }while(0)
	#elif DEBUG_MESSAGE_FORMAT == 1
		#define debug_message(flag,buf,...) \
		do{if(flag){printf(__VA_ARGS__);} }while(0)
	#endif 
#else
	#define debug_message(buf,...)
#endif


#ifdef DELAY_CAN_ENABLE
	#define DELAY_CAN_DEVICE(time) osDelay(time)
#else
	#define DELAY_CAN_DEVICE(TIME)
#endif

#pragma pack(push, 1)
typedef union alarms_u {
	struct {
		uint8_t phase_not_ok : 1;
		uint8_t emergency : 1;
		uint8_t keylock : 1;
		uint8_t interlock_1 : 1;
		uint8_t interlock_2 : 1;
		uint8_t stop : 1;
		uint8_t reserved : 2;
	} bits;
	uint8_t val;
} alarms_t;
#pragma pack(pop)
static_assert(sizeof(alarms_t) == 1, "alarms_t must be exactly 1 bytes!");

#pragma pack(push, 1)
typedef union state_u {
	struct {
		uint8_t phase_not_ok : 1;
		uint8_t emergency : 1;
		uint8_t keylock : 1;
		uint8_t interlock_1 : 1;
		uint8_t interlock_2 : 1;
		uint8_t stop : 1;
		uint8_t reserved : 2;
	} bits;
	uint8_t val;
} state_t;
#pragma pack(pop)
static_assert(sizeof(state_t) == 1, "state_t must be exactly 1 bytes!");

typedef struct {
	uint32_t ip [4];
	uint32_t mac[6];

	int term_resi;
	int term_beta;
	float v_ref;

	float tec_temp                    [TEC3_COUNT];
	float tec_onoff                   [TEC3_COUNT];

	float hpld1000_curr               [HPLD_1000_COUNT];
}
config_struct;

typedef struct
{
	float preamp_power_all;
	float amp_power_all;
	int output_started;
}
user_mode_struct;

typedef struct {
	int                     current_interface;

	config_struct           config;
	user_mode_struct        user_mode;


	hpld_1000_struct      	hpld_1000			[HPLD_1000_COUNT];
	tec3_controller_struct 	tec3 				[TEC3_COUNT];

	leds_t                  leds;
	alarms_t                alarms;
	state_t                 state;
}
device_struct;

extern QueueHandle_t send_can_mess_queue;

#endif
