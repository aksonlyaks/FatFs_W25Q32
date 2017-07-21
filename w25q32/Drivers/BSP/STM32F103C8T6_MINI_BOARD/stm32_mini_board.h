#ifndef __STM32_MINI_BOARD_H__
#define __STM32_MINI_BOARD_H__




#define  BSP_NULL    (void*)0











DSTATUS BSP_W25Q32_Init(uint8_t id);
DRESULT BSP_W25Q32_Sector_Program(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
DRESULT BSP_W25Q32_Sector_Read(BYTE id, /* Physical drive nmuber to identify the drive */
                        BYTE* ptr_buff,/* Data buffer to store read data */
                        DWORD sec_addr,/* Sector address in LBA */ 
                        UINT sec_cnt);/* Number of sectors to read */ 
DRESULT BSP_W25Q32_Ioctl (
	BYTE pdrv,      /* Physical drive nmuber (0..) */
	BYTE cmd,       /* Control code */
	void *buff      /* Buffer to send/receive control data */
);





#endif