#include "Headfile.h"
#include "PWM.h"
#define  MAX_PWM 2500  //400hz    周期2.5ms
//#define  MAX_PWM 20000  //50hz  周期20ms
//#define TM TIM4   //还要修改RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

/***************************************************
函数名: void PWM_GPIO_Init(void)
说明:	PWM输出IO初始化
入口:	无
出口:	无
备注:	上电初始化，运行一次
****************************************************/
void PWM_GPIO_Init(void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);

	//设置PA11、PA12、PB8、PB9 为推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用推挽输出
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输出速率
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        //GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
        
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;;
        GPIO_Init(GPIOB,&GPIO_InitStructure);
        //不同片区要使用不同的AF复用口
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_10);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_10);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_2);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_2);
}


void PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//定时器3作为PWM输出
        
        uint16_t prescalerValue = 0, ccr1_PWMVal = 0;
	PWM_GPIO_Init();
	prescalerValue = (u16) (SystemCoreClock / 1000000)-1;//10us
	
        
        //-----TIM4定时配置-----//
        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = MAX_PWM;		//40000/2M=20ms-->50Hz，从0开始计数,这个值被写入到Auto-Reload Register中
	TIM_TimeBaseStructure.TIM_Prescaler =0;	       //暂时不分频
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;	       //时钟分割
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	//重复比较次数更新事件
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
        TIM_PrescalerConfig(TIM4, prescalerValue, TIM_PSCReloadMode_Immediate);//预分频,现在计时器频率为20MHz
        
        
	//-----PWM配置-----//
        // Initialise the timer channels
        TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 		//选择定时器模式:TIM脉冲宽度调制模式1-->向上计数为有效电平
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
        TIM_OCInitStructure.TIM_Pulse = ccr1_PWMVal;//duty cycle // preset pulse width 0..pwm_periode
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 	//输出极性:TIM输出比较极性高

////These settings must be applied on the timer 1.
//TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
//TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
//TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;
     
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  					//初始化外设TIM3 OC1-->Motor1
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  					//初始化外设TIM3 OC2-->Motor2
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  					//初始化外设TIM3 OC3-->Motor3
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  					//初始化外设TIM3 OC4-->Motor4

        TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable );//使能TIM3在CCR2上的预装载寄存器,在更新事件时，值才被写入到CCR      
        TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable );
        TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable );
        TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable );
          
        TIM_ARRPreloadConfig(TIM4, ENABLE);//自动重载寄存器使能，下一个更新事件自动更新影子寄存器
        TIM_CtrlPWMOutputs(TIM4,ENABLE);
        TIM_Cmd(TIM4, ENABLE);
        
        PWM_Set(1000,1000,1000,1000);
}

/***********************************************************************************
函数名：void PWM_Set(const u16 pwm1, const u16 pwm2, const u16 pwm3, const u16 pwm4)
说明：PWM输出设置
入口：四个通道的值
出口：无
备注：满占空为2.5ms（20ms）
************************************************************************************/
void PWM_Set(const uint16_t pwm1, const uint16_t pwm2, const uint16_t pwm3, const uint16_t pwm4)
{
	TIM_SetCompare1(TIM4, pwm1);
	TIM_SetCompare2(TIM4, pwm2);
	TIM_SetCompare3(TIM4, pwm3);
	TIM_SetCompare4(TIM4, pwm4);
}





