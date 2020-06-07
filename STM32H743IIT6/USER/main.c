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

//AFE4490����
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
//	LCD_Init();								//��ʼ��LCD
//       W25QXX_Init();				   		 	//��ʼ��W25Q256
 	my_mem_init(SRAMIN);		    		//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		    		//��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMDTCM);		    		//��ʼ��CCM�ڴ�� 
	EXTI_Init();
	
	//AFE4490����
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


//�жϷ����������Ҫ��������
//��HAL�������е��ⲿ�жϷ�����������ô˺���
//GPIO_Pin:�ж����ź�
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static u8 led0sta=1,led1sta=1;
	
	 //�������˴�Ϊ�˷���ʹ������ʱ������ʵ�ʴ����н�ֹ���жϷ������е����κ�delay֮�����ʱ����������
    delay_ms(20);     
    switch(GPIO_Pin)
    {
        case GPIO_PIN_0:
            if(WK_UP==1)	//����LED0,LED1�������
            {
                led1sta=!led1sta;
                led0sta=!led1sta;
                LED1(led1sta);
                LED0(led0sta);
            }
            break;
        case GPIO_PIN_2:
            if(KEY1==0) 	//����LED1��ת	
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

//�жϷ�����
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);   //�����жϴ����ú���
}

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);   //�����жϴ����ú���
}

void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);   //�����жϴ����ú���
}

