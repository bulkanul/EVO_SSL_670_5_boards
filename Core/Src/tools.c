#include "../Inc/tools.h"
#ifndef TB_DEF
#endif
#include "../Inc/commands_handler.h"
#include "../Inc/server.h"
#include "../Inc/hardware.h"
#include "math.h"
#include "assert.h"


static int interface_in   = INTERFACE_ETHERNET;
static int console_mode = DISABLE;
static int CONF_IP [ 4 ] ;

static uint8_t usr_ontask_terminator  = false;
static uint8_t usr_ontask_state       = false;
static uint8_t usr_offtask_state 	  = false;
static uint8_t usr_offtask_terminator = false;

#define get(name,type) type get_##name(){ return name;}
#define set(name,type) void set_##name(type _name){  name = _name;}

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

#ifndef TB_DEF

#else
#define HANDLER_POWER(...)	
#define handler_power(...) 
#endif

int check_type_id(char* data_in,char* type,int max_id){
	int id = -1;
	int err = 0;
	char* text_type = strstr(data_in," ");
//// we add space to know that cmd is properly separated
//	char type[32];
//	strcpy(type, _type);
//	type[strlen(type)] = ' ';
////
	if(text_type !=0)
	    err = cmd_compare(++text_type, type) == 0;
	else
	    err++;
	if(text_type != 0 && err == 0){
		char* text_id = strstr(text_type," ");
		if(text_id != 0){
			sscanf(++text_id,"%i",&id);
			if(id >= max_id)
				id = -1;
		} else
			err++;

	} else
		err++;
	return id;
}

int cmd_compare(char* buf, const char* cmd)
{
	int size = strlen(cmd);
	for(int i = 0; i < size; i ++)
	{
		if(*(buf + i) != *(cmd + i)) return 0;
	}
	return 1;
}

void debug_mess(char* mess)
{
	char buf[1000];
	sprintf(buf, "<%lu> : ", (long)osKernelSysTick());
	*(buf + strlen(buf)) = '\0';
	if (strlen(buf) + strlen(mess) < 500)
	{
		sprintf(buf + strlen(buf), "%s", mess);
	}
	*(buf + strlen(buf)) = '\0';
#ifndef TB_DEF
#ifdef DEBUG_ENABLE			 
	for (int i = 0; i < strlen(buf); i++)
		xQueueSend(debugger_queue, &buf[i], 0);
#endif

#ifdef DEBUG_OVER_TCP
	if (console_mode == ENABLE)
		send_response(INTERFACE_ETHERNET, buf);
#endif
#else 
	taskENTER_CRITICAL();
	printf(buf);
	taskEXIT_CRITICAL();
#endif
}

void send_response ( int interface_in , char *resp )
{
	if (interface_in == INTERFACE_ETHERNET)
	{
#ifndef TB_DEF
		if (getRemoteClient() != NULL)
			netconn_write(getRemoteClient(), resp, strlen(resp), NETCONN_COPY);
#else
		netconn_write(0, resp, strlen(resp), NETCONN_COPY);

#endif

	}
	else if ( interface_in == INTERFACE_USB_UART1 )
		HAL_UART_Transmit ( &huart1 , ( uint8_t* ) resp , strlen ( resp ) , 500 ) ;
	else if ( interface_in == INTERFACE_USB_UART2 )
		HAL_UART_Transmit ( &huart2 , ( uint8_t* ) resp , strlen ( resp ) , 500 ) ;
}

void read_last_config_data(config_struct *dest_struct){
	int conf_struct_size = sizeof(config_struct);
	int err = -1;
	int repeats = 20 ;

	portENTER_CRITICAL();

	flash_read_config_data((uint32_t*)dest_struct,conf_struct_size,PARAMS_MEMORY_ADDR);
	if ( * ( ( uint32_t * ) dest_struct ) == 0xFFFFFFFF )		// It's a first run
	{//FIXME != => == 0xFFFFFFFF
		default_conf ( dest_struct ) ;
		while ( err == -1 && repeats > 0 )
		{
			err = save_last_config_data ( dest_struct , sizeof ( config_struct ) ) ;
			repeats -- ;
		}
	}
	else
		memcpy ( get_CONF_IP()  , dest_struct->ip , 16 ) ;
	portEXIT_CRITICAL();
}


void default_conf ( config_struct *dest_struct )
{
	memset(dest_struct,(unsigned char)0,sizeof(config_struct));
	dest_struct->ip[0] = 192;
	dest_struct->ip[1] = 168;
	dest_struct->ip[2] = 26;
	dest_struct->ip[3] = 220;
	dest_struct->mac[0] = 0x00;
	dest_struct->mac[1] = 0x80;
	dest_struct->mac[2] = 0xE1;
	dest_struct->mac[3] = 0x00;
	dest_struct->mac[4] = 0x00;
	dest_struct->mac[5] = 0x0B;
}


int save_last_config_data(config_struct* source_struct, int conf_struct_size)
{
	int err = !SUCCESS;
	int repeats = 5;
	portENTER_CRITICAL();
	while(err == !SUCCESS && repeats > 0)
	{
		err = flash_save_config_data((uint32_t*)source_struct, conf_struct_size, PARAMS_MEMORY_ADDR);
		repeats --;
	}
	portEXIT_CRITICAL();

	if(repeats == 0) return -1;
	return 0;
}

float abs_f(float c)
{
	return (c >= 0)? c : c*(-1);
}

