


/**************************************************************************************************

    CMT2300A �ڲ�����һ�� 32 x 16 bits �� EEPROM�����е�ַ 0 �C 25 �Լ���ַ 31 Ϊ CMOSTEK �ڲ�ʹ
�ã���ַ 26 �C 30 һ�� 5 �� word��ÿ�� word 16 ���أ�Ϊ�հ׿ռ䣬���ṩ���û�ʹ�á�

    ���� CMOSTEK ��˵�� EEPROM �������Ǵ���һЩ�ڲ���������оƬ�������������� EEPROM ����оƬ
������ʱ����д�����У�顣��оƬ����֮�� EEPROM ֻ����ÿ�� POR���ϵ縴λ�ͷţ���һ���Խ���
��Ĳ������Ƶ��ڲ��Ĵ�������Щ�Ĵ����� SLEEP ״̬����Ȼ����Ч��һ��Ӧ������£����� POR ���һ��
�Ը��ƣ�����ʱ��оƬ�ڲ������ٶ� EEPROM ���в�������д�Ͷ�ȡ����

   �ڿ�ʼ EEPROM ����֮ǰ���û�����Ҫȷ�� CMT2300A ͣ���� STBY ״̬�¡� ���� EEPROM ��ÿһ��
Word��ÿ��Ҫ���������ݣ������밴�ա����� --> ��д --> ��ȡУ�顱�������������� ������α�������ʽ��
������������ EEPROM �Ĳ������̡� �û���Ҫ���ǿɲ����ĵ�ַ��Χ�� 26 �C 30�����������������ַ�� �п�
�ܻᵼ��оƬ����ʧЧ����������û�Ҫ�����ĵ�һ����ַΪ 26��



1. ��ʼ����
go_eeprom // �� STBY ״̬�£����� go_eeprom ��оƬ�л��� EEPROM ACC ״̬
get CHIP_MODE_STA = 0111 // ȷ���Ѿ��л��� EEPROM ACC ״̬
set EEPROM_NPROT 1 // �� EEPROM �ķǱ���λ
set EEPROM_PWRON 1 // �� EEPROM �ĵ�Դ����
set EEPROM_START 1 // �� EERROM �������̿�ʼλ
set EEPROM_CONF_EN 1 // �� EEPROM ����ʹ��λ
get EEPROM_CONF_EN = 1 // ȷ������ʹ��λ���򿪣������λû�д򿪣����ֿ�ʼ����������̣���
// �ͻ������������ַ������оƬ����ʧЧ


2. ��������
set EEPROM_ADDR_CONF 26 // ����������ַ��Ϊ 26��ʮ���ƣ�
set EEPROM_ERASE 1 // ����������λ�� 1
get EEPROM_ERASE = 0 // ��ѯ������־λ��ֱ������оƬ�� 0��֤���������
// �����ظ��� 3 ������������Ŀ���ַ������������ݵĵ�ַ�������


3. ��д����
set EEPROM_ADDR_CONF 26 // ������д��ַ��Ϊ 26��ʮ���ƣ�
set EEPROM_DATA_LOW B // �� 16-bit �����ݵĵ� 8 λд������Ĵ����������������Ϊ B
set EEPROM_DATA_HIG A // �� 16-bit �����ݵĸ� 8 λд������Ĵ����������������Ϊ A
set EEPROM_PROG 1 // ����д����λ�� 1
get EEPROM_PROG = 0 // ��ѯ��д��־λ��ֱ������оƬ�� 0��֤����д���
// �����ظ��� 5 ���� ��д����Ŀ���ַ�� ֮ǰû�б������ĵ�ַ���ܱ���д


4. ��ȡУ�����
set EEPROM_ADDR_CONF 26 // ������ȡУ���ַ��Ϊ 26��ʮ���ƣ�
set EEPROM_READ 1 // ����ȡ����λ�� 1
get EEPROM_READ = 0 // ��ѯ��ȡ��־λ��ֱ������оƬ�� 0��֤����ȡ���
get EEPROM_DATA_LOW = B // ������Ĵ������� 16-bit �����ݵĵ� 8 λ�� �����ڴ�����ֵ B ���Ա�
get EEPROM_DATA_HIG = A // ������Ĵ������� 16-bit �����ݵĸ� 8 λ�������ڴ�����ֵ A ���Ա�
// �����ظ��� 5 ���� ��ȡУ������Ŀ���ַ��֮ǰû�б���д�ĵ�ַ�����ȡУ��


5. ��������
set EEPROM_STOP 1 // �� EEPROM �������̽���λ�� 1
set EEPROM_PWRON 1 // �� EEPROM �ĵ�Դ����
set EEPROM_NPROT 1 // �� EEPROM �ķǱ���λ
set EEPROM_START 0 // �� EERROM �������̿�ʼλ����
set EEPROM_STOP 0 // �� EERROM �������̽���λ����
go_stby or go_sleep // ��оƬ�л��� STBY ���� SLEEP ״̬�������û��������
get CHIP_MODE_STA = 0010 / 0001 // ȷ���Ѿ��л��� STBY ���� SLEEP ״̬


   ���������У���ĳЩ�Ĵ������ض�����ͬһ���Ĵ�����ַ�У��û�����Ϊ�˼��ٳ���ִ��ʱ���һ���Ե�
д����Щ�Ĵ�����Ҫ�ϸ���������һ��һ����������Ϊ����оƬ�ڲ���Ƶ�Ҫ�󣬷�����и����Գ����޷�
Ԥ֪�Ĵ���

   EEPROM �� ERASE/PROG �����ʱ���� 10 ms ���ң� READ �����ʱ������ 1 ms ���ң� ���ݻ�����
���ն�������������н����û�ͨ����ѯ��־λ�ķ�ʽ��ȷ�������Ƿ���ɣ������ǵȴ�ʱ��ķ�ʽ�������
����Ҫ������ʹ�õȴ� 10 ms ������ʱ���������ѯ EEPROM_ERASE �� EEPROM_PROG ��EEPROM_READ ��
��־λ�ķ�ʽ��д����

   ��оƬ����֮ǰ�� EEPROM �������ϸ�Ļ���ɸѡ������޶ȵر�֤�����ʡ�Ȼ����ʵ��ʹ���У�������
��оƬһ�������޷����� 100%�����ʣ�������������У������ĳһ���ļĴ�������ԱȻ������ݶ�ȡУ��
��������Ǿʹ���оƬ�ǲ����ģ��û������� MCU ���������⴦�� �ڵ��Խ׶Σ��û�һ���޷���������
�������������ִ���һ�㶼�ǳ���û���ϸ�׼�����̵�ԭ����ɵġ�
***************************************************************************************************/

