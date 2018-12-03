#include "Headfile.h"
#include "PWM.h"
#define  MAX_PWM 2500  //400hz    ����2.5ms
//#define  MAX_PWM 20000  //50hz  ����20ms
//#define TM TIM4   //��Ҫ�޸�RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

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
        
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);

	//����PA11��PA12��PB8��PB9 Ϊ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�����������
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�������
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        //GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
        
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;;
        GPIO_Init(GPIOB,&GPIO_InitStructure);
        //��ͬƬ��Ҫʹ�ò�ͬ��AF���ÿ�
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_10);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_10);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_2);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_2);
}


void PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//��ʱ��3��ΪPWM���
        
        uint16_t prescalerValue = 0, ccr1_PWMVal = 0;
	PWM_GPIO_Init();
	prescalerValue = (u16) (SystemCoreClock / 1000000)-1;//10us
	
        
        //-----TIM4��ʱ����-----//
        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = MAX_PWM;		//40000/2M=20ms-->50Hz����0��ʼ����,���ֵ��д�뵽Auto-Reload Register��
	TIM_TimeBaseStructure.TIM_Prescaler =0;	       //��ʱ����Ƶ
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;	       //ʱ�ӷָ�
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	//�ظ��Ƚϴ��������¼�
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
        TIM_PrescalerConfig(TIM4, prescalerValue, TIM_PSCReloadMode_Immediate);//Ԥ��Ƶ,���ڼ�ʱ��Ƶ��Ϊ20MHz
        
        
	//-----PWM����-----//
        // Initialise the timer channels
        TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 		//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1-->���ϼ���Ϊ��Ч��ƽ
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
        TIM_OCInitStructure.TIM_Pulse = ccr1_PWMVal;//duty cycle // preset pulse width 0..pwm_periode
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 	//�������:TIM����Ƚϼ��Ը�

////These settings must be applied on the timer 1.
//TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
//TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
//TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;
     
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  					//��ʼ������TIM3 OC1-->Motor1
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  					//��ʼ������TIM3 OC2-->Motor2
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  					//��ʼ������TIM3 OC3-->Motor3
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  					//��ʼ������TIM3 OC4-->Motor4

        TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable );//ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���,�ڸ����¼�ʱ��ֵ�ű�д�뵽CCR      
        TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable );
        TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable );
        TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable );
          
        TIM_ARRPreloadConfig(TIM4, ENABLE);//�Զ����ؼĴ���ʹ�ܣ���һ�������¼��Զ�����Ӱ�ӼĴ���
        TIM_CtrlPWMOutputs(TIM4,ENABLE);
        TIM_Cmd(TIM4, ENABLE);
        
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
	TIM_SetCompare1(TIM4, pwm1);
	TIM_SetCompare2(TIM4, pwm2);
	TIM_SetCompare3(TIM4, pwm3);
	TIM_SetCompare4(TIM4, pwm4);
}