void indication_handler(QueueHandle_t* indication_queue,device_struct* mcs)
{
	int green_led_counter = 0;
	int yellow_led_counter = 0;
	int red_led_counter = 0;
	uint8_t tmp = 0;

	for(;;)
	{
#ifndef TB_DEF
#endif

		if(xQueueReceive(*indication_queue,&tmp,0) == pdPASS )
		{
			if(tmp == CAN_IND_REQ)
			{
				yellow_led_counter = 0;
			}
			if(tmp == ETH_IND_REQ)
			{
				red_led_counter = 0;
			}
			tmp = 0;
		}
		/// green led blink ----------------------------------------------------------------------------------
		if(green_led_counter == 10)
		{
#ifndef TB_DEF
			mcs->leds.board.green.on();
#else
#endif
		}
		else if(green_led_counter == 200)
		{
#ifndef TB_DEF
			mcs->leds.board.green.off();
#else
#endif
			green_led_counter = 0;
		}
		green_led_counter++;
		/// green led blink ----------------------------------------------------------------------------------

		/// yellow led blink ----------------------------------------------------------------------------------
		if(yellow_led_counter < 5)
		{
#ifndef TB_DEF
			mcs->leds.board.yellow.on();
#else
#endif
			yellow_led_counter++;
		}
		else
		{
#ifndef TB_DEF
			mcs->leds.board.yellow.off();
#else
#endif
			yellow_led_counter++;
		}
		/// yellow led blink ----------------------------------------------------------------------------------

		/// red led blink ----------------------------------------------------------------------------------
		if(red_led_counter < 5)
		{
#ifndef TB_DEF
			mcs->leds.board.red.on();
#else
#endif
			red_led_counter++;
		}
		else
		{
#ifndef TB_DEF
			mcs->leds.board.red.off();
#else
#endif
			red_led_counter++;
		}
		/// red led blink ----------------------------------------------------------------------------------
		osDelay(10);
	}
}

int get_error(device_struct* mcs){
	alarms_t* alarm = &mcs->alarms;
	int err = 0;
	err += alarm->val;
	err += mcs->user_mode.overheat;
// TODO need to dicide if this section is needed
//	for(int i = 0; i < TEC3_COUNT; i ++)
//	{
//		if(mcs->config.tec_onoff[i] == 1)
//			err += (mcs->tec3[i].state.started != 1);
//	}
//	for(int i = 0; i < TEC3_COUNT; i ++)
//	{
//		if(mcs->config.tec_onoff[i] == 1)
//			err += (mcs->tec3[i].available != 1);
//	}
	return err != 0;
}

void alarm_and_state_handler (device_struct *mcs)
{
	mcs->alarms.bits.emergency         = 	is_alarm_emergency();
	mcs->alarms.bits.keylock           =    is_alarm_keylock();
	mcs->alarms.bits.interlock         = 	is_alarm_interlock();
	mcs->alarms.bits.interlock_chiller = 	is_alarm_chiller_interlock();
}

int get_emission(device_struct* mcs){
	uint8_t emission = 0;
#if HPLD_1000_COUNT > 0
	for(uint16_t i = 0; i  < HPLD_1000_COUNT; i++)
		emission += mcs->hpld_1000[i].available & mcs->hpld_1000[i].state.started_state;
#endif
	return emission != 0;
}

void form_cm_header(can_message_struct *cm, int id){
#ifdef FDCAN
	cm->tx_header.Identifier = id;
	cm->tx_header.IdType = FDCAN_STANDARD_ID;
	cm->tx_header.TxFrameType = FDCAN_DATA_FRAME;
	cm->tx_header.DataLength = FDCAN_DLC_BYTES_8;
	cm->tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	cm->tx_header.BitRateSwitch = FDCAN_BRS_OFF;
	cm->tx_header.FDFormat = FDCAN_CLASSIC_CAN;
	cm->tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	cm->tx_header.MessageMarker = 0;
#else
	cm->tx_header.StdId = id;
	cm->tx_header.DLC = 8;
	cm->tx_header.RTR = CAN_RTR_DATA;
	cm->tx_header.IDE = CAN_ID_STD;
	cm->tx_header.TransmitGlobalTime = DISABLE;
#endif
}

int protection_err_clr(device_struct *mcs)
{
	int err = 0;
	for(int i = 0 ; i < HPLD_1000_COUNT; i ++)
	{
		hpld_1000_struct *dev = &mcs->hpld_1000[i];
		if (dev->available == 1)
		{
			int reps = 5;
			while (reps-- > 0 && dev->state.flags != 0)
			{
				err += hpld_1000_rst_protection(dev);
				hpld_1000_get_flags(dev);
				osDelay(1);
			}
		}
	}
	return err;
}

void user_mode_prepare(void)
{
	if(get_usr_ontask_state() == true)
	{
		while(get_usr_ontask_state() == true)
		{
			set_usr_ontask_terminator(true);
			osDelay(10);
		}
		usr_ontask_terminator = false;
	}
	if(usr_offtask_state == true)
	{
		while(get_usr_offtask_state() == true)
		{
			set_usr_offtask_terminator(true);
			osDelay(10);
		}
		set_usr_offtask_terminator(false);
	}
	else
		return;
}

void err_cmd(char* resp,char* data_in,int id)
{
    char cmd[40];
	char type[30];
		data_in[1] = 'r';
    sscanf(data_in,"%s %s",cmd,type);
    sprintf(resp,"%s %s %i ERR\r\n",cmd,type,id);
}
