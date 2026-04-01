#ifndef COMMANDS_HANDLER_DEF
#define COMMANDS_HANDELR_DEF

#include "global.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "string.h"
#ifndef TB_DEF
#include "lwip/api.h"
#include "params_func.h"
#endif
#include "cmsis_os.h"
#include "usart.h"
#include "user_mode.h"
#include "tools.h"

void CommandsHandler(device_struct *mcs,char* resp,char* debug_buffer,char* tcp_buffer,int i);

#endif
