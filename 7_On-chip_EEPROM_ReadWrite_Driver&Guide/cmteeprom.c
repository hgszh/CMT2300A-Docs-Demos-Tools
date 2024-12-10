


/**************************************************************************************************

    CMT2300A 内部包含一块 32 x 16 bits 的 EEPROM。其中地址 0 – 25 以及地址 31 为 CMOSTEK 内部使
用，地址 26 – 30 一共 5 个 word（每个 word 16 比特）为空白空间，可提供给用户使用。

    对于 CMOSTEK 来说， EEPROM 的作用是储存一些内部参数，让芯片可以正常工作。 EEPROM 会在芯片
出厂的时候被烧写并完成校验。在芯片出厂之后， EEPROM 只会在每次 POR（上电复位释放）后一次性将储
存的参数复制到内部寄存器，这些寄存器在 SLEEP 状态下仍然会生效。一般应用情况下，除了 POR 后的一次
性复制，其余时间芯片内部不会再对 EEPROM 进行擦除，烧写和读取操作

   在开始 EEPROM 操作之前，用户首先要确定 CMT2300A 停留在 STBY 状态下。 对于 EEPROM 的每一个
Word，每次要更改其数据，都必须按照“擦除 --> 烧写 --> 读取校验”的流程来操作。 下面用伪代码的形式，
来介绍完整的 EEPROM 的操作流程。 用户需要谨记可操作的地址范围是 26 – 30，如果操作了其它地址， 有可
能会导致芯片永久失效，这里假设用户要操作的第一个地址为 26：



1. 开始操作
go_eeprom // 在 STBY 状态下，发送 go_eeprom 将芯片切换到 EEPROM ACC 状态
get CHIP_MODE_STA = 0111 // 确认已经切换到 EEPROM ACC 状态
set EEPROM_NPROT 1 // 打开 EEPROM 的非保护位
set EEPROM_PWRON 1 // 打开 EEPROM 的电源开关
set EEPROM_START 1 // 打开 EERROM 操作流程开始位
set EEPROM_CONF_EN 1 // 打开 EEPROM 配置使能位
get EEPROM_CONF_EN = 1 // 确认配置使能位被打开，如果这位没有打开，而又开始了下面的流程，那
// 就会误操作其它地址，导致芯片永久失效


2. 擦除操作
set EEPROM_ADDR_CONF 26 // 将待擦除地址设为 26（十进制）
set EEPROM_ERASE 1 // 将擦除触发位置 1
get EEPROM_ERASE = 0 // 查询擦除标志位，直到它被芯片清 0，证明擦除完成
// 继续重复这 3 部，擦除其余目标地址，不想更改内容的地址无需擦除


3. 烧写操作
set EEPROM_ADDR_CONF 26 // 将待烧写地址设为 26（十进制）
set EEPROM_DATA_LOW B // 将 16-bit 的数据的低 8 位写入这个寄存器，这里假设数据为 B
set EEPROM_DATA_HIG A // 将 16-bit 的数据的高 8 位写入这个寄存器，这里假设数据为 A
set EEPROM_PROG 1 // 将烧写触发位置 1
get EEPROM_PROG = 0 // 查询烧写标志位，直到它被芯片清 0，证明烧写完成
// 继续重复这 5 部， 烧写其余目标地址， 之前没有被擦除的地址不能被烧写


4. 读取校验操作
set EEPROM_ADDR_CONF 26 // 将待读取校验地址设为 26（十进制）
set EEPROM_READ 1 // 将读取触发位置 1
get EEPROM_READ = 0 // 查询读取标志位，直到它被芯片清 0，证明读取完成
get EEPROM_DATA_LOW = B // 从这个寄存器读出 16-bit 的数据的低 8 位， 并与期待的数值 B 做对比
get EEPROM_DATA_HIG = A // 从这个寄存器读出 16-bit 的数据的高 8 位，并与期待的数值 A 做对比
// 继续重复这 5 部， 读取校验其余目标地址，之前没有被烧写的地址无需读取校验


5. 结束操作
set EEPROM_STOP 1 // 将 EEPROM 操作流程结束位置 1
set EEPROM_PWRON 1 // 打开 EEPROM 的电源开关
set EEPROM_NPROT 1 // 打开 EEPROM 的非保护位
set EEPROM_START 0 // 将 EERROM 操作流程开始位清零
set EEPROM_STOP 0 // 将 EERROM 操作流程结束位清零
go_stby or go_sleep // 将芯片切换回 STBY 或者 SLEEP 状态，根据用户需求而定
get CHIP_MODE_STA = 0010 / 0001 // 确认已经切换到 STBY 或者 SLEEP 状态


   上述流程中，有某些寄存器比特都处于同一个寄存器地址中，用户不能为了减少程序执行时间而一次性地
写入这些寄存器，要严格按照流程来一步一步操作，因为这是芯片内部设计的要求，否则会有概率性出现无法
预知的错误。

   EEPROM 的 ERASE/PROG 的最大时间是 10 ms 左右， READ 的最大时间大概是 1 ms 左右， 根据环境和
工艺而定，因此流程中建议用户通过查询标志位的方式来确定操作是否完成，而不是等待时间的方式。如果用
户需要，可以使用等待 10 ms 或更多的时间来替代查询 EEPROM_ERASE 或 EEPROM_PROG 或EEPROM_READ 的
标志位的方式来写程序。

   在芯片出厂之前， EEPROM 经过了严格的坏点筛选，最大限度地保证了良率。然而在实际使用中，就如所
有芯片一样，都无法避免 100%的良率，因此上述流程中，如果在某一步的寄存器结果对比或者数据读取校验
步骤出错，那就代表芯片是不良的，用户无需在 MCU 程序做特殊处理。 在调试阶段，用户一般无法碰见这样
的情况，如果出现错误，一般都是程序没有严格准守流程的原因造成的。
***************************************************************************************************/

