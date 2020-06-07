#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h"
#include "key.h"
#include "ltdc.h"
#include "lcd.h"
#include "sdram.h"
#include "usmart.h"
#include "mpu.h"
#include "malloc.h"
#include "w25qxx.h"
#include "sdmmc_sdcard.h"
#include "nand.h"    
#include "ftl.h"  
#include "ff.h"
#include "exfuns.h"
#include "afe4490.h"
#include "exti.h"

//AFE4490部分
u32 IRtemp1;
u32 REDtemp1;
u32 IRtemp2;
u32 REDtemp2;
u8  ReadLED;

void EXTI0_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
int main(void)
{
 	u32 total,free;
	u8 t=0;	
	u8 res=0;	
	
	
	
	Cache_Enable();                			//打开L1-Cache
	MPU_Memory_Protection();        		//保护相关存储区域
	HAL_Init();				        		//初始化HAL库
	Stm32_Clock_Init(160,5,2,4);  		    //设置时钟,400Mhz 
	delay_init(400);						//延时初始化
	uart_init(115200);						//串口初始化
	usmart_dev.init(200); 		    		//初始化USMART	
	LED_Init();								//初始化LED
	KEY_Init();								//初始化按键
	SDRAM_Init();                   		//初始化SDRAM
//	LCD_Init();								//初始化LCD
//       W25QXX_Init();				   		 	//初始化W25Q256
 	my_mem_init(SRAMIN);		    		//初始化内部内存池
	my_mem_init(SRAMEX);		    		//初始化外部内存池
	my_mem_init(SRAMDTCM);		    		//初始化CCM内存池 
	EXTI_Init();
	
	//AFE4490部分
	AFE4490_Init_2();
 	AFE4490_Reg_Init_2();
	delay_ms(100);
//	AFE4490_Init_1();
//       AFE4490_Reg_Init_1();
//	
   	      
 				    
	while(1)
	{
//		afe4490_Write_2(CONTROL0,0x000001);
//		IRtemp2=afe4490_Read_2(LED1VAL);
////		printf("AFE4490 IR:%d \r\n",IRtemp2);
//		afe4490_Write_2(CONTROL0,0x000001);
//		REDtemp2=afe4490_Read_2(LED2VAL);
//		printf("%d,0,0,0\r\n",REDtemp2);
//		
		
//		AFE4490Write(TIAGAIN,0x000000);	// CF = 5pF, RF = 500kR
//              AFE4490Write(TIA_AMB_GAIN,0x000005);	// Timers ON, average 3 samples 
//              AFE4490Write(LEDCNTRL,0x0011414);	// Switch to READ mode  
//              AFE4490Write(CONTROL2,0x010000);	// LED_RANGE=100mA, LED=50mA 
//              AFE4490Write(CONTROL1,0x010707);	// Timers ON, average 3 samples 
//              AFE4490Write(CONTROL0,0x000001);	// Switch to READ mode   
//              IRtemp=AFE4490Read(LED1VAL);
//		printf("AFE4490 IR:%d \r\n",IRtemp); 
//              REDtemp=AFE4490Read(LED2VAL);
//		printf("AFE4490 RED:%d \r\n",REDtemp);
		if(ADC_RDY==1){
//		   printf("receive");
		   if(ReadLED==0){
	        afe4490_Write_2(CONTROL0,0x000001);
		IRtemp2=afe4490_Read_2(LED1VAL);
		printf("%d,0,0,0\r\n",IRtemp2);
		   ReadLED++;  
	     }
	     if(ReadLED==1){
	        afe4490_Write_2(CONTROL0,0x000001);
		REDtemp2=afe4490_Read_2(LED2VAL);
//		printf("AFE4490 RED:%d\r\n",REDtemp2);
	       ReadLED=0;
	     }
		}
		
//	   if(ADC_RDY==0){
//		   printf("\r\n");
//		   printf("ADC_RDY=0\r\n");
//		   printf("\r\n");
//	   }
	} 		 
}


//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static u8 led0sta=1,led1sta=1;
	
	 //消抖，此处为了方便使用了延时函数，实际代码中禁止在中断服务函数中调用任何delay之类的延时函数！！！
    delay_ms(20);     
    switch(GPIO_Pin)
    {
        case GPIO_PIN_0:
            if(WK_UP==1)	//控制LED0,LED1互斥点亮
            {
                led1sta=!led1sta;
                led0sta=!led1sta;
                LED1(led1sta);
                LED0(led0sta);
            }
            break;
        case GPIO_PIN_2:
            if(KEY1==0) 	//控制LED1翻转	
            {
                led1sta=!led1sta;
                LED1(led1sta);	
            };
            break;
        case GPIO_PIN_11:
            if(ReadLED==0){
	        afe4490_Write_2(CONTROL0,0x000001);
		IRtemp2=afe4490_Read_2(LED1VAL);
		printf("AFE4490 IR:%d \r\n",IRtemp2);
		   ReadLED++;  
	     }
	     if(ReadLED==1){
	        afe4490_Write_2(CONTROL0,0x000001);
		REDtemp2=afe4490_Read_2(LED2VAL);
		printf("AFE4490 RED:%d\r\n",REDtemp2);
	       ReadLED=0;
	     }
            break;
    }
}

//中断服务函数
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);   //调用中断处理公用函数
}

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);   //调用中断处理公用函数
}

void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);   //调用中断处理公用函数
}

