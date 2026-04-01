#include "../Inc/commands_handler.h"
#include "../Inc/tools.h"
#include "../Inc/gitcommit.h"
#include "../Inc/user_mode.h"

#include <stdint.h>

extern void gen_command(device_struct *mcs,char* resp,char* debug_buffer,char* tcp_buffer,int i);
extern void preamp_command(device_struct *mcs,char* resp,char* debug_buffer,char* tcp_buffer,int i);
extern void amp_command(device_struct *mcs,char* resp,char* debug_buffer,char* tcp_buffer,int i);
extern void divide_command(device_struct* mcs, char* resp, char* debug_buffer, char* tcp_buffer, int i);

static void common_command(device_struct *mcs, char *resp, char *debug_buffer, char *tcp_buffer, int i);

void CommandsHandler(device_struct *mcs,char* resp,char* debug_buffer,char* tcp_buffer,int i){
	common_command(mcs, resp, debug_buffer, tcp_buffer, i);
	user_command(mcs,resp,debug_buffer,tcp_buffer,i);

	gen_command(mcs,resp,debug_buffer,tcp_buffer,i);
	preamp_command(mcs,resp,debug_buffer,tcp_buffer,i);
	amp_command(mcs,resp,debug_buffer,tcp_buffer,i);

#if DIVIDE_COUNT > 0
	divide_command(mcs,resp,debug_buffer,tcp_buffer,i);
#endif

	if(resp[0] == 'l')
		send_response(mcs->current_interface, resp);
    resp[0] = 0;
}

static void common_command (device_struct *mcs, char* resp, char* debug_buffer, char* tcp_buffer, int i)
{
	UNUSED(debug_buffer);
	int err = 0;
	config_struct *conf = &mcs->config;
	//--------------REQUEST CONSOLE----------------------
	if (cmd("console")) {
		if (get_console_mode() == DISABLE) {
			set_console_mode(ENABLE);
			response("Console mode ON\r\n");
		} else if (get_console_mode() == ENABLE) {
			set_console_mode(DISABLE);
			response("Console mode OFF\r\n");
		}
	}
	//----------------GET PROTOCOL VERSION------------------
	else if (cmd("lgvers"))
		response("lrvers %s\r\n", PROTOCOL_VERSION);
	else if (cmd("lghash"))
		response("lrhash %s\r\n", GIT_INFO);
	//--------------SET CURRENT IP ADDRESS REQUEST-----------
	else if (cmd("lsip")){
		rd("lsip %d.%d.%d.%d", (int*)&conf->ip[0],(int*)&conf->ip[1],(int*)&conf->ip[2],(int*)&conf->ip[3]);

		err += save_last_config_data(&mcs->config, sizeof(config_struct));

		response("lrip  %d.%d.%d.%d\r\n", (int)conf->ip[0],(int)conf->ip[1],(int)conf->ip[2],(int)conf->ip[3]);
		response_err("lrip  %d.%d.%d.%d ERR\r\n", (int)conf->ip[0],(int)conf->ip[1],(int)conf->ip[2],(int)conf->ip[3]);
	}
	//--------GET CURRENT IP ADDRESS  REQUEST---------------
	else if (cmd("lgip"))
		response("lrip  %d.%d.%d.%d\r\n", (int)conf->ip[0],(int)conf->ip[1],(int)conf->ip[2],(int)conf->ip[3]);
	//--------------FACTORY RESET REQUEST------------------
	else if (cmd("lsfactrst")){
		default_conf(&mcs->config);

		response("lrfactrst OK\r\n");
	}
	else if(cmd("lsmac")){
		rd("lsmac %x %x %x %x %x %x" ,
				(unsigned int*)&conf->mac[0],
				(unsigned int*)&conf->mac[1],
				(unsigned int*)&conf->mac[2],
				(unsigned int*)&conf->mac[3],
				(unsigned int*)&conf->mac[4],
				(unsigned int*)&conf->mac[5]
		) ;

		err += save_last_config_data ( &mcs->config , sizeof ( config_struct ) );

		response("lrmac %x %x %x %x %x %x\r\n ",
				(unsigned int)conf->mac[0],
				(unsigned int)conf->mac[1],
				(unsigned int)conf->mac[2],
				(unsigned int)conf->mac[3],
				(unsigned int)conf->mac[4],
				(unsigned int)conf->mac[5]
		) ;

		response_err("lrmac %x %x %x %x %x %x ERR\r\n",
				(unsigned int)conf->mac[0],
				(unsigned int)conf->mac[1],
				(unsigned int)conf->mac[2],
				(unsigned int)conf->mac[3],
				(unsigned int)conf->mac[4],
				(unsigned int)conf->mac[5]
		);
	}
	else if(cmd("lgmac")){
		response("lrmac %x %x %x %x %x %x\r\n ",
			 (unsigned int)conf->mac[0],
			 (unsigned int)conf->mac[1],
			 (unsigned int)conf->mac[2],
			 (unsigned int)conf->mac[3],
			 (unsigned int)conf->mac[4],
			 (unsigned int)conf->mac[5]
		);
	}
	else if (cmd("lsbootloader")){
		rd("lrbootloader ON\r\n");
#define SHARED_VAR_ADDR  ((uint32_t*)0x2001F000)
#define SHARED_VAR       (*SHARED_VAR_ADDR)
		SHARED_VAR = 1;
		NVIC_SystemReset();
	}
}
