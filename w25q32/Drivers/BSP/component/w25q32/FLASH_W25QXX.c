/*************************************************************************
* WINBOND W25QXX FLASH CHIP LIBRARY
*
*	SUPPORT FOR ONLY NORMAL SPI MODE (NO BI/QUAD SPI)
* SPI DATA FORMAT : 8 BITS, MSB FIRST
*
* THIS LIBRARY DOES NOT CONTROL THE SPI CS LINE. THATS THE REPSONSIBILITY
* OF THE USER APPLICATION
*
* THE EXCEPTION IS WHEN READING STATUS REGISTER FOR THE BUSY BIT. SINCE
* THIS SHOULD BE DONE BEFORE EXECUTING ANY OPERATION ON THE FLASH AND THE FACT
* THAT W25QXX DEVICE CANNOT DO COMMAND CHAINING (AFTER SENDING STATUS READ
* REGISTER COMMAND, THE CS LINE NEEDS TO BE TOGGLED BEFORE SENDING THE
* ACTUAL COMMAND, THE LIBRARY DOES NEED ACCESS TO CS LINE CONTROL FUNCTIONS
* THIS IS THE ONLY INSTANCE WHERE THE LIBRARY CONTROLS THE CS LINE
*
* FLASH MEMORY GETS ERASED WITH ALL 1 BITS (DEFAULT STATE)
*
* MARCH 27 2017
* ANKIT BHATNAGAR
* ANKIT.BHATNAGARINDIA@GMAIL.COM
*
* ***********************************************************************/
#include "stdint.h"
#include "FLASH_W25QXX.h"

extern void BSP_W25Q32_CS_HIGH();
extern void BSP_W25Q32_CS_LOW();
extern uint8_t BSP_SPI_TRANS_RECV(uint8_t send);

//FUNCTION POINTERS FOR INTERNAL LIBRARY USE
//SET THESE TO YOUR APPLICATION FUNCTIONS
static uint8_t (*spi_send_get_function)(uint8_t) = BSP_SPI_TRANS_RECV;
static void (*spi_cs_low_function)(void) = BSP_W25Q32_CS_LOW;
static void (*spi_cs_high_function)(void) = BSP_W25Q32_CS_HIGH;

void FLASH_W25QXX_Enable_Reset(void)
{
	//RESET W25QXX DEVICE
	
	//WAIT FOR THE FLASH TO BE FREE
	while(FLASH_W25QXX_Get_Busy_Status() != 0){};
	
	//REASSERT CS LINE
	spi_cs_low_function();
		
	spi_send_get_function(FLASH_W25QXX_COMMAND_ENABLE_RESET);
        //DEASSERT CS LINE
	spi_cs_high_function();
}

void FLASH_W25QXX_Do_Reset(void)
{
	//RESET W25QXX DEVICE
	
	//WAIT FOR THE FLASH TO BE FREE
	while(FLASH_W25QXX_Get_Busy_Status() != 0){};
	
	//REASSERT CS LINE
	spi_cs_low_function();
		
	spi_send_get_function(FLASH_W25QXX_COMMAND_RESET);
        
        spi_cs_high_function(); 
}

uint8_t FLASH_W25QXX_Get_Status_Register(uint8_t register_num)
{
	//RETURN THE VALUE OF THE SPECIFIED STATUS REGISTER
	uint8_t recv=0xff;
        
       //DEASSERT CS LINE
       spi_cs_low_function();
       
	if(register_num == 1)
	{
	spi_send_get_function(FLASH_W25QXX_COMMAND_READ_STATUS_REG_1);
	recv= (uint8_t)spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	}
	else if(register_num == 2)
	{
	spi_send_get_function(FLASH_W25QXX_COMMAND_READ_STATUS_REG_2);
	recv= (uint8_t)spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	}
        
	spi_cs_high_function();
	return recv;
}

