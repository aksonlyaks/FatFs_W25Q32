/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "fatfs.h"
#include "stm32_mini_board.h"
#include "FLASH_W25QXX.h"
#include "app_log.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;

/* USER CODE BEGIN Variables */
FIL file_handle;
FATFS SDFatFs;  /* File system object for SD card logical drive */

//uint8_t read[4096];
/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

extern void MX_FATFS_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
#if APP_LOG_ENABLED > 0    
#undef  APP_LOG_MODULE_NAME 
#undef  APP_LOG_MODULE_LEVEL
#define APP_LOG_MODULE_NAME   "[board]"
#define APP_LOG_MODULE_LEVEL   APP_LOG_LEVEL_DEBUG    
#endif
/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

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

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0,1800 );
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{
  /* init code for FATFS */
  MX_FATFS_Init();

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  
if(f_mount(&SDFatFs, (TCHAR const*)USER_Path, 0) != FR_OK)
{
  APP_LOG_DEBUG("mount failed!\r\n");
}
else
{
   APP_LOG_DEBUG("mount success!\r\n");
}

 if(f_mkfs((TCHAR const*)USER_Path, 0, 1) != FR_OK)
  {
    APP_LOG_DEBUG("mkfs failed!\r\n");
  }
else
{
   APP_LOG_DEBUG("mkfs success!\r\n");
}

/*
  while(1)
  {
    uint16_t m_id;
    uint8_t id[8];
    m_id=FLASH_W25QXX_Get_Manufacturer_Id();
    APP_LOG_DEBUG("m_id is:%d\r\n",m_id);
    
    FLASH_W25QXX_Get_Unique_Id(id);
    for(uint8_t i=0;i<8;i++)
    {
     APP_LOG_DEBUG("m_id is:%d\r\n",id[i]);
    }
    
     BSP_W25Q32_Sector_Read( 0, 
                          read,
                           0,
                            1);
    for(uint32_t i=0;i<4096;i++)
    {
      read[i]=i;
    }
    BSP_W25Q32_Sector_Program( 0, read,  0,  1) ; 
    
     BSP_W25Q32_Sector_Read( 0, 
                          read,
                           0,
                            1); 
    for(uint32_t i=0;i<4096;i++)
    {
      APP_LOG_DEBUG(" %d:\r\n",read[i]);
    }
  }
*/
  
  for(;;)
  {
    FRESULT ret;
    UINT bw;
    uint8_t read[30]={0};
    ret=f_open (&file_handle, "hello.txt",FA_WRITE|FA_OPEN_ALWAYS);
    
    if(ret==FR_OK)
    {
      APP_LOG_DEBUG("open file success!\r\n");
      ret= f_write (&file_handle, "wkxboot fat_file_ststem!",30, &bw);	
      if(ret==FR_OK)
      {
        APP_LOG_DEBUG("file write success!\r\n");
      }
      else
      {
        APP_LOG_DEBUG("file write failed!");
      }
    }
    else
    {
     APP_LOG_DEBUG("file open failed!");
    }    
    ret= f_close(&file_handle);
    if(ret==FR_OK)
    {
     APP_LOG_DEBUG("file close success!");
     ret=f_open (&file_handle, "hello.txt",FA_READ);
      if(ret==FR_OK)
    {
      APP_LOG_DEBUG("file open  success!"); 
     ret= f_read (&file_handle, read, 30 ,&bw);
     if(ret==FR_OK)
     {
       APP_LOG_DEBUG("file read  success!");  
        for(int i=0;i<30;i++)
        {
          APP_LOG_DEBUG("data:\r\n%c",read[i]); 
        }
          ret= f_close(&file_handle);
         if(ret==FR_OK)
         {
           APP_LOG_DEBUG("file close  success!");     
         }
         else
         {
            APP_LOG_DEBUG("file close  failed!");  
         }
       
     }
     else
     {
        APP_LOG_DEBUG("file read  failed!"); 
     }
    }
    else
    {
    APP_LOG_DEBUG("file open  failed!");   
    }
    }
    else
    {
     APP_LOG_DEBUG("file close failed!"); 
    }
   
    osDelay(5000);
  }

  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
