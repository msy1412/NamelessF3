#ifndef _NAMELESSCOTRUN_CTRL_
#define _NAMELESSCOTRUN_CTRL_
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

//ע�������п�����������ģʽ������ֱ�߶�λ��+��ֱ�ٶ�+��ֱ���ٶ�
#define YAW_Pos_Control_Accel_Disable  0
#define YAW_VEL_FEEDFORWARD  1//��ֱ�ٶ�ǰ��������

#define Optical_Enable  0//����������ͣ��־

#define ADRC_MODE  0
#define PID_MODE   1
#define TEST_MODE  2
//#define GYRO_CONTROL_MODE  TEST_MODE
#define GYRO_CONTROL_MODE  PID_MODE
//#define GYRO_CONTROL_MODE  ADRC_MODE
void Total_Control(void);
void Control_Output(void);
void Landon_Earth_Check(void);//�Լ촥�ؽ��뵡��ģʽ

extern float LPButter_Vel_Error(float curr_input);
extern uint8_t Controler_High_Mode;
extern uint16_t High_Hold_Throttle;
extern uint8_t Landon_Earth_Flag;
extern uint16 Throttle;


extern float Active_Para1,Active_Para2;
#endif