uint8_t FLASH_W25QXX_Get_Busy_Status(void)
{
	//RETURN THE BUSY STATUS OF THE DEVICE
	
	uint8_t val = FLASH_W25QXX_Get_Status_Register(1);
	return (val & 0x01);
}

void FLASH_W25QXX_Write_Enable(void)
{
	//SET THE WRITE ENABLE FLAG IN DEVICE
	
	//WAIT FOR THE FLASH TO BE FREE
	while(FLASH_W25QXX_Get_Busy_Status() != 0){};
	
	//REASSERT CS LINE
	spi_cs_low_function();		
	spi_send_get_function(FLASH_W25QXX_COMMAND_WRITE_ENABLE);
        spi_cs_high_function(); 
        
        while(FLASH_W25QXX_Get_Busy_Status() != 0){};
}

void FLASH_W25QXX_Write_Disable(void)
{
	//SET THE WRITE DISABLE FLAG IN DEVICE
	
	//WAIT FOR THE FLASH TO BE FREE
	while(FLASH_W25QXX_Get_Busy_Status() != 0){};
	
	//REASSERT CS LINE
	spi_cs_low_function();
		
	spi_send_get_function(FLASH_W25QXX_COMMAND_WRITE_DISABLE);
        spi_cs_high_function(); 
        
        while(FLASH_W25QXX_Get_Busy_Status() != 0){};
}

uint16_t FLASH_W25QXX_Get_Manufacturer_Id(void)
{
	//RETURN MANUFACTURER ID(1 BYTE) & DEVICE ID(1 BYTE)
	
	//WAIT FOR THE FLASH TO BE FREE
	while(FLASH_W25QXX_Get_Busy_Status() != 0){};
	
	//REASSERT CS LINE
	spi_cs_low_function();
		
	uint16_t temp;
	
	spi_send_get_function(FLASH_W25QXX_COMMAND_MANUFACTURER_ID);
	spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	temp = spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	temp = temp << 8;
	temp |= spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	
        spi_cs_high_function(); 
	return temp;
}


void FLASH_W25QXX_Get_Unique_Id(uint8_t* ret_ptr)
{
	//RETURN 64 BIT DEVICE UNIQUE ID
	
	//WAIT FOR THE FLASH TO BE FREE
	while(FLASH_W25QXX_Get_Busy_Status() != 0){};
	
	//REASSERT CS LINE
	spi_cs_low_function();
	
	spi_send_get_function(FLASH_W25QXX_COMMAND_UNIQUE_ID);
	spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	
	ret_ptr[0] = (uint8_t)spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	ret_ptr[1] = (uint8_t)spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	ret_ptr[2] = (uint8_t)spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	ret_ptr[3] = (uint8_t)spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	ret_ptr[4] = (uint8_t)spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	ret_ptr[5] = (uint8_t)spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	ret_ptr[6] = (uint8_t)spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	ret_ptr[7] = (uint8_t)spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
        
        spi_cs_high_function(); 
}


void FLASH_W25QXX_Get_Jedec_Id(uint8_t* ret_ptr)
{
	//RETURN DEVICE JEDEC ID
	
	//WAIT FOR THE FLASH TO BE FREE
	while(FLASH_W25QXX_Get_Busy_Status() != 0){};
	
	//REASSERT CS LINE
	spi_cs_low_function();
	
	spi_send_get_function(FLASH_W25QXX_COMMAND_JEDEC_ID);
	ret_ptr[0] = spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	ret_ptr[1] = spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	ret_ptr[2] = spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
       	
        spi_cs_high_function(); 
}


