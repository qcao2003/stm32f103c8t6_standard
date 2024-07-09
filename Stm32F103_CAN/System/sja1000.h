#ifndef __SJA1000_H
#define __SJA1000_H
#include "sys.h"
#include "Delay.h"
#include "stm32f10x.h"  
#include "uart1.h"
#include "exit.h"

#define CAN_CS_H GPIO_SetBits(GPIOA, GPIO_Pin_12);
#define CAN_CS_L GPIO_ResetBits(GPIOA, GPIO_Pin_12);

#define CAN_ALE_H GPIO_SetBits(GPIOA, GPIO_Pin_15);
#define CAN_ALE_L GPIO_ResetBits(GPIOA, GPIO_Pin_15);

#define CAN_WR_H GPIO_SetBits(GPIOB, GPIO_Pin_3);
#define CAN_WR_L GPIO_ResetBits(GPIOB, GPIO_Pin_3);

#define CAN_RD_H GPIO_SetBits(GPIOB, GPIO_Pin_4);
#define CAN_RD_L GPIO_ResetBits(GPIOB, GPIO_Pin_4);

#define CAN_RESET_H GPIO_SetBits(GPIOB, GPIO_Pin_5);
#define CAN_RESET_L GPIO_ResetBits(GPIOB, GPIO_Pin_5);

#define SJA_BASE_ADR     0xfd00	 //�Ĵ�����ַ�Ļ�ַ

//SJA1000�Ĵ�����ַ���壬������Peliģʽ����չ֡��ʽ
/**********************
ģʽ���ƼĴ�������λ����
************************/
#define SJA_MOD   XBYTE[SJA_BASE_ADR + 0x00]

#define RM_BIT     0x01  //��λģʽ����λ
#define LOM_BIT    0x02  //ֻ��ģʽλ
#define STM_BIT    0x04  //�Լ�ģʽλ
#define AFM_BIT    0x08  //�����˲���ģʽλ
#define SM_BIT     0x10  //˯��ģʽλ

/**********************
����Ĵ�������λ����
************************/
#define SJA_CMR    XBYTE[SJA_BASE_ADR + 0x01]
#define TR_BIT         0x01       //��������λ
#define AT_BIT         0x02        //��ֹ����λ
#define RRB_BIT        0x04        //�ͷŽ��ջ�����λ
#define CDO_BIT        0x08        //����������λ
#define SRR_BIT        0x10        //������������λ

/**********************
״̬�Ĵ�������λ����
************************/
#define SJA_SR     XBYTE[SJA_BASE_ADR + 0x02]
#define RBS_BIT           0x01                    //���ջ�����״̬λ
#define DOS_BIT           0x02                     //�������״̬λ
#define TBS_BIT           0x04                      //���ͻ�����״̬λ
#define TCS_BIT           0x08                      //�������״̬λ
#define RS_BIT            0x10                    //����״̬λ
#define TS_BIT            0x20                     //����״̬λ
#define ES_BIT            0x40                   //����״̬λ
#define BS_BIT            0x80                    //����״̬λ

/**********************
�жϼĴ�������λ����
************************/
#define SJA_IR     XBYTE[SJA_BASE_ADR + 0x03]
#define RI_BIT            0x01                     //�����ж�λ
#define TI_BIT            0x02                      //�����ж�λ
#define EI_BIT            0x04                      //���󾯸��ж�λ
#define DOI_BIT           0x08                     //��������ж�λ
#define WUI_BIT           0x10                      //�����ж�λ
#define EPI_BIT           0x20                      //���������ж�λ
#define ALI_BIT           0x40                     //�ٲö�ʧ�ж�λ
#define BEI_BIT           0x80                    //���ߴ����ж�λ

/**********************
�ж�ʹ�ܼĴ�������λ����
************************/
#define SJA_IER    XBYTE[SJA_BASE_ADR + 0x04]
#define RIE_BIT           0x01                     //�����ж�ʹ��λ
#define TIE_BIT           0x02                      //�����ж�ʹ��λ
#define EIE_BIT           0x04                      //���󾯸��ж�ʹ��λ
#define DOIE_BIT          0x08                      //��������ж�ʹ��λ
#define WUIE_BIT          0x10                      //�����ж�ʹ��λ
#define EPIE_BIT          0x20                      //���������ж�ʹ��λ
#define ALIE_BIT          0x40                      //�ٲö�ʧ�ж�ʹ��λ
#define BEIE_BIT          0x80                     //���ߴ����ж�ʹ��λ


#define SJA_BTR0   XBYTE[SJA_BASE_ADR + 0x06]	  //���߶�ʱ��0�Ĵ���
#define SJA_BTR1   XBYTE[SJA_BASE_ADR + 0x07]	  //���߶�ʱ��1�Ĵ���
#define SAM_BIT           0x80               //����ģʽλ��0==���߱�����1�Σ�1== ���߱�����3��

/**********************
������ƼĴ�������λ����
************************/
#define SJA_OCR    XBYTE[SJA_BASE_ADR + 0x08]
           /*OCMODE1 ��OCMODE0 */
#define BiPhaseMode       0x00                      //˫�����ģʽ
#define NormalMode        0x02                  //�������ģʽ
#define ClkOutMode        (0x01|0x02)       //ʱ�����ģʽ
           /*TX1 ������ܽ�����*/
#define OCPOL1_BIT        0x20                  //������Կ���λ
#define Tx1Float          0x00                      //����Ϊ����
#define Tx1PullDn         0x40                  //����Ϊ����
#define Tx1PullUp         0x80                 //����Ϊ����
#define Tx1PshPull        (0x40|0x80)       //����Ϊ����
            /*TX0 ������ܽ�����*/