#include "cmteeprom.h"

u8 go_eeprom(void)
{
	return CMT2300A_AutoSwitchStatus(0x01);
}


void Step1(void)//开始
{
	u8 tem;
	CMT2300A_GoStby();
	go_eeprom();
	
	tem  = CMT2300A_ReadReg(0x62);
	tem |= 0x01;      //EEPROM_NPROT 1 // 打开 EEPROM 的非保护位
	CMT2300A_WriteReg(0x62, tem);
	
	tem  = CMT2300A_ReadReg(0x62);
	tem |= 0x02;      //EEPROM_PWRON 1 // 打开 EEPROM 的电源开关
	CMT2300A_WriteReg(0x62, tem);
	
	tem  = CMT2300A_ReadReg(0x62);
	tem |= 0x04;      //EEPROM_START 1 // 打开 EERROM 操作流程开始位
	CMT2300A_WriteReg(0x62, tem);
	
	do{
  tem  = CMT2300A_ReadReg(0x7A);
	tem |= 0x20;      //EEPROM_CONF_EN 1 // 打开 EEPROM 配置使能位
	CMT2300A_WriteReg(0x7A, tem);
	}while((CMT2300A_ReadReg(0x7A)&0x20)!=0x20);
}

void Step2(u8 addr)//擦除
{
	u8 tem;

	tem  = CMT2300A_ReadReg(0x7A);
	tem &= 0xe0;     
	tem |= addr;      //EEPROM_ADDR_CONF // 将待擦除地址设为 
	CMT2300A_WriteReg(0x7A, tem);
	
	tem  = CMT2300A_ReadReg(0x79);  
	tem |= 0x01;      //EEPROM_ERASE 1 // 将擦除触发位置 1
	CMT2300A_WriteReg(0x79, tem);
	
	do{
	delay_ms(1);	
	}while((CMT2300A_ReadReg(0x79)&0x01)==0x01);//等待擦除完成 
	
}


void Step3(u8 addr,u16 data)//烧写
{
	u8 tem;

	tem  = CMT2300A_ReadReg(0x7A);
	tem &= 0xe0;     
	tem |= addr;      //EEPROM_ADDR_CONF // 将待烧写地址设为
	CMT2300A_WriteReg(0x7A, tem);
	
	CMT2300A_WriteReg(0x76, data);
	CMT2300A_WriteReg(0x77, data>>8);
	
	tem  = CMT2300A_ReadReg(0x79);  
	tem |= 0x02;      //EEPROM_PROG 1 // 将烧写触发位置 1
	CMT2300A_WriteReg(0x79, tem);
	
	do{
	delay_ms(1);	
	}while((CMT2300A_ReadReg(0x79)&0x02)==0x02);//等待烧写完成 
	
}

u16 Step4(u8 addr)//读数据
{
	u8 tem;

	tem  = CMT2300A_ReadReg(0x7A);
	tem &= 0xe0;     
	tem |= addr;      //EEPROM_ADDR_CONF // 将待读取校验地址设为
	CMT2300A_WriteReg(0x7A, tem);
	
	tem  = CMT2300A_ReadReg(0x79);  
	tem |= 0x04;      //EEPROM_READ 1 // 将读取触发位置 1
	CMT2300A_WriteReg(0x79, tem);
	
	do{
	delay_ms(1);	
	}while((CMT2300A_ReadReg(0x79)&0x04)==0x04);//等待读取完成 
	
	return CMT2300A_ReadReg(0x77)*256|CMT2300A_ReadReg(0x76);
}

void Step5(void)//结束
{
	u8 tem;

	tem  = CMT2300A_ReadReg(0x62);
	tem |= 0x08;      //EEPROM_STOP 1 // 将 EEPROM 操作流程结束位置 1
	CMT2300A_WriteReg(0x62, tem);
	
	tem  = CMT2300A_ReadReg(0x62);
	tem |= 0x02;      //EEPROM_PWRON 1 // 打开 EEPROM 的电源开关
	CMT2300A_WriteReg(0x62, tem);
	
	tem  = CMT2300A_ReadReg(0x62);
	tem |= 0x01;      //EEPROM_NPROT 1 // 打开 EEPROM 的非保护位
	CMT2300A_WriteReg(0x62, tem);
	
	tem  = CMT2300A_ReadReg(0x62);
	tem &= ~0x04;      //EEPROM_START 0 // 将 EERROM 操作流程开始位清零
	CMT2300A_WriteReg(0x62, tem);
	
	tem  = CMT2300A_ReadReg(0x62);
	tem &= ~0x08;      //EEPROM_STOP 0 // 将 EERROM 操作流程结束位清零
	CMT2300A_WriteReg(0x62, tem);
	
	CMT2300A_GoSleep();
}



