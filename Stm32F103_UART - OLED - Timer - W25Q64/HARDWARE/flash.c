#include "flash.h" 
#include "spi.h"
#include "Delay.h"   
#include "uart1.h"
//Mini STM32������
//W25X16 �������� 
//����ԭ��@ALIENTEK
//2010/6/13
//V1.0
u16 SPI_FLASH_TYPE=W25Q64;//Ĭ�Ͼ���25Q64
//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25X16
//����Ϊ2M�ֽ�,����32��Block,512��Sector 

//��ʼ��SPI FLASH��IO��
void SPI_Flash_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;  //SPI CS Ƭѡ�ź�
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
	SPI1_Init();		   //��ʼ��SPI
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_4);	//����Ϊ18Mʱ��,����ģʽ
	SPI1_SetSpeed(SPI_BaudRatePrescaler_16);	//����Ϊ4Mʱ�ӣ�����ģʽ
	SPI_FLASH_TYPE=SPI_Flash_ReadID();//��ȡFLASH ID.
	printf("FlashID is 0X%X\r\n",SPI_FLASH_TYPE);
}  

//��ȡSPI_FLASH��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
u8 SPI_Flash_ReadSR(void)   
{  
	u8 byte=0;   
	SPI_FLASH_CS_ENABLE;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI1_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
	SPI_FLASH_CS_DISABLE;                            //ȡ��Ƭѡ     
	return byte;   
} 
//дSPI_FLASH״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void SPI_FLASH_Write_SR(u8 sr)   
{   
	SPI_FLASH_CS_ENABLE;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	SPI1_ReadWriteByte(sr);               //д��һ���ֽ�  
	SPI_FLASH_CS_DISABLE;                            //ȡ��Ƭѡ     	      
}   
//SPI_FLASHдʹ��	
//��WEL��λ   
void SPI_FLASH_Write_Enable(void)   
{
	SPI_FLASH_CS_ENABLE;                            //ʹ������   
  SPI1_ReadWriteByte(W25X_WriteEnable);      //����дʹ��  
	SPI_FLASH_CS_DISABLE;                            //ȡ��Ƭѡ     	      
} 
//SPI_FLASHд��ֹ	
//��WEL����  
void SPI_FLASH_Write_Disable(void)   
{  
	SPI_FLASH_CS_ENABLE;                            //ʹ������   
  SPI1_ReadWriteByte(W25X_WriteDisable);     //����д��ָֹ��    
	SPI_FLASH_CS_DISABLE;                            //ȡ��Ƭѡ     	      
} 			    
//��ȡоƬID W25Q64��ID:0XEF16
u16 SPI_Flash_ReadID(void)
{
	u16 Temp = 0;	  
	SPI_FLASH_CS_ENABLE;				    
	SPI1_ReadWriteByte(0x90);//���Ͷ�ȡID����	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	SPI_FLASH_CS_DISABLE;				    
	return Temp;
}   		   

//��ȡоƬJedecDeviceID W25Q64��ID:0XEF16
u32 SPI_Flash_Read_JedecDeviceID(void)
{
	u32 Temp = 0;	  
	SPI_FLASH_CS_ENABLE;				    
	SPI1_ReadWriteByte(W25X_JedecDeviceID);//���Ͷ�ȡID����	    
	Temp |= SPI1_ReadWriteByte(Dummy_Byte) << 16; 	    
	Temp |= SPI1_ReadWriteByte(Dummy_Byte) << 8;
	Temp |= SPI1_ReadWriteByte(Dummy_Byte);	 
	SPI_FLASH_CS_DISABLE;				    
	return Temp;
} 


