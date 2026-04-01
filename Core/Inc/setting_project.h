#ifndef SETTING_PROJECT_H
#define SETTING_PROJECT_H

//#define DEBUG_ENABLE
//#define DEMO
//#define DEBUG_OVER_TCP 1

#define SERVER_PORT 7878 // NN

/* Change this section according to the device -------------------------------*/
#define PROTOCOL_VERSION "EVO_YFL_1064_FW Main_board_03022026\r"
/* ---------------------------------------------------------------------------*/
#define POWER_COUNT 		0
#define SEED_GEN_COUNT 		1
#define AMP_COUNT 			(4)
#define DIVIDE_COUNT 		0
#define PREAMP_COUNT 		(4)

#define TCP_QUEUE_BUF_SIZE 1000
#define RECV_OK_TIMEOUT 100000
#define DELAY_CAN 0

#define	CAN_IND_REQ 200
#define	ETH_IND_REQ 201

#define IDLE_STATE 0
#define TRIGGERED_STATE 1

#define INTERFACE_RS232_UART3 300
#define INTERFACE_USB_UART1 301
#define INTERFACE_ETHERNET 302

#define TASK_TIMEOUT		300			// *10 ms
#define ERRCLR_NUMBER		200			// *10 ms

/* Change this section according to the device -------------------------------*/
#define MAX_CURRENT 25

#define PD_COUNT 1
#define PR_ALARM_PD1 0

#define MAX_CALIB_COUNT 43

#define MODE_DCDC_CW 0
#define MODE_DCDC_TTL 1

#define CAN_REFRESH_DEVICE_DELAY 10

#define EXT_DEVICE_ADDR   0x055

#endif //SETTING_PROJECT_H
