
#include "sys.h"
#include "spi.h"

//定义AFE4490寄存器地址
#define CONTROL0    0x00  //CONTROL0：控制寄存器0（地址= 00h，复位值= 0000h），只写，CONTROL0用于AFE软件和计数计时器复位，诊断使能以及SPI读取功能
#define LED2STC     0x01  //LED2启动计数寄存器（地址= 01h，复位值= 0000h），设置LED2信号采样的开始时序值
#define LED2ENDC    0x02  //LED2结束计数寄存器（地址= 02h，复位值= 0000h），设置LED2信号采样的结束时序值
#define LED2LEDSTC    0x03  //LED2 LED启动计数寄存器（地址= 03h，复位值= 0000h），设置LED2信号开启时的开始时序值。
#define LED2LEDENDC   0x04 //LED2 LED结束计数寄存器（地址= 04h，复位值= 0000h），设置LED2信号关闭时的结束时序值。
#define ALED2STC    0x05   //LED2启动环境采样计数寄存器（地址= 05h，复位值= 0000h），设置环境LED2信号采样的开始时序值。
#define ALED2ENDC   0x06   //LED2结束环境采样计数寄存器（地址= 06h，复位值= 0000h），设置环境LED2信号采样的结束时序值。
#define LED1STC     0x07   //ED1启动采样计数寄存器（地址= 07h，复位值= 0000h），设置LED1信号采样的开始时序值。
#define LED1ENDC    0x08   //LED1结束采样计数寄存器（地址= 08h，复位值= 0000h），设置LED1信号采样的结束时序值。
#define LED1LEDSTC    0x09 //LED1 LED启动计数寄存器（地址= 09h，复位值= 0000h），设置LED1信号开启时的起始时序值。
#define LED1LEDENDC   0x0a //LED1 LED结束计数寄存器（地址= 0Ah，复位值= 0000h），设置LED1信号关闭时的结束时序值
#define ALED1STC    0x0b   //采样环境LED1启动计数寄存器（地址= 0Bh，复位值= 0000h），设置环境LED1信号采样的开始时序值。
#define ALED1ENDC   0x0c   //采样环境LED1结束计数寄存器（地址= 0Ch，复位值= 0000h），设置环境LED1信号采样的结束时序值。
#define LED2CONVST    0x0d //LED2转换起始计数寄存器，设置LED2转换的开始时序值。
#define LED2CONVEND   0x0e //LED2转换结束计数寄存器（地址= 0Eh，复位值= 0000h），设置LED2转换的结束时序值。
#define ALED2CONVST   0x0f //LED2环境转换起始计数寄存器（地址= 0Fh，复位值= 0000h），设置环境LED2转换的开始时序值。
#define ALED2CONVEND  0x10 //LED2环境转换结束计数寄存器（地址= 10h，复位值= 0000h），设置环境LED2转换的结束时序值。
#define LED1CONVST    0x11 //LED1转换起始计数寄存器，设置LED1转换的开始时序值。
#define LED1CONVEND   0x12 //LED1转换结束计数寄存器（地址= 12h，复位值= 0000h），设置LED1转换的结束时序值。
#define ALED1CONVST   0x13 //LED1环境转换起始计数寄存器（地址= 13h，复位值= 0000h），设置环境LED1转换的开始时序值。
#define ALED1CONVEND  0x14 //LED1环境转换结束计数寄存器（地址= 14h，复位值= 0000h），设置环境LED1转换的结束时序值。
#define ADCRSTCNT0    0x15 //ADC0复位开始计数寄存器，设置ADC0复位转换信号的起始位置。
#define ADCRSTENDCT0  0x16 //ADC0复位结束计数寄存器（地址= 16h，复位值= 0000h），设置ADC0复位转换信号的结束位置。
#define ADCRSTCNT1    0x17 //ADC1复位开始计数寄存器（地址= 17h，复位值= 0000h），设置ADC1复位转换信号的开始位置。
#define ADCRSTENDCT1  0x18 //ADC1复位结束计数寄存器（地址= 18h，复位值= 0000h），设置ADC1复位转换信号的结束位置。
#define ADCRSTCNT2    0x19 //ADC2复位起始计数寄存器（地址= 19h，复位值= 0000h），设置ADC2复位转换信号的起始位置。
#define ADCRSTENDCT2  0x1a //ADC2复位结束计数寄存器（地址= 1Ah，复位值= 0000h），设置ADC2复位转换信号的结束位置
#define ADCRSTCNT3    0x1b //ADC3复位起始计数寄存器（地址= 1Bh，复位值= 0000h），设置ADC3复位转换信号的起始位置。
#define ADCRSTENDCT3  0x1c //ADC3复位结束计数寄存器（地址= 1Ch，复位值= 0000h），设置ADC3复位转换信号的结束位置。
#define PRPCOUNT    0x1d   //脉冲重复周期计数寄存器（地址= 1Dh，复位值= 0000h），设置设备脉冲重复周期计数。
#define CONTROL1    0x1e   //控制寄存器1（地址= 1Eh，复位值= 0000h），配置时钟警报引脚，定时器和平均次数。
#define SPARE1      0x1f   //SPARE1寄存器供将来使用（地址= 1Fh，复位值= 0000h），备用寄存器，保留供将来使用。
#define TIAGAIN     0x20    //跨阻放大器增益设置寄存器（地址= 20h，复位值= 0000h），设置器件互阻放大器的增益模式以及反馈电阻和电容值。
#define TIA_AMB_GAIN  0x21  //跨阻放大器和环境抵消级增益寄存器（地址= 21h，复位值= 0000h），配置环境光消除放大器的增益，消除电流和滤波器转折频率。
#define LEDCNTRL    0x22  //LED控制寄存器（地址= 22h，复位值= 0000h），设置LED电流范围以及LED1和LED2驱动电流
#define CONTROL2    0x23  //控制寄存器2（地址= 23h，复位值= 0000h），控制LED发射器，晶体以及AFE，发射器和接收器的电源模式
#define SPARE2      0x24  //SPARE2寄存器以备将来使用（地址= 24h，复位值= 0000h）
#define SPARE3      0x25  //SPARE3寄存器以备将来使用（地址= 25h，复位值= 0000h）
#define SPARE4      0x26  //SPARE4寄存器以备将来使用（地址= 26h，复位值= 0000h）
#define RESERVED1   0x27  //RESERVED1寄存器仅供工厂使用（地址= 27h，复位值= XXXXh）
#define RESERVED2   0x28  //RESERVED2寄存器仅供工厂使用（地址= 28h，复位值= XXXXh）
#define ALARM     0x29    //警报寄存器（地址= 29h，复位值= 0000h）
#define LED2VAL     0x2a  //LED2数字值，包含由ADC转换的最新LED2采样的数字值。每次更新该寄存器的内容时，ADC_RDY信号都会变高。在下一个采样被AFE转换之前，主机处理器必须读出该寄存器
#define ALED2VAL    0x2b  //LED2环境数字值，包含由ADC转换的最新LED2环境采样的数字值。每次更新该寄存器的内容时，ADC_RDY信号都会变高。在下一个采样被AFE转换之前，主机处理器必须读出该寄存器。
#define LED1VAL     0x2c  //LED1数字值
#define ALED1VAL    0x2d  //LED1环境数字值
#define LED2ABSVAL    0x2e //（ LED2 – LED2环境）数字值
#define LED1ABSVAL    0x2f //（ LED1 – LED1环境）数字值
#define DIAG      0x30     //诊断标志寄存器（地址= 30h，复位值= 0000h）

//定义AFE4490操作引脚，芯片2
#define AFE4490_STE_2(n)(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET))
#define AFE4490_AFEPDN_2(n)(n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_RESET))

//定义AFE4490芯片1操作引脚
#define AFE4490_STE_1(n)(n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET))
#define AFE4490_AFEPDN_1(n)(n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET))


//定义芯片2操作函数
void AFE4490_SPI2_Init(void);
void AFE4490_Init_2(void);
u8 afe4490_Write_2(u8 address, u32 data);
u32 afe4490_Read_2 (u8 address);
void AFE4490_Reg_Init_2(void);
	

//定义芯片1操作函数
void AFE4490_SPI1_Init(void);
void AFE4490_Init_1(void);
u8 afe4490_Write_1 (u8 address, u32 data);
u32 afe4490_Read_1 (u8 address);
void AFE4490_Reg_Init_1(void);

	