#include "cmteeprom.h"

u8 go_eeprom(void)
{
	return CMT2300A_AutoSwitchStatus(0x01);
}


void Step1(void)//��ʼ
{
	u8 tem;
	CMT2300A_GoStby();
	go_eeprom();
	
	tem  = CMT2300A_ReadReg(0x62);
	tem |= 0x01;      //EEPROM_NPROT 1 // �� EEPROM �ķǱ���λ
	CMT2300A_WriteReg(0x62, tem);
	
	tem  = CMT2300A_ReadReg(0x62);
	tem |= 0x02;      //EEPROM_PWRON 1 // �� EEPROM �ĵ�Դ����
	CMT2300A_WriteReg(0x62, tem);
	
	tem  = CMT2300A_ReadReg(0x62);
	tem |= 0x04;      //EEPROM_START 1 // �� EERROM �������̿�ʼλ
	CMT2300A_WriteReg(0x62, tem);
	
	do{
  tem  = CMT2300A_ReadReg(0x7A);
	tem |= 0x20;      //EEPROM_CONF_EN 1 // �� EEPROM ����ʹ��λ
	CMT2300A_WriteReg(0x7A, tem);
	}while((CMT2300A_ReadReg(0x7A)&0x20)!=0x20);
}

void Step2(u8 addr)//����
{
	u8 tem;

	tem  = CMT2300A_ReadReg(0x7A);
	tem &= 0xe0;     
	tem |= addr;      //EEPROM_ADDR_CONF // ����������ַ��Ϊ 
	CMT2300A_WriteReg(0x7A, tem);
	
	tem  = CMT2300A_ReadReg(0x79);  
	tem |= 0x01;      //EEPROM_ERASE 1 // ����������λ�� 1
	CMT2300A_WriteReg(0x79, tem);
	
	do{
	system_delay_ms(1);	
	}while((CMT2300A_ReadReg(0x79)&0x01)==0x01);//�ȴ�������� 
	
}


void Step3(u8 addr,u16 datatemp)//��д
{
	u8 tem;

	tem  = CMT2300A_ReadReg(0x7A);
	tem &= 0xe0;     
	tem |= addr;      //EEPROM_ADDR_CONF // ������д��ַ��Ϊ
	CMT2300A_WriteReg(0x7A, tem);
	
	CMT2300A_WriteReg(0x76, datatemp);
	CMT2300A_WriteReg(0x77, datatemp>>8);
	
	tem  = CMT2300A_ReadReg(0x79);  
	tem |= 0x02;      //EEPROM_PROG 1 // ����д����λ�� 1
	CMT2300A_WriteReg(0x79, tem);
	
	do{
	system_delay_ms(1);	
	}while((CMT2300A_ReadReg(0x79)&0x02)==0x02);//�ȴ���д��� 
	
}

u16 Step4(u8 addr)//������
{
	u8 tem;

	tem  = CMT2300A_ReadReg(0x7A);
	tem &= 0xe0;     
	tem |= addr;      //EEPROM_ADDR_CONF // ������ȡУ���ַ��Ϊ
	CMT2300A_WriteReg(0x7A, tem);
	
	tem  = CMT2300A_ReadReg(0x79);  
	tem |= 0x04;      //EEPROM_READ 1 // ����ȡ����λ�� 1
	CMT2300A_WriteReg(0x79, tem);
	
	do{
	system_delay_ms(1);	
	}while((CMT2300A_ReadReg(0x79)&0x04)==0x04);//�ȴ���ȡ��� 
	
	return CMT2300A_ReadReg(0x77)*256|CMT2300A_ReadReg(0x76);
}

void Step5(void)//����
{
	u8 tem;

	tem  = CMT2300A_ReadReg(0x62);
	tem |= 0x08;      //EEPROM_STOP 1 // �� EEPROM �������̽���λ�� 1
	CMT2300A_WriteReg(0x62, tem);
	
	tem  = CMT2300A_ReadReg(0x62);
	tem |= 0x02;      //EEPROM_PWRON 1 // �� EEPROM �ĵ�Դ����
	CMT2300A_WriteReg(0x62, tem);
	
	tem  = CMT2300A_ReadReg(0x62);
	tem |= 0x01;      //EEPROM_NPROT 1 // �� EEPROM �ķǱ���λ
	CMT2300A_WriteReg(0x62, tem);
	
	tem  = CMT2300A_ReadReg(0x62);
	tem &= ~0x04;      //EEPROM_START 0 // �� EERROM �������̿�ʼλ����
	CMT2300A_WriteReg(0x62, tem);
	
	tem  = CMT2300A_ReadReg(0x62);
	tem &= ~0x08;      //EEPROM_STOP 0 // �� EERROM �������̽���λ����
	CMT2300A_WriteReg(0x62, tem);
	
	CMT2300A_GoSleep();
}



