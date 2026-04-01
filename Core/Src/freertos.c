/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "../Inc/main.h"
#ifndef TB_DEF
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#endif
#include "../Inc/tools.h"
#include "../Inc/global.h"
#include "../Inc/can.h"
#include "../Inc/server.h"
#include "../Inc/commands_handler.h"
#include "../Inc/hardware.h"

#include "../Inc/drivers/amp.h"
#include "../Inc/drivers/preamp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
device_struct mcs_storage;

uint8_t uart1_rx_byte;
const uint8_t seed_gen_id[SEED_GEN_COUNT] = {9};
const uint8_t preamp_id[PREAMP_COUNT] = {10,11,12,13};
const uint8_t amp_id[AMP_COUNT] = {20,21,22,23};

/* Definitions for dev_refresh_task */
int dev_refresh_task_state = 0;
osThreadId  dev_refresh_taskHandle;

QueueHandle_t  ind_queueHandle;
QueueHandle_t int_can_mess_queue;
QueueHandle_t send_can_mess_queue;
QueueHandle_t link_queueHandle;
QueueHandle_t tcp_rx_data_queue;
QueueHandle_t  exti_alarm_queueHandle;
QueueHandle_t  debugger_queueHandle;

/* USER CODE END Variables */
osThreadId main_taskHandle;
osThreadId indi_taskHandle;
osThreadId server_taskHandle;
osThreadId can_sendHandle;
osThreadId toolsTaskHandle;
osMutexId CanMutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void dev_refresh_task_h(const void *argument);
/* USER CODE END FunctionPrototypes */

