/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "mmc_sd.h"
#include "malloc.h"

/* Definitions of physical drive number for each drive */
#define SD_CARD		0	/* Example: Map Ramdisk to physical drive 0 */
#define EX_FLASH		1	/* Example: Map MMC/SD card to physical drive 1 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

/* Get Drive Status */
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	u8 res=0;	    
	switch(pdrv)
	{
		case SD_CARD://SD卡
			res = SD_Initialize();//SD_Initialize() 
		 	if(res)//STM32 SPI的bug,在sd卡操作失败的时候如果不执行下面的语句,可能导致SPI读写异常
			{
				SD_SPI_SpeedLow();
				SD_SPI_ReadWriteByte(0xff);//提供额外的8个时钟
				SD_SPI_SpeedHigh();
			}
  			break;
		case EX_FLASH://外部flash
//			SPI_Flash_Init();
//			if(SPI_FLASH_TYPE==W25Q64)FLASH_SECTOR_COUNT=9832;	//W25Q64
//			else FLASH_SECTOR_COUNT=0;							//其他
 			break;
		default:
			res=1; 
	}		 
	if(res)return  STA_NOINIT;
	else return 0; //初始化成功
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

//初始化设备
DSTATUS disk_initialize 
(
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	//DSTATUS stat;
	int result;
	switch (pdrv) 
	{
		case SD_CARD:
			result = SD_Initialize();  //初始化成功则返回0
			if(result)//STM32 SPI的bug,在sd卡操作失败的时候如果不执行下面的语句,可能导致SPI读写异常
			{
				SD_SPI_SpeedLow();
				SD_SPI_ReadWriteByte(0xff);//提供额外的8个时钟
				SD_SPI_SpeedHigh();
			}
			break;               
		case EX_FLASH :
			break;
	}
	if(result)
	{ return STA_NOINIT; } //初始化失败
	else 
	{ return 0; }
	
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

//读扇区
//pdrv:卷标 buff:读缓冲首地址 sector:开始扇区地址 count:扇区数
DRESULT disk_read 
(
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	u8 res;
	if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误	
	switch (pdrv) 
	{
		case SD_CARD:			
			res=SD_ReadDisk(buff,sector,count);
			if(res)//STM32 SPI的bug,在sd卡操作失败的时候如果不执行下面的语句,可能导致SPI读写异常
			{
				SD_SPI_SpeedLow();
				SD_SPI_ReadWriteByte(0xff);//提供额外的8个时钟
				SD_SPI_SpeedHigh();
				return RES_NOTRDY; //未准备
			}
			return RES_OK;

		case EX_FLASH:

			return RES_OK;

	}
	return RES_PARERR;  //无效参数
}

//写一个扇区
#if FF_FS_READONLY == 0  //ffconf.h中开启
DRESULT disk_write 
(
	BYTE  pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT  count			/* Number of sectors to write */
)
{
	DRESULT res;
	if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误	
	switch (pdrv) 
		{
			case SD_CARD:     
				res = (DRESULT)SD_WriteDisk((u8*)buff,sector,count); //写入成功返回0
				return res;
		
			case EX_FLASH:			

				return res;
	  }
		return RES_PARERR;
}
#endif


//其他磁盘控制
DRESULT disk_ioctl
(
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,	  	/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
//	int result;

	switch (pdrv) 
		{
	       case SD_CARD : 
					switch(cmd)
					{
						case CTRL_SYNC:         
							SD_CS=0;													
							if(SD_WaitReady()==0)res = RES_OK; 
							else res = RES_ERROR;	  
							SD_CS=1;
							break;	 
						case GET_SECTOR_SIZE:
							*(WORD*)buff = 512;
								res = RES_OK;
								break;	 
						case GET_BLOCK_SIZE:
							*(WORD*)buff = 8;
							res = RES_OK;
							break;	 
						case GET_SECTOR_COUNT:
							*(DWORD*)buff = SD_GetSectorCount();
							res = RES_OK;
							break;
						default:
							res = RES_PARERR;
							break;
					}
					return res;
	       case EX_FLASH :
					switch(cmd)
						{
							default:
								res = RES_PARERR;
								break;
						}
						return res;		                

	 }
	return RES_PARERR;
}

DWORD get_fattime (void)
{				 
	return 0;
}			 

