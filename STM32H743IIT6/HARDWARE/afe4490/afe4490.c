#include "afe4490.h"
#include "delay.h"

//SPI_HandleTypeDef SPI2_Handler;  //SPI2���


//оƬ2����
void AFE4490_SPI2_Init(void)
{
    __HAL_SPI_DISABLE(&SPI2_Handler);               //�ȹر�SPI2
    SPI2_Handler.Init.CLKPolarity=SPI_POLARITY_LOW; //����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
    SPI2_Handler.Init.CLKPhase=SPI_PHASE_1EDGE;     //����ͬ��ʱ�ӵĵ�1�������أ��������½������ݱ�����
    HAL_SPI_Init(&SPI2_Handler);
    __HAL_SPI_ENABLE(&SPI2_Handler);                //ʹ��SPI2
}

//ʹ��оƬ2
void AFE4490_Init_2(void){
	
	GPIO_InitTypeDef GPIO_Initure;
	
	 __HAL_RCC_GPIOB_CLK_ENABLE();                   //ʹ��GPIOBʱ��
	 __HAL_RCC_GPIOC_CLK_ENABLE();                   //ʹ��GPIOBʱ��
	
	//����AFE4490ʹ�ܽ�
	GPIO_Initure.Pin=GPIO_PIN_12;           //PB12����ΪAFE4490ʹ�ܽ�
       GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
       GPIO_Initure.Pull=GPIO_PULLUP;          //����
       GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
       HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //��ʼ��
	
	//������ͨ��
//	GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_Initure.Pin=GPIO_PIN_14;           //PB12����ΪAFE4490ʹ�ܽ�
       GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
       GPIO_Initure.Pull=GPIO_PULLUP;          //����
       GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
       HAL_GPIO_Init(GPIOE,&GPIO_Initure);     //��ʼ��
	
	
       SPI2_Init(); 
       AFE4490_SPI2_Init();
       AFE4490_STE_2(1);  //��SPI�رգ�������ʱ����ʹ��	
	AFE4490_AFEPDN_2(0);
}

u8 afe4490_Write_2 (u8 address, u32 data){
      
	u8 status;
	AFE4490_STE_2(0); //ʹ��AFE4490 SPI
	status=SPI2_ReadWriteByte(address);
//	printf("Write address: %d, data: %u, status: %d\r\n",address,data,status);
	status=SPI2_ReadWriteByte((data >> 16) & 0xFF); //д�߰�λ
//	printf("Write high: %d\r\n",status);
	status=SPI2_ReadWriteByte((data >> 8) & 0xFF);  //д�а�λ
//	printf("Write middle: %d\r\n",status);
	status=SPI2_ReadWriteByte(data  & 0xFF);        //д�ڰ�λ
//	printf("Write low: %d\r\n",status);
//	printf("\r\n");
	AFE4490_STE_2(1);
}


u32 afe4490_Read_2 (u8 address){

	u32 data =0;
	AFE4490_STE_2(0); //ʹ��AFE4490 SPI
	SPI2_ReadWriteByte(address);
	data += (u32)SPI2_ReadWriteByte((0)<<16);
	data += (u32)SPI2_ReadWriteByte((0)<<8);
	data += (u32)SPI2_ReadWriteByte(0);
	AFE4490_STE_2(1);
	return data;
}


