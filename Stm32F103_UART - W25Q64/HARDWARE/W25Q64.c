#include "stm32f10x.h"                  // Device header
#include "spi.h"
#include "W25Q64_Ins.h"
#include "W25Q64.h"

/**
  * @brief  芯片读写初始化函数
  * @param  无
  * @retval 无
  */
void W25Q64_Init(void)
{
	MySPI_Init();
}

/**
  * @brief  读取设备ID
  * @param  MID		指向厂商ID变量的指针，厂商ID为8位ID变量 
  * @param  DID		指向设备ID变量的指针，设备ID为16位变量
  * @retval 无
  */
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_JEDEC_ID);		// 读ID号指令
	
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	// 厂商ID，默认为0xEF
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	// 设备ID，表示存储类型，默认为0x40
	*DID <<= 8;
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);	// 设备ID，表示容量，默认为0x17
	
	MySPI_Stop();
}

/**
  * @brief  发送写使能命令
  * @param  无
  * @retval 无
  */
void W25Q64_WriteEnable(void)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	MySPI_Stop();
}

/**
  * @brief  带超时的等待忙状态函数
  * @param  无
  * @retval 无
  */
void W25Q64_WaitBusyWithTimeout(void)
{
	uint32_t Timeout = 100000;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 1)	// 利用连续读出状态寄存器，实现等待Busy的功能
	{
		Timeout --;
		if (Timeout == 0)
		{
			/* 可以在这里添加超时错误函数 */
			break;
		}
	}
	MySPI_Stop();
}

/**
  * @brief  页编程（写入）函数
  * @param  Address		写入目标的24位首地址，连续写入时地址指针自动增1
  * @param  DataArray	写入数组的地址指针
  * @param  Count		写入数据的长度
  * @retval 无	
  */
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	W25Q64_WriteEnable();		// 时序结束后W25Q64会自动写失能
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);		// 自动舍弃高位
	MySPI_SwapByte(Address);			// 自动舍弃高位
	for (i = 0; i < Count; i ++)
	{
		MySPI_SwapByte(DataArray[i]);
	}
	MySPI_Stop();
	
	W25Q64_WaitBusyWithTimeout();
}

/**
  * @brief  页擦除函数，在执行写入操作前要进行擦除
  * @param  Address		擦除页的首地址
  * @retval 无
  */
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();		// 时序结束后W25Q64会自动写失能
	
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);		// 自动舍弃高位
	MySPI_SwapByte(Address);			// 自动舍弃高位
	MySPI_Stop();
	
	W25Q64_WaitBusyWithTimeout();		// 事后等待，优点是函数之外存储器一定不忙，缺点是会牺牲一点代码执行效率
}


/**
  * @brief  读取数据函数
  * @param  Address		读取目标的24位首地址，连续写入时地址指针自动增1
  * @param  DataArray	存放数据的数组的地址指针
  * @param  Count		读取数据的长度
  * @retval 无	
  */
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);
	
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);		// 自动舍弃高位
	MySPI_SwapByte(Address);			// 自动舍弃高位
	
	for (i = 0; i < Count; i ++)
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	
	MySPI_Stop();
}