#define OCPOL0_BIT        0x04                      //������Կ���λ
#define Tx0Float          0x00                      //����Ϊ����
#define Tx0PullDn         0x08                      //����Ϊ����
#define Tx0PullUp         0x10                      //����Ϊ����
#define Tx0PshPull        (0x10|0x08)                    //����Ϊ����

#define SJA_TEST   XBYTE[SJA_BASE_ADR + 0x09]         //���ԼĴ���

/********************************
 * #define SJA_10   XBYTE[SJA_BASE_ADR + 0x0a]   �Ĵ������ܱ���
 ********************************/

/**********************
�����Ĵ�������λ����
************************/
#define SJA_ALC      XBYTE[SJA_BASE_ADR + 0x0b]		//�ٲö�ʧ��׽�Ĵ���
#define SJA_ECC      XBYTE[SJA_BASE_ADR + 0x0c]		//����׽�Ĵ���
#define SJA_EWLR     XBYTE[SJA_BASE_ADR + 0x0d]		//���󱨾����ƼĴ���
#define SJA_RXERR    XBYTE[SJA_BASE_ADR + 0x0e]		//RX ����������Ĵ���
#define SJA_TXERR    XBYTE[SJA_BASE_ADR + 0x0f]		//TX ����������Ĵ���

/**********************
�����˲����Ĵ�������λ����
************************/
#define SJA_ACR0     XBYTE[SJA_BASE_ADR + 0x10]		//���մ���0�Ĵ���
#define SJA_ACR1     XBYTE[SJA_BASE_ADR + 0x11]		//���մ���1�Ĵ���
#define SJA_ACR2     XBYTE[SJA_BASE_ADR + 0x12]		//���մ���2�Ĵ���
#define SJA_ACR3     XBYTE[SJA_BASE_ADR + 0x13]		//���մ���3�Ĵ���

#define SJA_AMR0     XBYTE[SJA_BASE_ADR + 0x14]		//��������0�Ĵ���
#define SJA_AMR1     XBYTE[SJA_BASE_ADR + 0x15]		//��������1�Ĵ���
#define SJA_AMR2     XBYTE[SJA_BASE_ADR + 0x16]		//��������2�Ĵ���
#define SJA_AMR3     XBYTE[SJA_BASE_ADR + 0x17]		//��������3�Ĵ���

/**********************
TX��������ַ����
************************/
#define SJA_TBSR0    XBYTE[SJA_BASE_ADR + 0x10]
#define SJA_TBSR1    XBYTE[SJA_BASE_ADR + 0x11]
#define SJA_TBSR2    XBYTE[SJA_BASE_ADR + 0x12]
#define SJA_TBSR3    XBYTE[SJA_BASE_ADR + 0x13]
#define SJA_TBSR4    XBYTE[SJA_BASE_ADR + 0x14]
#define SJA_TBSR5    XBYTE[SJA_BASE_ADR + 0x15]
#define SJA_TBSR6    XBYTE[SJA_BASE_ADR + 0x16]
#define SJA_TBSR7    XBYTE[SJA_BASE_ADR + 0x17]
#define SJA_TBSR8    XBYTE[SJA_BASE_ADR + 0x18]
#define SJA_TBSR9    XBYTE[SJA_BASE_ADR + 0x19]
#define SJA_TBSR10   XBYTE[SJA_BASE_ADR + 0x1a]
#define SJA_TBSR11   XBYTE[SJA_BASE_ADR + 0x1b]
#define SJA_TBSR12   XBYTE[SJA_BASE_ADR + 0x1c]

/**********************
RX��������ַ����
************************/
#define SJA_RBSR0    XBYTE[SJA_BASE_ADR + 0x10]
#define SJA_RBSR1    XBYTE[SJA_BASE_ADR + 0x11]
#define SJA_RBSR2    XBYTE[SJA_BASE_ADR + 0x12]
#define SJA_RBSR3    XBYTE[SJA_BASE_ADR + 0x13]
#define SJA_RBSR4    XBYTE[SJA_BASE_ADR + 0x14]
#define SJA_RBSR5    XBYTE[SJA_BASE_ADR + 0x15]
#define SJA_RBSR6    XBYTE[SJA_BASE_ADR + 0x16]
#define SJA_RBSR7    XBYTE[SJA_BASE_ADR + 0x17]
#define SJA_RBSR8    XBYTE[SJA_BASE_ADR + 0x18]
#define SJA_RBSR9    XBYTE[SJA_BASE_ADR + 0x19]
#define SJA_RBSR10   XBYTE[SJA_BASE_ADR + 0x1a]
#define SJA_RBSR11   XBYTE[SJA_BASE_ADR + 0x1b]
#define SJA_RBSR12   XBYTE[SJA_BASE_ADR + 0x1c]

#define SJA_RMC      XBYTE[SJA_BASE_ADR + 0x1d]           //RX ��Ϣ������ �Ĵ���
#define SJA_RBSA     XBYTE[SJA_BASE_ADR + 0x1e]            //RX ��������ʼ��ַ �Ĵ���

/**********************
ʼ�շ�Ƶ�Ĵ�����ַ����
************************/
#define SJA_CDR       XBYTE[SJA_BASE_ADR + 0x1f]		 //ʱ�ӷ�Ƶ �Ĵ���
#define CLKOff_BIT        0x08                     //ʱ�ӹر�λ��ʱ������ܽſ���λ
#define RXINTEN_BIT       0x20                      //���ڽ����жϵĹܽ�TX1
#define CBP_BIT           0x40                      //CAN �Ƚ�����·����λ
#define CANMode_BIT       0x80                     //CAN ģʽ����λ

#endif
