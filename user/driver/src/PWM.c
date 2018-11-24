#include "Headfile.h"
#include "PWM.h"
/*----------------------------------------------------------------------------------------------------------------------/
        *               ������ֻ��������ѧϰʹ�ã���Ȩ����Ȩ���������ƴ��Ŷӣ�
        *               �����ƴ��Ŷӽ��ɿس���Դ���ṩ�������ߣ�
        *               ������ҪΪ�����ƴ��Ŷ��ṩ������
        *               δ��������ɣ����ý�Դ�����ṩ������
        *               ���ý�Դ����ŵ����Ϲ�����������أ�
        *               �������Դ�����Ĳ�����緢��������Ϊ��
        *               �����ƴ��Ŷӽ���֮�Է��ɽ��������
-----------------------------------------------------------------------------------------------------------------------/
        *               ������Ϣ���ܶ���ֹ��ǰ�����������˳���������
        *               ��Դ���ף���ѧ����ϧ��ף������Ϯ�����׳ɹ�������
-----------------------------------------------------------------------------------------------------------------------/
	*		�����ƴ���Դ�ɿ� V1.1	�人�Ƽ���ѧ  By.YuYi
	*		CSDN����: http://blog.csdn.net/u011992534
	*               �ſ�ID��NamelessCotrun����С��
	*               �����ƴ���Դ�ɿ�QQȺ��540707961
        *               https://shop348646912.taobao.com/?spm=2013.1.1000126.2.5ce78a88ht1sO2
        *               �ٶ�����:�����ƴ���Դ�ɿ�
        *               �޸�����:2017/10/30
        *               �汾��V1.1
        *               ��Ȩ���У�����ؾ���
        *               Copyright(C) �人�Ƽ���ѧ�����ƴ��Ŷ� 2017-2019
        *               All rights reserved
----------------------------------------------------------------------------------------------------------------------*/

#define  MAX_PWM 2500  //400hz    ����2.5ms
//#define  MAX_PWM 20000  //50hz  ����20ms

/***************************************************
������: void PWM_GPIO_Init(void)
˵��:	PWM���IO��ʼ��
���:	��
����:	��
��ע:	�ϵ��ʼ��������һ��
****************************************************/
void PWM_GPIO_Init(void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʼTIM4 ��ʱ�� ��GPIOBʱ�� ��AFIOʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//��ʱ��3��ΪPWM���
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO ,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);

	//����PA5��PA6��PB0��PB1 Ϊ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�������
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;//
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}


void PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

        uint16_t prescalerValue = 0, ccr1_PWMVal = 0;
	PWM_GPIO_Init();
	prescalerValue = (u16) (SystemCoreClock / 1000000)-1;//10us
	//-----TIM3��ʱ����-----//
	TIM_TimeBaseStructure.TIM_Period = MAX_PWM;		//40000/2M=20ms-->50Hz����0��ʼ����,���ֵ��д�뵽Auto-Reload Register��
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	       //��ʱ����Ƶ
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;	       //ʱ�ӷָ�
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	//�ظ��Ƚϴ��������¼�
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_PrescalerConfig(TIM3, prescalerValue, TIM_PSCReloadMode_Immediate);//Ԥ��Ƶ,���ڼ�ʱ��Ƶ��Ϊ20MHz

		//-----PWM����-----//
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 		//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1-->���ϼ���Ϊ��Ч��ƽ
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = ccr1_PWMVal;					//duty cycle
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 	//�������:TIM����Ƚϼ��Ը�

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  					//��ʼ������TIM3 OC1-->Motor1
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  					//��ʼ������TIM3 OC2-->Motor2
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  					//��ʼ������TIM3 OC3-->Motor3
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  					//��ʼ������TIM3 OC4-->Motor4

   	TIM_ARRPreloadConfig(TIM3, ENABLE);//�Զ����ؼĴ���ʹ�ܣ���һ�������¼��Զ�����Ӱ�ӼĴ���
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);//ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���,�ڸ����¼�ʱ��ֵ�ű�д�뵽CCR
        TIM_Cmd(TIM3, ENABLE);
        PWM_Set(1000,1000,1000,1000);
}

/***********************************************************************************
��������void PWM_Set(const u16 pwm1, const u16 pwm2, const u16 pwm3, const u16 pwm4)
˵����PWM�������
��ڣ��ĸ�ͨ����ֵ
���ڣ���
��ע����ռ��Ϊ2.5ms��20ms��
************************************************************************************/
void PWM_Set(const uint16_t pwm1, const uint16_t pwm2, const uint16_t pwm3, const uint16_t pwm4)
{
	TIM_SetCompare1(TIM3, pwm1);
	TIM_SetCompare2(TIM3, pwm2);
	TIM_SetCompare3(TIM3, pwm3);
	TIM_SetCompare4(TIM3, pwm4);
}





