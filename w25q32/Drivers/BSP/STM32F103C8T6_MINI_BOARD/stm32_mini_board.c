#include "stdint.h"
#include "diskio.h"
#include "FLASH_W25QXX.h"
#include "spi.h"
#include "gpio.h"
#include "stm32_mini_board.h"
#include "app_log.h"

#if APP_LOG_ENABLED > 0    
#undef  APP_LOG_MODULE_NAME 
#undef  APP_LOG_MODULE_LEVEL
#define APP_LOG_MODULE_NAME   "[board]"
#define APP_LOG_MODULE_LEVEL   APP_LOG_LEVEL_DEBUG    
#endif

void BSP_init()
{
  
APP_LOG_DEBUG("BSP init!\r\n");  
  
  
}

DSTATUS BSP_W25Q32_Init(uint8_t id)
{
  uint8_t ret=STA_NOINIT;
  if(id==0)
  {
    ret=0;
  }
  else
    ret=STA_NOINIT;
  return ret;
}

DSTATUS BSP_W25Q32_get_status(uint8_t id)
{
  
  
   return 0;
   
  
}
#define CTRL_SYNC		0	/* Complete pending write process (needed at _FS_READONLY == 0) */
#define GET_SECTOR_COUNT	1	/* Get media size (needed at _USE_MKFS == 1) */
#define GET_SECTOR_SIZE		2	/* Get sector size (needed at _MAX_SS != _MIN_SS) */
#define GET_BLOCK_SIZE		3	/* Get erase block size (needed at _USE_MKFS == 1) */


DRESULT BSP_W25Q32_Ioctl (
	BYTE pdrv,      /* Physical drive nmuber (0..) */
	BYTE cmd,       /* Control code */
	void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res = RES_ERROR;
    if(pdrv!=0)
      return res;
    
    switch(cmd)
    {
    case CTRL_SYNC:
      break;
    case GET_SECTOR_COUNT:
      *(uint32_t*)buff=SECTORS_PER_CHIP;
      break;
    case GET_SECTOR_SIZE:
       *(uint16_t*)buff=BYTES_PER_PAGE*PAGES_PER_SECTOR;
      break;
    case GET_BLOCK_SIZE:
       *(uint32_t*)buff=16*BYTES_PER_PAGE*PAGES_PER_SECTOR;
      break;
      
    }
    return 0;
}

DRESULT BSP_W25Q32_Sector_Read(BYTE id, /* Physical drive nmuber to identify the drive */
                        BYTE* ptr_buff,/* Data buffer to store read data */
                        DWORD sec_addr,/* Sector address in LBA */ 
                        UINT sec_cnt)/* Number of sectors to read */ 
{
DRESULT ret=RES_OK;
uint32_t byte_addr_start,byte_cnt;

byte_addr_start=sec_addr*PAGES_PER_SECTOR*BYTES_PER_PAGE;
byte_cnt=sec_cnt*PAGES_PER_SECTOR*BYTES_PER_PAGE;

if(ptr_buff==BSP_NULL || sec_addr+sec_cnt > SECTORS_PER_CHIP)
ret=RES_PARERR;


FLASH_W25QXX_Read_Data_Block(byte_addr_start,ptr_buff,byte_cnt);

return ret;
}

 DRESULT BSP_W25Q32_Sector_Program(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
 {
   DRESULT ret=RES_OK;
  if(pdrv!=0 || buff==BSP_NULL) 
    ret=RES_PARERR;
  
  FLASH_W25QXX_Sector_Program(buff,sector,count );
  
   return ret;
 }

uint8_t BSP_SPI_TRANS_RECV(uint8_t send)
{
  uint8_t tx[1],rx[1];
  tx[0]=send; 
  HAL_SPI_TransmitReceive(&hspi1, tx,rx,1,0xffff); 
  return rx[0];
  
}
void BSP_W25Q32_CS_LOW()
{
  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET); 
  
}
void BSP_W25Q32_CS_HIGH()
{
  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);  
  
}





