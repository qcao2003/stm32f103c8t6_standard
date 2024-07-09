#include "stm32f10x.h"
#include "dht11.h"
#include "sys.h"
#include "Delay.h"

uint8_t  dat[5]={0x00,0x00,0x00,0x00,0x00};    //�洢��ȡ����ʪ����Ϣ
uint32_t sum=0;         //���У��ʱ�����
 
void DHT_GPIO_Config_Output( void )
{		
	GPIO_InitTypeDef GPIO_InitStruct;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
 
void DHT_GPIO_Config_Input( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
 
 
uint8_t DHT_Read_Byte(void)
{
	uint8_t temp;  
	uint8_t ReadDat=0; 
	uint8_t t = 0;
	uint8_t i; 
 
	for(i=0;i<8;i++)
	{
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0&&t<100)  
		{		
			Delay_us(1);
			t++;  //��ֹ����
		}
		t=0;
		//���ڡ�0������ߵ�ƽʱ��26~28us��1����ߵ�ƽʱ��70us����ʱ30us�����жϸߵ͵�ƽ������0��ȡ�����ǵ͵�ƽ���ߵ�ƽ��֮
		Delay_us(30);
		temp=0;
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==1) temp=1;		
			
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==1&&t<100)
		{		
			Delay_us(1);
			t++;
		}
		t=0;
		ReadDat<<=1; 
		ReadDat|=temp;
	}	
	return ReadDat;
}
 
uint8_t DHT_Read(void)
{
	uint8_t i;
	uint8_t t = 0;
	DHT_GPIO_Config_Output();
	DHT11_LOW ;  //����
	Delay_ms(18);
	DHT11_HIGH;  //����
	Delay_us(40);
	DHT_GPIO_Config_Input();
	Delay_us(20);
	//��ʱ20us���͵�ƽ80us����ʣ60us������Ƿ��ǵ͵�ƽ��ȷ���Ƿ�����Ӧ�ź�
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)  //�����ȡ���͵�ƽ��֤��DHT11��Ӧ
	{
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0&&t<100)//������Ӧ�źŵ͵�ƽʣ��60us���ȴ���ߵ�ƽ
		{
			Delay_us(1);
			t++;			
		}
		t=0;//����100us�Զ��������У����⿨��
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==1&&t<100)//������Ӧ�źŸߵ�ƽ80us���ȴ���͵�ƽ
			{
				Delay_us(1);
				t++;			
			}
		t=0;
		for(i=0;i<5;i++)  //����40λ����
			{
				dat[i]=DHT_Read_Byte();//����1���ֽ�
			}
		Delay_us(50);//�����ź�
	}
	sum=dat[0]+dat[1]+dat[2]+dat[3];
	if(dat[4]==(u8)sum)  //У��  
	{
		return 1;    
	}
	else
		return 0;    
}