//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
	u16 i;    												    
	SPI_FLASH_CS_ENABLE;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_ReadData);         //���Ͷ�ȡ���� 
	//SPI1_ReadWriteByte(W25X_FastReadData);         //���Ϳ��ٶ�ȡ����
	printf("ReadAddr is 0x%X\r\n",ReadAddr);
	SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ    
	SPI1_ReadWriteByte((u8)((ReadAddr)>>8));   
	SPI1_ReadWriteByte((u8)ReadAddr);   
	for(i=0;i<NumByteToRead;i++)
	{ 
			pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //ѭ������  
			//printf("This Read byte is 0x%X\r\n",pBuffer[i]);
	}
	SPI_FLASH_CS_DISABLE;                            //ȡ��Ƭѡ     	      
}  
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
  SPI_FLASH_Write_Enable();                  //SET WEL 
	SPI_FLASH_CS_ENABLE;                            //ʹ������   
  SPI1_ReadWriteByte(W25X_PageProgram);      //����дҳ����  
	printf("WriteAddr is 0x%X\r\n",WriteAddr);	
	SPI1_ReadWriteByte((u8)((WriteAddr)>>16)); //����24bit��ַ    
	SPI1_ReadWriteByte((u8)((WriteAddr)>>8));   
	SPI1_ReadWriteByte((u8)WriteAddr);   
	for(i=0;i<NumByteToWrite;i++)
	{
		SPI1_ReadWriteByte(pBuffer[i]);//ѭ��д�� 
		//printf("The Write byte is 0x%X\r\n",pBuffer[i]);
	} 
	SPI_FLASH_CS_DISABLE;                            //ȡ��Ƭѡ 
	SPI_Flash_Wait_Busy();					   //�ȴ�д�����
	printf("Writing is done\r\n");

} 
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void SPI_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		SPI_Flash_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)  		   
u8 SPI_FLASH_BUF[4096];  //һ��������4096���ֽ�
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    

	secpos=WriteAddr/4096;//������ַ 0~511 for w25x16 �����ǰ��ַ����4096�õ����ǵ�ǰ��ַ���ڵĵڼ�����
	secoff=WriteAddr%4096;//�������ڵ�ƫ�� 
	secremain=4096-secoff;//����ʣ��ռ��С   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		SPI_Flash_Read(SPI_FLASH_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)
			{
				printf("This unit is not 0XFF\r\n");
				break;//��Ҫ����
			}  	  
		}
		if(i<secremain)//��Ҫ����
		{
			SPI_Flash_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);//д���������� 
			printf("None check writing\r\n");
		}else 
		{
			SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.
			printf("SPI_FLASH has been write\r\n");
		} 		
	
		if(NumByteToWrite==secremain)
		{
			printf("SPI_FLASH has been done\r\n");
			break;//д�������
		}
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		  pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		  NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 	 
}
//��������оƬ
//��Ƭ����ʱ��:
//W25X16:25s 
//W25X32:40s 
//W25X64:40s 
//�ȴ�ʱ�䳬��...
void SPI_Flash_Erase_Chip(void)   
{                                             
	SPI_FLASH_Write_Enable();                  //SET WEL 
	SPI_Flash_Wait_Busy();   
	SPI_FLASH_CS_ENABLE;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_ChipErase);        //����Ƭ��������  
	SPI_FLASH_CS_DISABLE;                            //ȡ��Ƭѡ     	      
	SPI_Flash_Wait_Busy();   				   //�ȴ�оƬ��������
	printf("Erasing is done\r\n");
}   
//����һ������
//Dst_Addr:������ַ 0~511 for w25x16
//����һ��ɽ��������ʱ��:150ms
void SPI_Flash_Erase_Sector(u32 Dst_Addr)   
{   
	printf("Erasing is the %d Sector\r\n",Dst_Addr);
	Dst_Addr*=4096;					//һ��������4096���ֽ�
	SPI_FLASH_Write_Enable();                  //SET WEL 
	SPI_Flash_Wait_Busy();   
	SPI_FLASH_CS_ENABLE;                            //ʹ������ 
	SPI1_ReadWriteByte(W25X_SectorErase);      //������������ָ�� 
	SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ    
	SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));   
	SPI1_ReadWriteByte((u8)Dst_Addr);  
	SPI_FLASH_CS_DISABLE;                            //ȡ��Ƭѡ     
	SPI_Flash_Wait_Busy();   				   //�ȴ��������
}  
//�ȴ�����
void SPI_Flash_Wait_Busy(void)   
{   
	uint32_t TimeOut = 10000;

	while ((SPI_Flash_ReadSR()&0x01)==0x01)
	{
		if(--TimeOut == 0)
			break;
		printf("Flash is busy\r\n");
	}   // �ȴ�BUSYλ���
}  
//�������ģʽ
void SPI_Flash_PowerDown(void)   
{ 
	SPI_FLASH_CS_ENABLE;                            //ʹ������   
  SPI1_ReadWriteByte(W25X_PowerDown);        //���͵�������  
	SPI_FLASH_CS_DISABLE;                            //ȡ��Ƭѡ     	      
	Delay_us(3);                               //�ȴ�TPD  
}   
//����
void SPI_Flash_WAKEUP(void)   
{  
	SPI_FLASH_CS_ENABLE;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	SPI_FLASH_CS_DISABLE;                            //ȡ��Ƭѡ     	      
	Delay_us(3);                               //�ȴ�TRES1
}   

void Write_page_with_check_Sector(u8* TEXT_Buffer,u32 FLASH_SIZE,u16 NumByteToWrite)
{
	u8 datatemp[10];
	SPI_Flash_Read(datatemp,FLASH_SIZE,1);
	printf("datatemp[0] is 0x%X\r\n",datatemp[0]);
	if(datatemp[0]!=0XFF)
		SPI_Flash_Erase_Sector((FLASH_SIZE)/4096);  //ɾ����ǰ������
		
	SPI_Flash_Write_Page(TEXT_Buffer,FLASH_SIZE,NumByteToWrite);
}