uint8_t FLASH_W25QXX_Read_Data_Byte(uint32_t add)
{
	//READ AND RETURN BYTE OF DATA READ FROM THE SPECIFIED ADDRESS
	//AUTO INCREAMENTS THE READ ADDRESS FOR THE NEXT READ OPERATION
	uint8_t recv;
	//WAIT FOR THE FLASH TO BE FREE
	while(FLASH_W25QXX_Get_Busy_Status() != 0){};
		
	//REASSERT CS LINE
	spi_cs_low_function();
		
	uint8_t add_hi = (uint8_t)((add & 0x00FF0000) >> 16);
	uint8_t add_mid = (uint8_t)((add & 0x0000FF00) >> 8);
	uint8_t add_lo = (uint8_t)(add);
	spi_send_get_function(FLASH_W25QXX_COMMAND_READ_DATA);
	spi_send_get_function(add_hi);
	spi_send_get_function(add_mid);
	spi_send_get_function(add_lo);
	 recv =(uint8_t)spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
        spi_cs_high_function(); 
         return recv;
        
}


void FLASH_W25QXX_Read_Data_Block(uint32_t add, uint8_t* data_ptr, uint32_t read_len)
{
	//READ A BLOCK OF DATA FROM THE SPECIFIED ADDRESS AND STORE IN THE BLOCK POINTER
	
	//WAIT FOR THE FLASH TO BE FREE
	while(FLASH_W25QXX_Get_Busy_Status() != 0){};
		
	//REASSERT CS LINE
	spi_cs_low_function();
	
	uint8_t add_hi = (uint8_t)((add & 0x00FF0000) >> 16);
	uint8_t add_mid = (uint8_t)((add & 0x0000FF00) >> 8);
	uint8_t add_lo = (uint8_t)(add);
	spi_send_get_function(FLASH_W25QXX_COMMAND_READ_DATA);
	spi_send_get_function(add_hi);
	spi_send_get_function(add_mid);
	spi_send_get_function(add_lo);
	
	for(uint32_t i=0; i<read_len; i++)
	{
	 data_ptr[i] = (uint8_t)spi_send_get_function(FLASH_W25QXX_COMMAND_NOP);
	}
        
        spi_cs_high_function();
}

void FLASH_W25QXX_Write_Status_Registers(uint8_t val_stat_reg_1, uint8_t val_stat_reg_2)
{
	//WRITE THE SPECIFIED VALUES TO STATUS REGISTERS 1 AND 2
	
	//WAIT FOR THE FLASH TO BE FREE
	while(FLASH_W25QXX_Get_Busy_Status() != 0){};
        
        FLASH_W25QXX_Write_Enable();	
	//REASSERT CS LINE
	spi_cs_low_function();
	
	spi_send_get_function(FLASH_W25QXX_COMMAND_WRITE_STATUS_REGS);
	spi_send_get_function(val_stat_reg_1);
	spi_send_get_function(val_stat_reg_2);
        
        spi_cs_high_function();
}


void FLASH_W25QXX_Sector_Erase_4kb(uint32_t add_sector)
{
	//ERASE THE SPECIFIED SECTOR (4KB)
	
	//WAIT FOR THE FLASH TO BE FREE
	while(FLASH_W25QXX_Get_Busy_Status() != 0){};
	
        FLASH_W25QXX_Write_Enable();	
	//REASSERT CS LINE
	spi_cs_low_function();
	
	uint8_t add_hi = (uint8_t)((add_sector & 0x00FF0000) >> 16);
	uint8_t add_mid = (uint8_t)((add_sector & 0x0000FF00) >> 8);
	uint8_t add_lo = (uint8_t)(add_sector);
	
	spi_send_get_function(FLASH_W25QXX_COMMAND_SECTOR_ERASE_4KB);
	spi_send_get_function(add_hi);
	spi_send_get_function(add_mid);
	spi_send_get_function(add_lo);
        
        spi_cs_high_function();
}

