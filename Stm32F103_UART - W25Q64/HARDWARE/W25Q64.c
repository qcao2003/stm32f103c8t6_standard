#include "stm32f10x.h"                  // Device header
#include "spi.h"
#include "W25Q64_Ins.h"
#include "W25Q64.h"

/**
  * @brief  оƬ��д��ʼ������
  * @param  ��
  * @retval ��
  */
void W25Q64_Init(void)
{
	MySPI_Init();
}

/**
  * @brief  ��ȡ�豸ID
  * @param  MID		ָ����ID������ָ�룬����IDΪ8λID���� 
  * @param  DID		ָ���豸ID������ָ�룬�豸IDΪ16λ����
  * @retval ��
  */
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_JEDEC_ID);		// ��ID��ָ��
	
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	// ����ID��Ĭ��Ϊ0xEF
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	// �豸ID����ʾ�洢���ͣ�Ĭ��Ϊ0x40
	*DID <<= 8;
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);	// �豸ID����ʾ������Ĭ��Ϊ0x17
	
	MySPI_Stop();
}

/**
  * @brief  ����дʹ������
  * @param  ��
  * @retval ��
  */
void W25Q64_WriteEnable(void)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	MySPI_Stop();
}

/**
  * @brief  ����ʱ�ĵȴ�æ״̬����
  * @param  ��
  * @retval ��
  */
void W25Q64_WaitBusyWithTimeout(void)
{
	uint32_t Timeout = 100000;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 1)	// ������������״̬�Ĵ�����ʵ�ֵȴ�Busy�Ĺ���
	{
		Timeout --;
		if (Timeout == 0)
		{
			/* ������������ӳ�ʱ������ */
			break;
		}
	}
	MySPI_Stop();
}

/**
  * @brief  ҳ��̣�д�룩����
  * @param  Address		д��Ŀ���24λ�׵�ַ������д��ʱ��ַָ���Զ���1
  * @param  DataArray	д������ĵ�ַָ��
  * @param  Count		д�����ݵĳ���
  * @retval ��	
  */
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	W25Q64_WriteEnable();		// ʱ�������W25Q64���Զ�дʧ��
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);		// �Զ�������λ
	MySPI_SwapByte(Address);			// �Զ�������λ
	for (i = 0; i < Count; i ++)
	{
		MySPI_SwapByte(DataArray[i]);
	}
	MySPI_Stop();
	
	W25Q64_WaitBusyWithTimeout();
}

/**
  * @brief  ҳ������������ִ��д�����ǰҪ���в���
  * @param  Address		����ҳ���׵�ַ
  * @retval ��
  */
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();		// ʱ�������W25Q64���Զ�дʧ��
	
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);		// �Զ�������λ
	MySPI_SwapByte(Address);			// �Զ�������λ
	MySPI_Stop();
	
	W25Q64_WaitBusyWithTimeout();		// �º�ȴ����ŵ��Ǻ���֮��洢��һ����æ��ȱ���ǻ�����һ�����ִ��Ч��
}


/**
  * @brief  ��ȡ���ݺ���
  * @param  Address		��ȡĿ���24λ�׵�ַ������д��ʱ��ַָ���Զ���1
  * @param  DataArray	������ݵ�����ĵ�ַָ��
  * @param  Count		��ȡ���ݵĳ���
  * @retval ��	
  */
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);
	
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);		// �Զ�������λ
	MySPI_SwapByte(Address);			// �Զ�������λ
	
	for (i = 0; i < Count; i ++)
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	
	MySPI_Stop();
}