void AFE4490_Reg_Init_2(void){
	
	
     afe4490_Write_2(CONTROL0,0x000000);

     afe4490_Write_2(CONTROL0,0x000008);  

     afe4490_Write_2(TIAGAIN,0x000000); // CF = 5pF, RF = 500kR
     afe4490_Write_2(TIA_AMB_GAIN,0x000001); 
     
     afe4490_Write_2(LEDCNTRL,0x001414);    
     afe4490_Write_2(CONTROL2,0x000000); // LED_RANGE=100mA, LED=50mA 
     afe4490_Write_2(CONTROL1,0x010707); // Timers ON, average 3 samples  
    
     afe4490_Write_2(PRPCOUNT, 0X001F3F);

    afe4490_Write_2(LED2STC, 0X001770);
    afe4490_Write_2(LED2ENDC,0X001F3E); 
    afe4490_Write_2(LED2LEDSTC,0X001770);
    afe4490_Write_2(LED2LEDENDC,0X001F3F);
    afe4490_Write_2(ALED2STC, 0X000000); 
    afe4490_Write_2(ALED2ENDC, 0X0007CE);
    afe4490_Write_2(LED2CONVST,0X000002); 
    afe4490_Write_2(LED2CONVEND, 0X0007CF);
    afe4490_Write_2(ALED2CONVST, 0X0007D2);
    afe4490_Write_2(ALED2CONVEND,0X000F9F);

    afe4490_Write_2(LED1STC, 0X0007D0); 
    afe4490_Write_2(LED1ENDC, 0X000F9E);
    afe4490_Write_2(LED1LEDSTC, 0X0007D0);
    afe4490_Write_2(LED1LEDENDC, 0X000F9F);
    afe4490_Write_2(ALED1STC, 0X000FA0); 
    afe4490_Write_2(ALED1ENDC, 0X00176E);
    afe4490_Write_2(LED1CONVST, 0X000FA2); 
    afe4490_Write_2(LED1CONVEND, 0X00176F);
    afe4490_Write_2(ALED1CONVST, 0X001772);
    afe4490_Write_2(ALED1CONVEND, 0X001F3F); 

    afe4490_Write_2(ADCRSTCNT0, 0X000000); 
    afe4490_Write_2(ADCRSTENDCT0,0X000000); 
    afe4490_Write_2(ADCRSTCNT1, 0X0007D0); 
    afe4490_Write_2(ADCRSTENDCT1, 0X0007D0);
    afe4490_Write_2(ADCRSTCNT2, 0X000FA0); 
    afe4490_Write_2(ADCRSTENDCT2, 0X000FA0); 
    afe4490_Write_2(ADCRSTCNT3, 0X001770); 
    afe4490_Write_2(ADCRSTENDCT3, 0X001770);
  
    delay_ms(100);   
}

//void AFE4490_Reg_Init_2(void){
//	
//    afe4490_Write_2(CONTROL1, 0x000101);
//    afe4490_Write_2(CONTROL2, 0x000000);  
//    afe4490_Write_2(PRPCOUNT, 0X001F3F);

//    afe4490_Write_2(LED2STC, 0X001770); //timer control
//    afe4490_Write_2(LED2ENDC,0X001F3E); //timer control
//    afe4490_Write_2(LED2LEDSTC,0X001770); //timer control
//    afe4490_Write_2(LED2LEDENDC,0X001F3F); //timer control
//    afe4490_Write_2(ALED2STC, 0X000000); //timer control
//    afe4490_Write_2(ALED2ENDC, 0X0007CE); //timer control
//    afe4490_Write_2(LED2CONVST,0X000002); //timer control
//    afe4490_Write_2(LED2CONVEND, 0X0007CF); //timer control
//    afe4490_Write_2(ALED2CONVST, 0X0007D2); //timer control
//    afe4490_Write_2(ALED2CONVEND,0X000F9F); //timer control

//    afe4490_Write_2(LED1STC, 0X0007D0); //timer control
//    afe4490_Write_2(LED1ENDC, 0X000F9E); //timer control
//    afe4490_Write_2(LED1LEDSTC, 0X0007D0); //timer control
//    afe4490_Write_2(LED1LEDENDC, 0X000F9F); //timer control
//    afe4490_Write_2(ALED1STC, 0X000FA0); //timer control
//    afe4490_Write_2(ALED1ENDC, 0X00176E); //timer control
//    afe4490_Write_2(LED1CONVST, 0X000FA2); //timer control
//    afe4490_Write_2(LED1CONVEND, 0X00176F); //timer control
//    afe4490_Write_2(ALED1CONVST, 0X001772); //timer control
//    afe4490_Write_2(ALED1CONVEND, 0X001F3F); //timer control

//    afe4490_Write_2(ADCRSTCNT0, 0X000000); //timer control
//    afe4490_Write_2(ADCRSTENDCT0,0X000000); //timer control
//    afe4490_Write_2(ADCRSTCNT1, 0X0007D0); //timer control
//    afe4490_Write_2(ADCRSTENDCT1, 0X0007D0); //timer control
//    afe4490_Write_2(ADCRSTCNT2, 0X000FA0); //timer control
//    afe4490_Write_2(ADCRSTENDCT2, 0X000FA0); //timer control
//    afe4490_Write_2(ADCRSTCNT3, 0X001770); //timer control
//    afe4490_Write_2(ADCRSTENDCT3, 0X001770);
//     
//    delay_ms(1000);
//    printf("AFE4490 Initialization Done\r\n"); 
//}







