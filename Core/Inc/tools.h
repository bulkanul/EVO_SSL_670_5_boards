#ifndef TOOLS_DEF
#define TOOLS_DEF

#include <stdbool.h>

#include "global.h"
#ifndef TB_DEF
#include "api.h"
#endif
#include "usart.h"

#define response(...)\
do {if(err == 0)\
	sprintf(resp,__VA_ARGS__);} while(0)

#define response_err(...)\
do {if(err != 0)\
	sprintf(resp,__VA_ARGS__);} while(0)

#define cmd(...) \
	cmd_compare((tcp_buffer+i),__VA_ARGS__)

#define rd(...)\
	sscanf(&tcp_buffer[i],__VA_ARGS__)

int  cmd_compare(char* buf, const char* cmd);
void debug_mess(char* mess);
void send_response ( int interface_in , char *resp );
void read_last_config_data(config_struct *dest_struct);
void default_conf ( config_struct *dest_struct );
int  save_last_config_data(config_struct* source_struct, int conf_struct_size);
void protection_reset(void);
float abs_f(float c);
void indication_handler(QueueHandle_t* indication_queue,device_struct* mcs);
void alarm_and_state_handler(device_struct *mcs);
int get_emission(device_struct* mcs);
void clear_status_device(device_struct* mcs);

void user_mode_prepare(void);
int check_type_id (char* data_in,char* type,int max_id);
void err_cmd(char* resp,char* data_in,int id);

#define get(name,type) type get_##name();
#define set(name,type) void set_##name(type _name);

get(usr_ontask_terminator,uint8_t)
get(usr_ontask_state,uint8_t)
get(usr_offtask_state,uint8_t)
get(usr_offtask_terminator,uint8_t)
set(usr_ontask_terminator,uint8_t)
set(usr_ontask_state,uint8_t)
set(usr_offtask_state,uint8_t)
set(usr_offtask_terminator,uint8_t)

get(console_mode,int)
set(console_mode,int)

get(interface_in,int)
set(interface_in,int)

get(CONF_IP,int*)

#undef get
#undef set

void full_off_devices(device_struct* mcs);

#endif //TOOLS_DEF
