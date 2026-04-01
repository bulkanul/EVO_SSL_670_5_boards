#ifndef USER_MODE_DEF
#define USER_MODE_DEF

#include "global.h"
#ifndef TB_DEF
#endif
void usr_ontask_h(void *argument);
void usr_offtask_h(void *argument);
int output_power_control(device_struct *mcs, float *ret_value, int reset);
float calculate_polinom_value(device_struct *mcs, float value, int type,int iDev);
void user_mode_prepare(void);
void create_usr_offtaskHandle(void);

void user_command (device_struct *mcs, char* resp, char* debug_buffer, char* tcp_buffer, int i);

#endif

