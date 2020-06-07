#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2018/6/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
//外部中断初始化
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();               //开启GPIOA时钟
     __HAL_RCC_GPIOH_CLK_ENABLE();               //开启GPIOH时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();               //开启GPIOH时钟
    
    GPIO_Initure.Pin=GPIO_PIN_0;                //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //上升沿触发
    GPIO_Initure.Pull=GPIO_PULLDOWN;			//下拉
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    
    GPIO_Initure.Pin=GPIO_PIN_2;     //PH2,3  下降沿触发，上拉
	GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;     //下降沿触发
	GPIO_Initure.Mode=GPIO_MODE_AF_PP; 		//复用
    GPIO_Initure.Pull=GPIO_PULLUP;				//上拉

    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
	
	 GPIO_Initure.Pin=GPIO_PIN_11;                //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //上升沿触发
    GPIO_Initure.Pull=GPIO_PULLDOWN;			//下拉
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
	
    
    //中断线0
    HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);       //抢占优先级为2，子优先级为0
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //使能中断线0
    
    //中断线2
    HAL_NVIC_SetPriority(EXTI2_IRQn,2,1);       //抢占优先级为2，子优先级为1
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);             //使能中断线2
    
    //中断线3
    HAL_NVIC_SetPriority(EXTI3_IRQn,2,2);       //抢占优先级为2，子优先级为2
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);             //使能中断线2
}


////中断服务函数
//void EXTI0_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);   //调用中断处理公用函数
//}

//void EXTI2_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);   //调用中断处理公用函数
//}

//void EXTI3_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);   //调用中断处理公用函数
//}

////中断服务程序中需要做的事情
////在HAL库中所有的外部中断服务函数都会调用此函数
////GPIO_Pin:中断引脚号
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//    static u8 led0sta=1,led1sta=1;
//	
//	 //消抖，此处为了方便使用了延时函数，实际代码中禁止在中断服务函数中调用任何delay之类的延时函数！！！
//    delay_ms(20);     
//    switch(GPIO_Pin)
//    {
//        case GPIO_PIN_0:
//            if(WK_UP==1)	//控制LED0,LED1互斥点亮
//            {
//                led1sta=!led1sta;
//                led0sta=!led1sta;
//                LED1(led1sta);
//                LED0(led0sta);
//            }
//            break;
//        case GPIO_PIN_2:
//            if(KEY1==0) 	//控制LED1翻转	
//            {
//                led1sta=!led1sta;
//                LED1(led1sta);	
//            };
//            break;
//        case GPIO_PIN_14:
//            if(KEY0==0)  	//同时控制LED0,LED1翻转 
//            {
//				led0sta=!led0sta;
//                LED0(led0sta);
//            }
//            break;
//    }
//}











