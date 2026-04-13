#include "../Inc/user_mode.h"
#include "../Inc/tools.h"
#include "../Inc/hardware.h"

#if AMP_COUNT > 0
	#include "../Inc/drivers/amp.h"
#endif
#if PREAMP_COUNT > 0
	#include "../Inc/drivers/preamp.h"
#endif

#if SEED_GEN_COUNT > 0
	#include "../Inc/drivers/generator.h"
#endif

#include <stddef.h>

extern device_struct mcs_storage;

int usr_on_task_state = false;
int usr_on_task_terminator = false;
int usr_off_task_state = false;
int usr_off_task_terminator = false;

osThreadId usr_ontaskHandle;
osThreadId usr_offtaskHandle;

void h_usr_ontask(const void *argument );
void h_usr_offtask(const void *argument);

osThreadDef(usr_ontask, h_usr_ontask, osPriorityNormal, 1, 256);
osThreadDef(usr_offtask, h_usr_offtask, osPriorityNormal, 1, 256);

void h_usr_ontask(const void *argument )
{
	device_struct *mcs = &mcs_storage;
	uint8_t err = 0;

#if HPLD_1000_COUNT > 0
	if(err == 0)
	{
		protection_err_clr(mcs);
		if(mcs->hpld_1000[0].state.flags != 0)
			err++;
	}

#if HPLD_1000_COUNT > 0
	if(err == 0)
	{
		int tmp_hpld_1000_mode = 0;
		err = hpld_1000_set_state_and_check(&mcs->hpld_1000[0],
				1, // onoff
				tmp_hpld_1000_mode, // mode
				mcs->config.hpld1000_curr[0]);
	}
#endif

	if(!err) osDelay(AFTER_START_DELAY*1000);
	long start_tecs_check_tick = osKernelSysTick();
	while(mcs->user_mode.ld_tec_not_ready && !err) {
		osDelay(100);
		if(osKernelSysTick() - start_tecs_check_tick > AFTER_START_TEMP_STABILIZE_TIME*1000)
			err ++;
	}

//  FIXME
	if(!err) EVO_SSL_670_15_CONTROL_433739_065_set_onoff(&mcs->cb[0], 1);
	if(!err) EVO_SSL_670_15_CONTROL_433739_065_set_hv_en(&mcs->cb[0], 0, 1);
	if(!err) EVO_SSL_670_15_CONTROL_433739_065_set_hv_en(&mcs->cb[0], 1, 1);

	if (err == 0)
		while (get_usr_ontask_terminator() == false && mcs->hpld_1000[0].state.flags == 0 && err == 0)
		{
			err += get_error(mcs);
			get_emission(mcs); // is for update output power value
			osDelay(100);
		}
#endif

//  FIXME
	EVO_SSL_670_15_CONTROL_433739_065_set_hv_en(&mcs->cb[0], 0, 0);
	EVO_SSL_670_15_CONTROL_433739_065_set_hv_en(&mcs->cb[0], 1, 0);
	EVO_SSL_670_15_CONTROL_433739_065_set_onoff(&mcs->cb[0], 0);

#if HPLD_1000_COUNT > 0
	for(uint16_t i = 0; i < HPLD_1000_COUNT; i++)
		hpld_1000_set_state_off(&mcs->hpld_1000[i]);
#endif

	set_usr_ontask_state(false);
  	osThreadTerminate(NULL);
}

void h_usr_offtask(const void *argument)
{
	device_struct *mcs = &mcs_storage ;
//	set_pocels_state(mcs, 0); // turn on pocels cell
#if HPLD_1000_COUNT > 0
	for(uint16_t i = 0; i < HPLD_1000_COUNT; i++)
		hpld_1000_set_state_off(&mcs->hpld_1000[i]);
#endif

	EVO_SSL_670_15_CONTROL_433739_065_set_hv_en(&mcs->cb[0], 0, 0);
	EVO_SSL_670_15_CONTROL_433739_065_set_hv_en(&mcs->cb[0], 1, 0);
	EVO_SSL_670_15_CONTROL_433739_065_set_onoff(&mcs->cb[0], 0);

	set_usr_offtask_state(false);
	osThreadTerminate(NULL);
}

void create_usr_offtaskHandle()
{
	if(get_usr_offtask_state() == false){
		set_usr_offtask_state(true);
		usr_offtaskHandle = osThreadCreate(osThread(usr_offtask), NULL);
	}
}

void create_usr_ontaskHandle()
{
	if(get_usr_ontask_state() == false){
		set_usr_ontask_state(true);
		usr_ontaskHandle = osThreadCreate(osThread(usr_ontask), NULL);
	}
}

