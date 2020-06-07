
#include "sys.h"
#include "spi.h"

//����AFE4490�Ĵ�����ַ
#define CONTROL0    0x00  //CONTROL0�����ƼĴ���0����ַ= 00h����λֵ= 0000h����ֻд��CONTROL0����AFE����ͼ�����ʱ����λ�����ʹ���Լ�SPI��ȡ����
#define LED2STC     0x01  //LED2���������Ĵ�������ַ= 01h����λֵ= 0000h��������LED2�źŲ����Ŀ�ʼʱ��ֵ
#define LED2ENDC    0x02  //LED2���������Ĵ�������ַ= 02h����λֵ= 0000h��������LED2�źŲ����Ľ���ʱ��ֵ
#define LED2LEDSTC    0x03  //LED2 LED���������Ĵ�������ַ= 03h����λֵ= 0000h��������LED2�źſ���ʱ�Ŀ�ʼʱ��ֵ��
#define LED2LEDENDC   0x04 //LED2 LED���������Ĵ�������ַ= 04h����λֵ= 0000h��������LED2�źŹر�ʱ�Ľ���ʱ��ֵ��
#define ALED2STC    0x05   //LED2�����������������Ĵ�������ַ= 05h����λֵ= 0000h�������û���LED2�źŲ����Ŀ�ʼʱ��ֵ��
#define ALED2ENDC   0x06   //LED2�����������������Ĵ�������ַ= 06h����λֵ= 0000h�������û���LED2�źŲ����Ľ���ʱ��ֵ��
#define LED1STC     0x07   //ED1�������������Ĵ�������ַ= 07h����λֵ= 0000h��������LED1�źŲ����Ŀ�ʼʱ��ֵ��
#define LED1ENDC    0x08   //LED1�������������Ĵ�������ַ= 08h����λֵ= 0000h��������LED1�źŲ����Ľ���ʱ��ֵ��
#define LED1LEDSTC    0x09 //LED1 LED���������Ĵ�������ַ= 09h����λֵ= 0000h��������LED1�źſ���ʱ����ʼʱ��ֵ��
#define LED1LEDENDC   0x0a //LED1 LED���������Ĵ�������ַ= 0Ah����λֵ= 0000h��������LED1�źŹر�ʱ�Ľ���ʱ��ֵ
#define ALED1STC    0x0b   //��������LED1���������Ĵ�������ַ= 0Bh����λֵ= 0000h�������û���LED1�źŲ����Ŀ�ʼʱ��ֵ��
#define ALED1ENDC   0x0c   //��������LED1���������Ĵ�������ַ= 0Ch����λֵ= 0000h�������û���LED1�źŲ����Ľ���ʱ��ֵ��
#define LED2CONVST    0x0d //LED2ת����ʼ�����Ĵ���������LED2ת���Ŀ�ʼʱ��ֵ��
#define LED2CONVEND   0x0e //LED2ת�����������Ĵ�������ַ= 0Eh����λֵ= 0000h��������LED2ת���Ľ���ʱ��ֵ��
#define ALED2CONVST   0x0f //LED2����ת����ʼ�����Ĵ�������ַ= 0Fh����λֵ= 0000h�������û���LED2ת���Ŀ�ʼʱ��ֵ��
#define ALED2CONVEND  0x10 //LED2����ת�����������Ĵ�������ַ= 10h����λֵ= 0000h�������û���LED2ת���Ľ���ʱ��ֵ��
#define LED1CONVST    0x11 //LED1ת����ʼ�����Ĵ���������LED1ת���Ŀ�ʼʱ��ֵ��
#define LED1CONVEND   0x12 //LED1ת�����������Ĵ�������ַ= 12h����λֵ= 0000h��������LED1ת���Ľ���ʱ��ֵ��
#define ALED1CONVST   0x13 //LED1����ת����ʼ�����Ĵ�������ַ= 13h����λֵ= 0000h�������û���LED1ת���Ŀ�ʼʱ��ֵ��
#define ALED1CONVEND  0x14 //LED1����ת�����������Ĵ�������ַ= 14h����λֵ= 0000h�������û���LED1ת���Ľ���ʱ��ֵ��
#define ADCRSTCNT0    0x15 //ADC0��λ��ʼ�����Ĵ���������ADC0��λת���źŵ���ʼλ�á�
#define ADCRSTENDCT0  0x16 //ADC0��λ���������Ĵ�������ַ= 16h����λֵ= 0000h��������ADC0��λת���źŵĽ���λ�á�
#define ADCRSTCNT1    0x17 //ADC1��λ��ʼ�����Ĵ�������ַ= 17h����λֵ= 0000h��������ADC1��λת���źŵĿ�ʼλ�á�
#define ADCRSTENDCT1  0x18 //ADC1��λ���������Ĵ�������ַ= 18h����λֵ= 0000h��������ADC1��λת���źŵĽ���λ�á�
#define ADCRSTCNT2    0x19 //ADC2��λ��ʼ�����Ĵ�������ַ= 19h����λֵ= 0000h��������ADC2��λת���źŵ���ʼλ�á�
#define ADCRSTENDCT2  0x1a //ADC2��λ���������Ĵ�������ַ= 1Ah����λֵ= 0000h��������ADC2��λת���źŵĽ���λ��
#define ADCRSTCNT3    0x1b //ADC3��λ��ʼ�����Ĵ�������ַ= 1Bh����λֵ= 0000h��������ADC3��λת���źŵ���ʼλ�á�
#define ADCRSTENDCT3  0x1c //ADC3��λ���������Ĵ�������ַ= 1Ch����λֵ= 0000h��������ADC3��λת���źŵĽ���λ�á�
#define PRPCOUNT    0x1d   //�����ظ����ڼ����Ĵ�������ַ= 1Dh����λֵ= 0000h���������豸�����ظ����ڼ�����
#define CONTROL1    0x1e   //���ƼĴ���1����ַ= 1Eh����λֵ= 0000h��������ʱ�Ӿ������ţ���ʱ����ƽ��������
#define SPARE1      0x1f   //SPARE1�Ĵ���������ʹ�ã���ַ= 1Fh����λֵ= 0000h�������üĴ���������������ʹ�á�
#define TIAGAIN     0x20    //����Ŵ����������üĴ�������ַ= 20h����λֵ= 0000h����������������Ŵ���������ģʽ�Լ���������͵���ֵ��
#define TIA_AMB_GAIN  0x21  //����Ŵ����ͻ�������������Ĵ�������ַ= 21h����λֵ= 0000h�������û����������Ŵ��������棬�����������˲���ת��Ƶ�ʡ�
#define LEDCNTRL    0x22  //LED���ƼĴ�������ַ= 22h����λֵ= 0000h��������LED������Χ�Լ�LED1��LED2��������
#define CONTROL2    0x23  //���ƼĴ���2����ַ= 23h����λֵ= 0000h��������LED�������������Լ�AFE���������ͽ������ĵ�Դģʽ
#define SPARE2      0x24  //SPARE2�Ĵ����Ա�����ʹ�ã���ַ= 24h����λֵ= 0000h��
#define SPARE3      0x25  //SPARE3�Ĵ����Ա�����ʹ�ã���ַ= 25h����λֵ= 0000h��
#define SPARE4      0x26  //SPARE4�Ĵ����Ա�����ʹ�ã���ַ= 26h����λֵ= 0000h��
#define RESERVED1   0x27  //RESERVED1�Ĵ�����������ʹ�ã���ַ= 27h����λֵ= XXXXh��
#define RESERVED2   0x28  //RESERVED2�Ĵ�����������ʹ�ã���ַ= 28h����λֵ= XXXXh��
#define ALARM     0x29    //�����Ĵ�������ַ= 29h����λֵ= 0000h��
#define LED2VAL     0x2a  //LED2����ֵ��������ADCת��������LED2����������ֵ��ÿ�θ��¸üĴ���������ʱ��ADC_RDY�źŶ����ߡ�����һ��������AFEת��֮ǰ��������������������üĴ���
#define ALED2VAL    0x2b  //LED2��������ֵ��������ADCת��������LED2��������������ֵ��ÿ�θ��¸üĴ���������ʱ��ADC_RDY�źŶ����ߡ�����һ��������AFEת��֮ǰ��������������������üĴ�����
#define LED1VAL     0x2c  //LED1����ֵ
#define ALED1VAL    0x2d  //LED1��������ֵ
#define LED2ABSVAL    0x2e //�� LED2 �C LED2����������ֵ
#define LED1ABSVAL    0x2f //�� LED1 �C LED1����������ֵ
#define DIAG      0x30     //��ϱ�־�Ĵ�������ַ= 30h����λֵ= 0000h��

//����AFE4490�������ţ�оƬ2
#define AFE4490_STE_2(n)(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET))
#define AFE4490_AFEPDN_2(n)(n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_RESET))

//����AFE4490оƬ1��������
#define AFE4490_STE_1(n)(n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET))
#define AFE4490_AFEPDN_1(n)(n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET))


//����оƬ2��������
void AFE4490_SPI2_Init(void);
void AFE4490_Init_2(void);
u8 afe4490_Write_2(u8 address, u32 data);
u32 afe4490_Read_2 (u8 address);
void AFE4490_Reg_Init_2(void);
	

//����оƬ1��������
void AFE4490_SPI1_Init(void);
void AFE4490_Init_1(void);
u8 afe4490_Write_1 (u8 address, u32 data);
u32 afe4490_Read_1 (u8 address);
void AFE4490_Reg_Init_1(void);

	