#include "include.h"

//--------------------------------------------------------
//函数名称: void CMT2300A_SPI_Init(void)
//函数功能：SPI初始化
//输入参数：无
//输出参数：无
//--------------------------------------------------------
void CMT2300A_SPI_Init(void)
{
    stc_spi_config_t  SPIConfig;
    //P2.5:CLK;P2.3:MISO;P2.4:MOSI
    //P0.3:CSB;P2.6:FCSB
    //P0.1:GPIO1;P0.2:GPIO2
    Clk_SetPeripheralGate(ClkPeripheralSpi,TRUE);
    Reset_SetPeripheralReset(ResetSpi,FALSE);
    Reset_SetPeripheralReset(ResetSpi,TRUE);

    Gpio_SetFunc_SPIMISO_P23();
    Gpio_SetFunc_SPIMOSI_P24();
    Gpio_SetFunc_SPICLK_P25();
    
    //配置SPI
    SPIConfig.bCPHA = Spicphafirst;
    SPIConfig.bCPOL = Spicpollow;
    SPIConfig.bIrqEn = FALSE;
    SPIConfig.bMasterMode = SpiMaster;
    SPIConfig.u8BaudRate = SpiClkDiv4;//系统时钟分频,4MHZ
    SPIConfig.pfnIrqCb = NULL;
    Spi_Init(&SPIConfig);
    
    Gpio_InitIOExt(0, 3,GpioDirOut,FALSE ,FALSE ,FALSE ,FALSE);
    CSB_H();
    Gpio_InitIOExt(2, 6,GpioDirOut,FALSE ,FALSE ,FALSE ,FALSE);
    FCSB_H();
    Gpio_InitIOExt(0, 1,GpioDirIn,FALSE ,FALSE ,FALSE ,FALSE);
    Gpio_InitIOExt(0, 2,GpioDirIn,FALSE ,FALSE ,FALSE ,FALSE);
}
//--------------------------------------------------------
//函数名称: u8 SPI_SendByte(u8 byte)
//函数功能：SPI发送数据
//输入参数：发送数据
//输出参数：接收数据
//--------------------------------------------------------
static u8 SPI_SendByte(u8 byte)
{
    u8 dat;
    u16 TimeOut = 1000;
    
    M0P_SPI->DATA = byte;
    while(TimeOut--)
    {
        if(TRUE == M0P_SPI->STAT_f.SPIF) break;
    }
    if(TimeOut == 0)
    {
        CMT2300A_SPI_Init();
        return 0xff;
    }
    dat = (u8)M0P_SPI->DATA;
    return dat;
}
//--------------------------------------------------------
//函数名称: void Delay(u32 Cont)
//函数功能：延时;1=1.62us;2=1.88us;3=2.14us;4=2.38us;8=3.38us
//输入参数：无
//输出参数：无
//--------------------------------------------------------
void Delay(u32 Cont)
{
	for(;Cont!=0;Cont--);
}
//--------------------------------------------------------
//函数名称: void CMT2300A_SPI_Write(u8 adder,u8 Data)
//函数功能：写寄存器
//输入参数：无
//输出参数：无
//--------------------------------------------------------
void CMT2300A_SPI_Write(u8 addr,u8 Data)
{
    CSB_L();
    SPI_SendByte(addr & 0x7F);
    SPI_SendByte(Data);
    FCSB_H();
    CSB_H();
}
//--------------------------------------------------------
//函数名称: u8 CMT2300A_SPI_Read(u8 Data)
//函数功能：读寄存器
//输入参数：无
//输出参数：无
//--------------------------------------------------------
u8 CMT2300A_SPI_Read(u8 Data)
{
    u8 dat;
    
    CSB_L();
    SPI_SendByte(Data | 0x80);
    dat = SPI_SendByte(0x00);
    FCSB_H();
    CSB_H();
    return dat;
}
//--------------------------------------------------------
//函数名称: u8 SPI_Read(u8 Data)
//函数功能：读寄存器
//输入参数：无
//输出参数：无
//--------------------------------------------------------
void CMT2300A_ReadFifo(u8 *buf, u8 len)
{
    for(u8 i = 0;i < len;i++)
    {
        FCSB_L();
        Delay(1);
        buf[i] = SPI_SendByte(0x00);
        Delay(1);
        CSB_H();
        FCSB_H();
    }
}
//--------------------------------------------------------
//函数名称: u8 SPI_Read(u8 Data)
//函数功能：读寄存器
//输入参数：无
//输出参数：无
//--------------------------------------------------------
void CMT2300A_WriteFifo(u8 *buf, u8 len)
{
    for(u8 i = 0;i < len;i++)
    {
        FCSB_L();
        Delay(1);
        SPI_SendByte(buf[i]);
        Delay(1);
        CSB_H();
        FCSB_H();
    }
}


