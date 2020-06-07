#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2018/6/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
//�ⲿ�жϳ�ʼ��
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();               //����GPIOAʱ��
     __HAL_RCC_GPIOH_CLK_ENABLE();               //����GPIOHʱ��
    __HAL_RCC_GPIOE_CLK_ENABLE();               //����GPIOHʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_0;                //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //�����ش���
    GPIO_Initure.Pull=GPIO_PULLDOWN;			//����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    
    GPIO_Initure.Pin=GPIO_PIN_2;     //PH2,3  �½��ش���������
	GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;     //�½��ش���
	GPIO_Initure.Mode=GPIO_MODE_AF_PP; 		//����
    GPIO_Initure.Pull=GPIO_PULLUP;				//����

    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
	
	 GPIO_Initure.Pin=GPIO_PIN_11;                //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //�����ش���
    GPIO_Initure.Pull=GPIO_PULLDOWN;			//����
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
	
    
    //�ж���0
    HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //ʹ���ж���0
    
    //�ж���2
    HAL_NVIC_SetPriority(EXTI2_IRQn,2,1);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ1
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);             //ʹ���ж���2
    
    //�ж���3
    HAL_NVIC_SetPriority(EXTI3_IRQn,2,2);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);             //ʹ���ж���2
}


////�жϷ�����
//void EXTI0_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);   //�����жϴ����ú���
//}

//void EXTI2_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);   //�����жϴ����ú���
//}

//void EXTI3_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);   //�����жϴ����ú���
//}

////�жϷ����������Ҫ��������
////��HAL�������е��ⲿ�жϷ�����������ô˺���
////GPIO_Pin:�ж����ź�
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//    static u8 led0sta=1,led1sta=1;
//	
//	 //�������˴�Ϊ�˷���ʹ������ʱ������ʵ�ʴ����н�ֹ���жϷ������е����κ�delay֮�����ʱ����������
//    delay_ms(20);     
//    switch(GPIO_Pin)
//    {
//        case GPIO_PIN_0:
//            if(WK_UP==1)	//����LED0,LED1�������
//            {
//                led1sta=!led1sta;
//                led0sta=!led1sta;
//                LED1(led1sta);
//                LED0(led0sta);
//            }
//            break;
//        case GPIO_PIN_2:
//            if(KEY1==0) 	//����LED1��ת	
//            {
//                led1sta=!led1sta;
//                LED1(led1sta);	
//            };
//            break;
//        case GPIO_PIN_14:
//            if(KEY0==0)  	//ͬʱ����LED0,LED1��ת 
//            {
//				led0sta=!led0sta;
//                LED0(led0sta);
//            }
//            break;
//    }
//}











