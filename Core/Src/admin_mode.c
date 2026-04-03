#include "../Inc/admin_mode.h"
#include "../Inc/tools.h"
#include "math.h"

void admin_command(device_struct* mcs, char* resp, char* debug_buffer, char* tcp_buffer, int i)
{
	int err = 0, id;
	id = check_type_id(tcp_buffer, "admn", ADMN_COUNT);
	if (id != -1) {
		config_struct* conf = &mcs->config;

//-----------REQUEST SET CONF ----------------
		if (cmd("lsconf admn")) {
			rd("lsconf admn %i %i %f %i %f %i %f %i %f %f\r\n",
				&id,
				&conf->tec_onoff[0],
				&conf->tec_temp [0],
				&conf->tec_onoff[1],
				&conf->tec_temp [1],
				&conf->tec_onoff[2],
				&conf->tec_temp [2],
				&conf->tec_onoff[3],
				&conf->tec_temp [3],
				&conf->hpld1000_curr [0]
			);
			response("lrconf admn %i %i %f %i %f %i %f %i %f %f\r\n",
				id,
				conf->tec_onoff[0],
				conf->tec_temp [0],
				conf->tec_onoff[1],
				conf->tec_temp [1],
				conf->tec_onoff[2],
				conf->tec_temp [2],
				conf->tec_onoff[3],
				conf->tec_temp [3],
				conf->hpld1000_curr [0]
			);
		}
//-----------REQUEST GET CONF ----------------
		else if (cmd("lgconf admn")) {
			response("lrconf admn %i %i %f %i %f %i %f %i %f %f\r\n",
				id,
				conf->tec_onoff[0],
				conf->tec_temp [0],
				conf->tec_onoff[1],
				conf->tec_temp [1],
				conf->tec_onoff[2],
				conf->tec_temp [2],
				conf->tec_onoff[3],
				conf->tec_temp [3],
				conf->hpld1000_curr [0]
			);
		}
//-----------REQUEST SET SAVE CONF ------------
		else if(cmd("lssave admn")){
			err += save_last_config_data(conf, sizeof(config_struct));
			response("lrsave admn %i OK\r\n",id);
		}

		if (err != 0)
			err_cmd(resp, tcp_buffer, id);

	}
}
