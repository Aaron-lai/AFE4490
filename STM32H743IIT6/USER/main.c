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


//AFE4490����
u32 IRtemp1;
u32 REDtemp1;
u32 IRtemp2;
u32 REDtemp2;

int main(void)
{
 	u32 total,free;
	u8 t=0;	
	u8 res=0;	
	
	Cache_Enable();                			//��L1-Cache
	MPU_Memory_Protection();        		//������ش洢����
	HAL_Init();				        		//��ʼ��HAL��
	Stm32_Clock_Init(160,5,2,4);  		    //����ʱ��,400Mhz 
	delay_init(400);						//��ʱ��ʼ��
	uart_init(115200);						//���ڳ�ʼ��
	usmart_dev.init(200); 		    		//��ʼ��USMART	
	LED_Init();								//��ʼ��LED
	KEY_Init();								//��ʼ������
	SDRAM_Init();                   		//��ʼ��SDRAM
	LCD_Init();								//��ʼ��LCD
       W25QXX_Init();				   		 	//��ʼ��W25Q256
 	my_mem_init(SRAMIN);		    		//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		    		//��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMDTCM);		    		//��ʼ��CCM�ڴ�� 
	
	//AFE4490����
	AFE4490_Init_2();
 	AFE4490_Reg_Init_2();
   	      
 				    
	while(1)
	{
		afe4490_Write_2(CONTROL0,0x000001);
		IRtemp2=afe4490_Read_2(LED1VAL);
		printf("AFE4490 IR:%d \r\n",IRtemp2);
		afe4490_Write_2(CONTROL0,0x000001);
		REDtemp2=afe4490_Read_2(LED2VAL);
		printf("AFE4490 RED:%d \r\n",REDtemp2);
		
		
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
	} 		 
}