void FLASH_W25QXX_Block_Erase_32Kb(uint32_t add_block)
{
	//ERASE THE SPECFIFIED BLOCK (32KB)
	
	//WAIT FOR THE FLASH TO BE FREE
	while(FLASH_W25QXX_Get_Busy_Status() != 0){};
	
        FLASH_W25QXX_Write_Enable();	
	//REASSERT CS LINE
	spi_cs_low_function();
	
	uint8_t add_hi = (uint8_t)((add_block & 0x00FF0000) >> 16);
	uint8_t add_mid = (uint8_t)((add_block & 0x0000FF00) >> 8);
	uint8_t add_lo = (uint8_t)(add_block);
	
	spi_send_get_function(FLASH_W25QXX_COMMAND_BLOCK_ERASE_32KB);
	spi_send_get_function(add_hi);
	spi_send_get_function(add_mid);
	spi_send_get_function(add_lo);
        
        spi_cs_high_function();
}

void FLASH_W25QXX_Block_Erase_64Kb(uint32_t add_block)
{
	//ERASE THE SPECIFIED BLOCK (64KB)
	
	//WAIT FOR THE FLASH TO BE FREE
	while(FLASH_W25QXX_Get_Busy_Status() != 0){};
        
         FLASH_W25QXX_Write_Enable();	
	//REASSERT CS LINE
	spi_cs_low_function();
	
	uint8_t add_hi = (uint8_t)((add_block & 0x00FF0000) >> 16);
	uint8_t add_mid = (uint8_t)((add_block & 0x0000FF00) >> 8);
	uint8_t add_lo = (uint8_t)(add_block);
	
	spi_send_get_function(FLASH_W25QXX_COMMAND_BLOCK_ERASE_64KB);
	spi_send_get_function(add_hi);
	spi_send_get_function(add_mid);
	spi_send_get_function(add_lo);
        
        spi_cs_high_function();
}

void FLASH_W25QXX_Chip_Erase(void)
{
	//ERASE THE COMPLETE CHIP
	
	//WAIT FOR THE FLASH TO BE FREE
	while(FLASH_W25QXX_Get_Busy_Status() != 0){};
        FLASH_W25QXX_Write_Enable();	
	//REASSERT CS LINE
	spi_cs_low_function();
	
	spi_send_get_function(FLASH_W25QXX_COMMAND_CHIP_ERASE);
        
        spi_cs_high_function();
}

void FLASH_W25QXX_Page_Program(uint32_t page_addr,const uint8_t *ptr_buff )
{
     uint32_t byte_addr;
     byte_addr=page_addr*BYTES_PER_PAGE;
  
     FLASH_W25QXX_Write_Enable();

     uint8_t add_hi = (uint8_t)((byte_addr & 0x00FF0000) >> 16);
     uint8_t add_mid = (uint8_t)((byte_addr & 0x0000FF00) >> 8);
     uint8_t add_lo = (uint8_t)(byte_addr);
     
    spi_cs_low_function();
    spi_send_get_function(FLASH_W25QXX_COMMAND_PAGE_WRITE);
    spi_send_get_function(add_hi);
    spi_send_get_function(add_mid);
    spi_send_get_function(add_lo);
	
    for(uint32_t i=0; i<BYTES_PER_PAGE; i++)
   {
    spi_send_get_function(ptr_buff[i]);
   }
   spi_cs_high_function();
}


void FLASH_W25QXX_Sector_Program(
	const uint8_t *buff,   /* Data to be written */
	uint32_t sector,       /* Sector address in LBA */
	uint32_t count )
{
  uint32_t page_cnt;
  uint32_t page_addr; 

  for(int i=0;i<count;i++)
  FLASH_W25QXX_Sector_Erase_4kb((sector+i)*BYTES_PER_PAGE*PAGES_PER_SECTOR);
  
  page_addr=sector*PAGES_PER_SECTOR;

  page_cnt=count*PAGES_PER_SECTOR;
  
   for(uint32_t cnt=0;cnt<page_cnt;cnt++)
   {
    //WAIT FOR THE FLASH TO BE FREE
    while(FLASH_W25QXX_Get_Busy_Status() != 0);
    
    FLASH_W25QXX_Page_Program(page_addr,buff+BYTES_PER_PAGE*cnt );
    page_addr+=1; 
   } 
}
