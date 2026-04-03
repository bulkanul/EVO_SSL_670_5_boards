#ifndef SETTING_PROJECT_H
#define SETTING_PROJECT_H

//#define DEBUG_ENABLE
//#define DEMO
//#define DEBUG_OVER_TCP 1

#define SERVER_PORT                     7878 // NN

#define TCP_QUEUE_BUF_SIZE              1000
#define RECV_OK_TIMEOUT                 100000

/* Change this section according to the device -------------------------------*/
#define PROTOCOL_VERSION                "EVO_SSL_670_5\r"
/* ---------------------------------------------------------------------------*/

#define HPLD_1000_COUNT 				(1)
#define TEC3_COUNT 						(4)
#define CB_COUNT 		          		(1)
#define ADMN_COUNT 		          		(1)

#define	CAN_IND_REQ                      200
#define	ETH_IND_REQ                      201

#define IDLE_STATE                       0
#define TRIGGERED_STATE                  1

#define INTERFACE_RS232_UART3            (300)
#define INTERFACE_USB_UART1              (301)
#define INTERFACE_ETHERNET               (302)
#define INTERFACE_USB_UART2              (303)

#define MCS_CAN_ID                       0x22

/* Change this section according to the device -------------------------------*/
#define DCDC_CURRENT_TOLERANCE           (0.02)

#endif //SETTING_PROJECT_H