void h_main_task(void const * argument);
void h_indi_task(void const * argument);
void h_server_task(void const * argument);
void h_can_send(void const * argument);
void h_tools(void const * argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
#ifndef TB_DEF
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
#endif
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	read_last_config_data(&mcs_storage.config);

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* definition and creation of CanMutex */
  osMutexDef(CanMutex);
  CanMutexHandle = osMutexCreate(osMutex(CanMutex));

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	ind_queueHandle = xQueueCreate(10, sizeof(uint8_t));
	int_can_mess_queue = xQueueCreate(5, sizeof(can_message_struct));
	send_can_mess_queue = xQueueCreate(2, sizeof(can_message_struct_tx));
	link_queueHandle = xQueueCreate(5, sizeof(uint8_t));
	tcp_rx_data_queue  = xQueueCreate(TCP_QUEUE_BUF_SIZE, sizeof(char));
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of main_task */
  osThreadDef(main_task, h_main_task, osPriorityNormal, 0, 2048);
  main_taskHandle = osThreadCreate(osThread(main_task), NULL);

  /* definition and creation of indi_task */
  osThreadDef(indi_task, h_indi_task, osPriorityNormal, 0, 256);
  indi_taskHandle = osThreadCreate(osThread(indi_task), NULL);

  /* definition and creation of server_task */
  osThreadDef(server_task, h_server_task, osPriorityNormal, 0, 2048);
  server_taskHandle = osThreadCreate(osThread(server_task), NULL);

  /* definition and creation of can_send */
  osThreadDef(can_send, h_can_send, osPriorityNormal, 0, 1024);
  can_sendHandle = osThreadCreate(osThread(can_send), NULL);

  /* definition and creation of toolsTask */
  osThreadDef(toolsTask, h_tools, osPriorityIdle, 0, 512);
  toolsTaskHandle = osThreadCreate(osThread(toolsTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  osThreadSuspend(server_taskHandle);
  osThreadSuspend(indi_taskHandle);
  osThreadSuspend(can_sendHandle);
  osThreadSuspend(toolsTaskHandle);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_h_main_task */
/**
  * @brief  Function implementing the main_task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_h_main_task */
void h_main_task(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN h_main_task */
	device_struct *mcs = &mcs_storage;
	char debug_buffer [1000];

	char tcp_buffer [1000];
	char resp [1000];
	long tcp_buffer_ind = 0 ;

	for(int try = 0; try < 3;try++){
		alarm_and_state_handler(mcs);
#ifndef TB_DEF
		osDelay(100);
#endif
	}

	leds_init (&mcs->leds);

	can_configure_filters(&hcan1, EXT_DEVICE_ADDR) ;
	can_start_module(&hcan1);
	osDelay(3000);
	#if SEED_GEN_COUNT > 0
		for(int i = 0; i < SEED_GEN_COUNT;i++)
			gen_init(&mcs->gen, &hcan1, seed_gen_id[i], &int_can_mess_queue,CanMutexHandle);
	#endif
	#if PREAMP_COUNT > 0
		for(int i = 0; i < PREAMP_COUNT;i++)
			preamp_init(&mcs->preamp[i], &hcan1,preamp_id[i], &int_can_mess_queue, CanMutexHandle);
	#endif
	#if AMP_COUNT > 0
		for(int i = 0; i < AMP_COUNT;i++)
			amp_init(&mcs->amp[i], &hcan1,amp_id[i], &int_can_mess_queue,CanMutexHandle);
	#endif
	#if DIVIDE_COUNT > 0
		for(int i = 0; i < DIVIDE_COUNT;i++){
		  divide_init(&mcs->div[i], &hcan1, 8, &int_can_mess_queue, CanMutexHandle);
		}
	#endif

	HAL_GPIO_WritePin(Protection_ON_OFF_GPIO_Port, Protection_ON_OFF_Pin, GPIO_PIN_RESET);

	if (!is_alarm_phase())
		power_on(mcs);

	osThreadResume(server_taskHandle);
	osThreadResume(indi_taskHandle);
	osThreadResume(can_sendHandle);
	osThreadResume(toolsTaskHandle);

	 osThreadDef ( dev_refresh , dev_refresh_task_h, osPriorityNormal, 1, 256);
	 dev_refresh_taskHandle = osThreadCreate(osThread(dev_refresh), NULL);
	/* Infinite loop */
	for(;;)
	{
		HAL_UART_Receive_IT ( &huart1 , &uart1_rx_byte , 1 ) ;
		if (xQueueReceive(tcp_rx_data_queue, &tcp_buffer[tcp_buffer_ind], 0) == pdPASS) {
			if (tcp_buffer[tcp_buffer_ind] == '\r' || tcp_buffer[tcp_buffer_ind] == '\n')
			{
				tcp_buffer[tcp_buffer_ind + 1] = '\0';
				CommandsHandler(mcs, resp, debug_buffer, tcp_buffer, 0);
				//	memset(tcp_buffer,0,tcp_buffer_ind);
				tcp_buffer[tcp_buffer_ind] = '\0';
				tcp_buffer_ind = 0;
			}
			else if (tcp_buffer_ind < TCP_QUEUE_BUF_SIZE - 2)
			{
				//backspase ...
				if (tcp_buffer[tcp_buffer_ind] == '\b' && tcp_buffer_ind > 0)
					tcp_buffer_ind--;
				else
					tcp_buffer_ind++;
			}
			else
			{
				tcp_buffer_ind = 0;
			}
		}
		else {
			osDelay(1);
		}
		// clear response buffer to prepare for next cycle begin
		mcs->current_interface = get_interface_in();
		// clear response buffer to prepare for next cycle end
	}
  /* USER CODE END h_main_task */
}

/* USER CODE BEGIN Header_h_indi_task */
/**
* @brief Function implementing the indi_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_h_indi_task */
void h_indi_task(void const * argument)
{
  /* USER CODE BEGIN h_indi_task */
	osDelay(100);
	/* Infinite loop */
	for(;;)
	{
		indication_handler(&ind_queueHandle, &mcs_storage);
		osDelay(1);
	}
  /* USER CODE END h_indi_task */
}

/* USER CODE BEGIN Header_h_server_task */
/**
* @brief Function implementing the server_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_h_server_task */
void h_server_task(void const * argument)
{
  /* USER CODE BEGIN h_server_task */
	osDelay(100);
	/* Infinite loop */
	for(;;)
	{
		server_handler(&tcp_rx_data_queue,&ind_queueHandle);
	}
  /* USER CODE END h_server_task */
}

/* USER CODE BEGIN Header_h_can_send */
/**
* @brief Function implementing the can_send thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_h_can_send */
void h_can_send(void const * argument)
{
  /* USER CODE BEGIN h_can_send */
	can_message_struct_tx cm;
	int repeat = 5;
	int err =0;
  /* Infinite loop */
  for(;;)
  {
	if(xQueueReceive(send_can_mess_queue,&cm,0) == pdPASS){
		err = 0;
		xSemaphoreTake(CanMutexHandle,portMAX_DELAY);
		for (int8_t reps = repeat; reps > 0; reps --){		
			can_send_message(cm.h_can, &cm.tx_header, cm.data);
			err = cm.ack_func(cm.device, cm.cmd_id, 100);
			if(err == 0)
				break;
		}
		xSemaphoreGive(CanMutexHandle);
	}
	else
		osDelay(1);
  }
  /* USER CODE END h_can_send */
}

/* USER CODE BEGIN Header_h_tools */
/**
* @brief Function implementing the toolsTaskHandle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_h_tools */
void h_tools(void const * argument)
{
  /* USER CODE BEGIN h_tools */
	device_struct* mcs = &mcs_storage;
  /* Infinite loop */
	for(;;)
	{
		alarm_and_state_handler (mcs);

		mcs->user_mode.output_started = get_emission(mcs);

		if (mcs->alarms.val != 0) {
			user_mode_prepare ();
			create_usr_offtaskHandle ();
		}

		preamp_all_power_update (mcs);
		amp_all_power_update (mcs);
	}
  /* USER CODE END h_tools */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void dev_refresh_task_h(const void *argument)
{
	//	device_struct *mcs = &mcs_storage ;
	dev_refresh_task_state = 1;
	device_struct* mcs = &mcs_storage;
	/* Infinite loop */
	for(;;)
	{
#if SEED_GEN_COUNT > 0
		refresh_gen_state(&mcs->gen);
#endif

#if PREAMP_COUNT > 0
		for(int i = 0; i < PREAMP_COUNT; i++){
			preamp_refresh_state(&mcs->preamp[i]);
		}
#endif

#if AMP_COUNT > 0
		for(int i = 0; i < AMP_COUNT;i++){
			amp_refresh_state(&mcs->amp[i]);
		}
#endif

#if DIVIDE_COUNT > 0
		for(int i = 0; i < DIVIDE_COUNT;i++){
			refresh_divide_state(&mcs->div[i]);
		}
#endif
		osDelay(1);
	}
	dev_refresh_task_state = 0;
}

void HAL_UART_RxCpltCallback ( UART_HandleTypeDef *huart )
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE ;
	if ( huart == &huart1 ){
		if ( xQueueSendFromISR ( tcp_rx_data_queue , ( &uart1_rx_byte ) ,
				&xHigherPriorityTaskWoken ) == pdPASS ){
// OK processing
			set_interface_in(INTERFACE_USB_UART1) ;
		} else{
// ERROR processing
		}
		HAL_UART_Receive_IT ( &huart1 , &uart1_rx_byte , 1 ) ;
	}
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{
	can_message_struct can_mess;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	int tmp = CAN_IND_REQ;

	if(CanHandle == &hcan1){
		if(HAL_CAN_GetRxMessage(CanHandle,CAN_RX_FIFO0,&can_mess.rx_header,can_mess.data) == HAL_OK){
			if(xQueueSendFromISR(int_can_mess_queue,&can_mess,&xHigherPriorityTaskWoken) != pdPASS){
				//Обработка ошибки
			}
			if(xQueueSendFromISR(ind_queueHandle,&tmp,&xHigherPriorityTaskWoken) != pdPASS){
				//Обработка ошибки
			}
		}
	}
}
/* USER CODE END Application */
