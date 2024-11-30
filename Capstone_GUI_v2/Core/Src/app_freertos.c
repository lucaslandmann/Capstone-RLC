/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os2.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define sampleSize 512
//System will capture specificed number of samples per channel
#define denoiseSize 1
#define maxGain 10.0f
#define channelCount 8
#define devAddress 0x90 //Device address of PCM6260, pre-shift
//TODO: Determine the actual array position of these values
#define c1Vol 0
#define c2Vol 1
#define c3Vol 2
#define c4Vol 3
#define c5Vol 4
#define c6Vol 5
#define masterVol 7
#define c1LR 8
#define c2LR 9
#define c3LR 10
#define c4LR 11
#define c5LR 0
#define c6LR 1

#define channelSettings 0xA0
extern struct channelStruct channels[channelCount] = {0};

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for GUI_Task */
osThreadId_t GUI_TaskHandle;
const osThreadAttr_t GUI_Task_attributes = {
  .name = "GUI_Task",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 8192 * 4
};
/* Definitions for Audio_Task */
osThreadId_t Audio_TaskHandle;
const osThreadAttr_t Audio_Task_attributes = {
  .name = "Audio_Task",
  .priority = (osPriority_t) osPriorityHigh7,
  .stack_size = 8192 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
extern portBASE_TYPE IdleTaskHook(void* p);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void TouchGFX_Task(void *argument);
void Audio_Function(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);

/* USER CODE BEGIN 2 */
void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
  
   vTaskSetApplicationTaskTag(NULL, IdleTaskHook);
}
/* USER CODE END 2 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

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
  /* USER CODE END RTOS_QUEUES */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of GUI_Task */
  GUI_TaskHandle = osThreadNew(TouchGFX_Task, NULL, &GUI_Task_attributes);

  /* creation of Audio_Task */
  Audio_TaskHandle = osThreadNew(Audio_Function, NULL, &Audio_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}
/* USER CODE BEGIN Header_StartDefaultTask */
/**
* @brief Function implementing the defaultTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN defaultTask */
  for(;;)
  {
  }
  /* USER CODE END defaultTask */
}

/* USER CODE BEGIN Header_Audio_Function */
/**
* @brief Function implementing the Audio_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Audio_Function */
void Audio_Function(void *argument)
{
  /* USER CODE BEGIN Audio_Task */
  /* Infinite loop */
  for(;;)
  {
	  if(adcReady)
	  {
		  //Loads sample data into Structs
		  for (uint16_t channel = 0; channel < channelCount; channel++)
		  {
		        for (uint16_t sample = 0; sample < (sampleSize / 2); sample++)
		        {
		            channels[channel].channelData[sample] = signExtend24((uint32_t)(adcData[channelCount*sample + channel]));

		            if(channel == 1){
		            channels[channel].channelData[sample] = (int32_t)((1.0f-wet)*((float)channels[1].channelData[sample])
							    + wet*Do_Delay((float)channels[1].channelData[sample], 1));
		            }
		        }
		  }
		  //TODO: apply effects
		  //TODO: mix
		  adcReady = false;
	  }
	  if(dacReady)
	  {
		  for(uint16_t sample = 0; sample < sampleSize / 2; sample++)
		  {
			  int32_t mixedSignalLeft = 0;
			  int32_t mixedSignalRight = 0;
			  for(uint16_t currChannel = 0; currChannel < 6; currChannel ++)
			  {
				  float digGain = (float)(channels[currChannel].volumeRunner >> 6) / 512.0f;
				  digGain = digGain * maxGain;

				  mixedSignalLeft += (int32_t)((float)channels[currChannel].channelData[sample] * digGain * channels[currChannel].lFloat);
				  mixedSignalRight += (int32_t)((float)channels[currChannel].channelData[sample] * digGain * channels[currChannel].rFloat);
			  }
			  mixedSignalLeft = mixedSignalLeft / 6;
			  mixedSignalRight = mixedSignalRight / 6;
			  dacData[(sample * 2)] =  mixedSignalLeft;//channels[2].channelData[sample];
			  dacData[(sample * 2) + 1] = mixedSignalRight;//channels[2].channelData[sample];
		  }
		  dacReady = false;
	  }
  }
  /* USER CODE END Audio_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

