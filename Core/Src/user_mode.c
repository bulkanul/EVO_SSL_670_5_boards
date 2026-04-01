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
	set_usr_ontask_state (false);
	osThreadTerminate (NULL);
}

void h_usr_offtask(const void *argument)
{
	device_struct * mcs = &mcs_storage;
	full_off_devices(mcs);
	clear_status_device(mcs);

	set_usr_offtask_state(false);
	osThreadTerminate(NULL);
}

void create_usr_offtaskHandle(void)
{
	if(get_usr_offtask_state() == false){
		set_usr_offtask_state(true);
		usr_offtaskHandle = osThreadCreate(osThread(usr_offtask), NULL);
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
			response ("lrstatus usr %i %i %i %i %i %i %i %i\r\n",
					id, //i
					mcs->state.bits.interlock_1,//i
					mcs->state.bits.interlock_2,//i
					mcs->state.bits.emergency,  //i
					mcs->state.bits.keylock,	 //i
					mcs->state.bits.phase_not_ok, //i
					mcs->state.bits.stop,
					mcs->alarms.val
			);
		} else if (cmd ("lserrclr usr")) {

#if SEED_GEN_COUNT > 0
			gen_set_flags_reset (&mcs->gen);
#endif
#if PREAMP_COUNT > 0
			for (size_t i = 0; i < PREAMP_COUNT; i++)
				preamp_set_flags_reset (&mcs->preamp[i]);
#endif
#if AMP_COUNT > 0
			for (size_t i = 0; i < AMP_COUNT; i++)
				amp_reset_flags (&mcs->amp[i]);
#endif
#if DIVIDE_COUNT > 0
			divide_reset_flags (mcs,resp,debug_buffer,tcp_buffer,i);
#endif

			if (mcs->alarms.bits.stop)
				stop_off();

			mcs->alarms.val = 0;

			response ("lrerrclr usr %i\r\n", id);
		} else if (cmd ("lsinitall usr")) {
#if AMP_COUNT > 0
			for (int i = 0; i < AMP_COUNT; i++){
				mcs->amp[i].can_err_count = 30;
				mcs->amp[i].available = 1;
			}
#endif

#if PREAMP_COUNT > 0
			for (int i = 0; i < PREAMP_COUNT; i++){
				mcs->preamp[i].can_err_count = 30;
				mcs->preamp[i].available = 1;
			}
#endif

#if DIVIDE_COUNT > 0
			mcs->div[0].can_err_count = 30;
			mcs->div[0].available = 1;
#endif

#if SEED_GEN_COUNT > 0
			mcs->gen.can_err_count = 30;
			mcs->gen.available = 1;
#endif

			response ("lrinitall usr %i\r\n", id);
		} else if (cmd ("lsstop usr")) {
			rd ("lsstop usr %i %i", &id, &i_val);

			if (i_val != 0 && i_val != 1)
				err++;

			if (err == 0)
				(i_val == 1) ? stop_on() : stop_off();

			response ("lrstop usr %i %i\r\n", id, i_val);
		}
	}
}