void user_command (device_struct *mcs, char* resp, char* debug_buffer, char* tcp_buffer, int i)
{
	UNUSED(debug_buffer);
	int id = check_type_id(tcp_buffer, "usr", 1);
	if (id != -1) {
		int i_val;
		int err = 0;

		//-------------REQUEST TO GET LASER STATUS IS USER MODE-----------
		if (cmd ("lgstatus usr")) {
			response ("lrstatus usr %i %i %i %i %i %i %i %i %i %i %i %f %f %f %f %f %f\r\n",
					id,
					mcs->alarms.bits.interlock,
					mcs->alarms.bits.interlock_chiller,
					mcs->user_mode.overheat,
					mcs->user_mode.ld_tec_not_ready,
					mcs->hpld_1000[0].state.started_state,
					mcs->user_mode.PSU_permission,
					mcs->user_mode.PSU_state,
					mcs->user_mode.output_started,
					mcs->cb[0].conf.quantron.mode,
					mcs->cb[0].state.divider,
					mcs->cb[0].state.temp[0],
					mcs->cb[0].state.temp[1],
					mcs->tec3[0].state.temp,
					mcs->tec3[1].state.temp,
					mcs->tec3[2].state.temp,
					mcs->tec3[3].state.temp
			);
		}
		else if (cmd ("lsonoff usr")) {
			rd("lsonoff usr %i %d", &id, &i_val);
			err += (i_val < 0 || i_val > 1);
			if(i_val == 1)
				err += (mcs->user_mode.PSU_permission == 0);

			if(!err)
			{
				user_mode_prepare();
				if (i_val)
					create_usr_ontaskHandle();
				else
					create_usr_offtaskHandle();
			}
			response("lronoff usr %i %d\r\n", id, i_val);
		}
		else if (cmd ("lserrclr usr")) {
			mcs->alarms.val = 0;
			protection_err_clr(mcs);
			response ("lrerrclr usr %i\r\n", id);
		}
		else if (cmd ("lspsupermission usr")) {
			rd ("lspsupermission usr %i %i", &id, &i_val);
			err += (i_val != 0 && i_val != 1);
			if (!err)
			{
				if(i_val == 1)
					PS_Enable_on_override(mcs);
				else
					PS_Enable_off_override(mcs);
				mcs->user_mode.PSU_permission = i_val;
			}
			response ("lrpsupermission usr %i %i\r\n", id, i_val);
		}
		//-----------REQUEST TO SET QUANTRON MODE---------
		else if(cmd("lsmode usr")){
			rd("lsmode usr %i %i\r\n", &id, &i_val);
			err += (i_val < 0 || i_val > 1);
			if(!err)
			    err += EVO_SSL_670_15_CONTROL_433739_065_set_mode(&mcs->cb[0], i_val);
			response("lrmode usr %i %i\r\n", id, i_val);
		}
		//-----------REQUEST TO SET QUANTRON MODE---------
		else if(cmd("lsdiv usr")){
			rd("lsdiv usr %i %i\r\n", &id, &i_val);
			err += (i_val < 1 || i_val > 254);
			if(!err)
			    err += EVO_SSL_670_15_CONTROL_433739_065_set_divider(&mcs->cb[0], i_val);
			response("lrdiv usr %i %i\r\n", id, i_val);
		}
		//-----------REQUEST TO SET QUANTRON MODE---------
		else if(cmd("lsfreq usr")){
			rd("lsfreq usr %i %i\r\n", &id, &i_val);
			err += (i_val < 0 || i_val > 1000000);
			if(!err)
			    err += EVO_SSL_670_15_CONTROL_433739_065_set_freq(&mcs->cb[0], i_val);
			response("lrfreq usr %i %i\r\n", id, i_val);
		}
//		else if (cmd ("lsinitall usr")) {
//#if HPLD_1000_COUNT > 0
//			for (int i = 0; i < HPLD_1000_COUNT; i++){
//				mcs->hpld_1000[i].can_err_count = 30;
//				mcs->hpld_1000[i].available = 1;
//			}
//#endif
//
//#if TEC3_COUNT > 0
//			for (int i = 0; i < TEC3_COUNT; i++){
//				mcs->tec3[i].can_err_count = 30;
//				mcs->tec3[i].available = 1;
//			}
//#endif
//			response ("lrinitall usr %i\r\n", id);
//		}
		if(err != 0)
			err_cmd(resp,tcp_buffer,id);
	}
}
