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
#include "fatfs.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;

/* USER CODE BEGIN Variables */
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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 1700);
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

    uint16_t m_id;
    uint8_t id[8];
    m_id=FLASH_W25QXX_Get_Manufacturer_Id();
    APP_LOG_DEBUG("m_id is:%d\r\n",m_id);
    
    FLASH_W25QXX_Get_Unique_Id(id);
    for(uint8_t i=0;i<8;i++)
    {
     APP_LOG_DEBUG("u_id is:%d\r\n",id[i]);
    }
    /*
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
*/
  int SEGGER_RTT_GetKey(void);
  for(;;)
  {
    FRESULT ret;
    UINT bw;
    char key;
    osDelay(100);
    uint8_t read[30]={0};
    
   key=(char) SEGGER_RTT_GetKey();
   
   switch(key)
   {
   case 'm':
  if(f_mount(&USERFatFS, (TCHAR const*)USERPath, 1) != FR_OK)
  {
   APP_LOG_DEBUG("mount fs failed!\r\n"); 
  }
  else
  {
   APP_LOG_DEBUG("mount fs success!\r\n");
  }
  break;
 /*
  case 'f':
  if(f_mkfs((TCHAR const*)USERPath, 1, 1) != FR_OK)
  {
    APP_LOG_DEBUG("mk fs failed!\r\n");
  }
  else
  {
   APP_LOG_DEBUG("mk fs success!\r\n");
  }
  break;
  case 'o':
  ret=f_open (&USERFile, "0:/hello.txt",FA_WRITE|FA_READ|FA_OPEN_ALWAYS);   
   if(ret==FR_OK)
   {
    APP_LOG_DEBUG("open file success!\r\n");
   }
   else
   {
    APP_LOG_DEBUG("open file failed!\r\n");
   }
    break;
    
   case 'w': 
   ret= f_write (&USERFile, "wkxboot fat_file_system!",sizeof("wkxboot fat_file_system!"), &bw);	
   if(ret==FR_OK)
   {
   APP_LOG_DEBUG("file write success!\r\n");  
   }
   else
   {
   APP_LOG_DEBUG("file write failed!\r\n");
   }  
   break;
   
   case 'c':
    ret= f_close(&USERFile);
    if(ret==FR_OK)
    {
     APP_LOG_DEBUG("file close success!\r\n");
    }
    else
    {
     APP_LOG_DEBUG("file close  failed!\r\n");
    }
    break;
 
   case 'r': 
   for(int i=0;i<30;i++)
   {
   read[i]=0;
   }
   ret= f_read (&USERFile, read, 30 ,&bw);
   if(ret==FR_OK)
   {
   APP_LOG_DEBUG("file read  success!\r\n");  
   read[29]=0;
   APP_LOG_DEBUG("data:\r\n%s",read); 
   }
   else
   {
   APP_LOG_DEBUG("file read  failed!\r\n"); 
   }
  break;
  
  case 'l':
  ret = f_lseek(&USERFile, 3);
  
   if(ret==FR_OK)
   {
   APP_LOG_DEBUG("file lseek  success!\r\n");  
   }
   else
   {
  APP_LOG_DEBUG("file lseek  failed!\r\n");  
   }
  break;
*/
  case 'u':
  FILINFO info;
  ret = f_stat("0:/hello.txt", &info);
  
   if(ret==FR_OK)
   {
   APP_LOG_DEBUG("file status  success!\r\n");  
   APP_LOG_INFO("file name:%s\r\n",info.fname);
   APP_LOG_INFO("file time:%d\r\n",info.fdate);
   }
   else
   {
  APP_LOG_DEBUG("file status  failed!\r\n");  
   }
  break;
 FRESULT scan_files (
    char* path       
 );
 
   case 'd':
   uint8_t buff[50];
   buff[0]='/';
    scan_files (
     buff        
 ); 
 break;
 
  /*
   case 'i':
 FRESULT open_dir();
  ret =open_dir();
  if (ret == FR_OK) {
    APP_LOG_DEBUG("打开文件夹成功！\r\n");
  }
  else
  {
   APP_LOG_DEBUG("打开文件夹失败！\r\n");
  }
  break;
*/
  /*
   case 'x':
     ret=f_mkdir("0:/xxoo");
   if (ret == FR_OK)
   {
     APP_LOG_DEBUG("创建文件夹成功！\r\n");
   }
   else
   {
       APP_LOG_DEBUG("创建文件夹失败！\r\n");
   }
*/
  case 's':
  APP_LOG_DEBUG("file size:%d\r\n",f_size(&USERFile));  
  break;  
  case 'p':
  APP_LOG_DEBUG("file pointer:%d\r\n",f_tell(&USERFile));  
  break;  
   
   }
   
   
  }

  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Application */

FRESULT open_dir()
{
     DIR dir; 
    return f_opendir(&dir,"0:/xxoo");                       /* Open the directory */
}

FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                res = scan_files(path);                    /* Enter the directory */
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                APP_LOG_INFO("%s/%s\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }

    return res;
}   
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
