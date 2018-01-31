#include "Headfile.h"
#include "ADRC.h"
/*----------------------------------------------------------------------------------------------------------------------/
        *               ������ֻ��������ѧϰʹ�ã���Ȩ����Ȩ���������ƴ��Ŷӣ�
        *               �����ƴ��Ŷӽ��ɿس���Դ���ṩ�������ߣ�
        *               ������ҪΪ�����ƴ��Ŷ��ṩ������
        *               δ���������ɣ����ý�Դ�����ṩ������
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
Fhan_Data ADRC_Pitch_Controller;
Fhan_Data ADRC_Roll_Controller;
const float ADRC_Unit[3][15]=
{
/*TD����΢����   �Ľ�����TD,h0=N*h      ����״̬�۲���ESO           �Ŷ�����     ���������*/
/*  r     h      N                  beta_01   beta_02    beta_03     b0       beta_0  beta_1  beta_2   N1     C    alpha1  alpha2*/
 {300000 ,0.005 , 2,               100,      1000,      2000,     0.001,    0.002,   1.0,      0.0005,    5,    5,    0.8,   1.5,    50},
 {300000 ,0.005 , 2,               100,      1000,      2000,     0.001,    0.002,   1.0,      0.0005,    5,    5,    0.8,   1.5,    50},
 {50000  ,0.005 , 30,              100,      2000,      10000,    5    ,    0.002,   10,        0.001,    5,    5,    0.5,   1.05,   50},
};


float Constrain_Float(float amt, float low, float high){
  return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

int16_t Sign_ADRC(float Input)
{
    int16_t output=0;
    if(Input>1E-6) output=1;
    else if(Input<-1E-6) output=-1;
    else output=0;
    return output;
}

int16_t Fsg_ADRC(float x,float d)
{
  int16_t output=0;
  output=(Sign_ADRC(x+d)-Sign_ADRC(x-d))/2;
  return output;
}


void ADRC_Init(Fhan_Data *fhan_Input1,Fhan_Data *fhan_Input2)
{
  fhan_Input1->r=ADRC_Unit[0][0];
  fhan_Input1->h=ADRC_Unit[0][1];
  fhan_Input1->N0=(uint16)(ADRC_Unit[0][2]);
  fhan_Input1->beta_01=ADRC_Unit[0][3];
  fhan_Input1->beta_02=ADRC_Unit[0][4];
  fhan_Input1->beta_03=ADRC_Unit[0][5];
  fhan_Input1->b0=ADRC_Unit[0][6];
  fhan_Input1->beta_0=ADRC_Unit[0][7];
  fhan_Input1->beta_1=ADRC_Unit[0][8];
  fhan_Input1->beta_2=ADRC_Unit[0][9];
  fhan_Input1->N1=(uint16)(ADRC_Unit[0][10]);
  fhan_Input1->c=ADRC_Unit[0][11];

  fhan_Input1->alpha1=ADRC_Unit[0][12];
  fhan_Input1->alpha2=ADRC_Unit[0][13];
  fhan_Input1->zeta=ADRC_Unit[0][14];

  fhan_Input2->r=ADRC_Unit[1][0];
  fhan_Input2->h=ADRC_Unit[1][1];
  fhan_Input2->N0=(uint16)(ADRC_Unit[1][2]);
  fhan_Input2->beta_01=ADRC_Unit[1][3];
  fhan_Input2->beta_02=ADRC_Unit[1][4];
  fhan_Input2->beta_03=ADRC_Unit[1][5];
  fhan_Input2->b0=ADRC_Unit[1][6];
  fhan_Input2->beta_0=ADRC_Unit[1][7];
  fhan_Input2->beta_1=ADRC_Unit[1][8];
  fhan_Input2->beta_2=ADRC_Unit[1][9];
  fhan_Input2->N1=(uint16)(ADRC_Unit[1][10]);
  fhan_Input2->c=ADRC_Unit[1][11];

  fhan_Input2->alpha1=ADRC_Unit[1][12];
  fhan_Input2->alpha2=ADRC_Unit[1][13];
  fhan_Input2->zeta=ADRC_Unit[1][14];
}



//ADRC���ٸ���΢����TD���Ľ����㷨fhan
void Fhan_ADRC(Fhan_Data *fhan_Input,float expect_ADRC)//����ADRC���ȹ���
{
  float d=0,a0=0,y=0,a1=0,a2=0,a=0;
  float x1_delta=0;//ADRC״̬���������
  x1_delta=fhan_Input->x1-expect_ADRC;//��x1-v(k)���x1�õ���ɢ���¹�ʽ
  fhan_Input->h0=fhan_Input->N0*fhan_Input->h;//��h0���h��������ٸ���΢�����ٶȳ�������
  d=fhan_Input->r*fhan_Input->h0*fhan_Input->h0;//d=rh^2;
  a0=fhan_Input->h0*fhan_Input->x2;//a0=h*x2
  y=x1_delta+a0;//y=x1+a0
  a1=sqrt(d*(d+8*ABS(y)));//a1=sqrt(d*(d+8*ABS(y))])
  a2=a0+Sign_ADRC(y)*(a1-d)/2;//a2=a0+sign(y)*(a1-d)/2;
  a=(a0+y)*Fsg_ADRC(y,d)+a2*(1-Fsg_ADRC(y,d));
  fhan_Input->fh=-fhan_Input->r*(a/d)*Fsg_ADRC(a,d)
                  -fhan_Input->r*Sign_ADRC(a)*(1-Fsg_ADRC(a,d));//�õ�����΢�ּ��ٶȸ�����
  fhan_Input->x1+=fhan_Input->h*fhan_Input->x2;//�������ٸ���״̬��x1
  fhan_Input->x2+=fhan_Input->h*fhan_Input->fh;//�������ٸ���״̬��΢��x2
}


//ԭ�㸽���������Զε������ݴκ���
float Fal_ADRC(float e,float alpha,float zeta)
{
int16 s=0;
float fal_output=0;
s=(Sign_ADRC(e+zeta)-Sign_ADRC(e-zeta))/2;
fal_output=e*s/(powf(zeta,1-alpha))+powf(ABS(e),alpha)*Sign_ADRC(e)*(1-s);
return fal_output;
}




/************����״̬�۲���********************/
//״̬�۲�������beta01=1/h  beta02=1/(3*h^2)  beta03=2/(8^2*h^3) ...
void ESO_ADRC(Fhan_Data *fhan_Input)
{
  fhan_Input->e=fhan_Input->z1-fhan_Input->y;//״̬���

  fhan_Input->fe=Fal_ADRC(fhan_Input->e,0.5,fhan_Input->h);//�����Ժ�������ȡ����״̬�뵱ǰ״̬���
  fhan_Input->fe1=Fal_ADRC(fhan_Input->e,0.25,fhan_Input->h);

  /*************��չ״̬������**********/
  fhan_Input->z1+=fhan_Input->h*(fhan_Input->z2-fhan_Input->beta_01*fhan_Input->e);
  fhan_Input->z2+=fhan_Input->h*(fhan_Input->z3
                                 -fhan_Input->beta_02*fhan_Input->fe
                                   +fhan_Input->b0*fhan_Input->u);
 //ESO����״̬���ٶ��źţ������Ŷ���������ͳMEMS������Ư�ƽϴ󣬹��ƻ����Ư��
  fhan_Input->z3+=fhan_Input->h*(-fhan_Input->beta_03*fhan_Input->fe1);
}


/************���������****************/
/*
void Nolinear_Conbination_ADRC(Fhan_Data *fhan_Input)
{
  float d=0,a0=0,y=0,a1=0,a2=0,a=0;
  float Sy=0,Sa=0;//ADRC״̬���������

  fhan_Input->h1=fhan_Input->N1*fhan_Input->h;

  d=fhan_Input->r*fhan_Input->h1*fhan_Input->h1;
  a0=fhan_Input->h1*fhan_Input->c*fhan_Input->e2;
  y=fhan_Input->e1+a0;
  a1=sqrt(d*(d+8*ABS(y)));
  a2=a0+Sign_ADRC(y)*(a1-d)/2;

  Sy=Fsg_ADRC(y,d);
  a=(a0+y-a2)*Sy+a2;
  Sa=Fsg_ADRC(a,d);
  fhan_Input->u0=-fhan_Input->r*((a/d)-Sign_ADRC(a))*Sa-fhan_Input->r*Sign_ADRC(a);

  //a=(a0+y)*Fsg_ADRC(y,d)+a2*(1-Fsg_ADRC(y,d));

  //fhan_Input->fh=-fhan_Input->r*(a/d)*Fsg_ADRC(a,d)
  //                -fhan_Input->r*Sign_ADRC(a)*(1-Fsg_ADRC(a,d));//�õ�����΢�ּ��ٶȸ�����
}
*/
void Nolinear_Conbination_ADRC(Fhan_Data *fhan_Input)
{
  float temp_e2=0;
  temp_e2=Constrain_Float(fhan_Input->e2,-3000,3000);
  fhan_Input->u0=fhan_Input->beta_1*Fal_ADRC(fhan_Input->e1,fhan_Input->alpha1,fhan_Input->zeta)
                +fhan_Input->beta_2*Fal_ADRC(temp_e2,fhan_Input->alpha2,fhan_Input->zeta);

}


void ADRC_Control(Fhan_Data *fhan_Input,float expect_ADRC,float feedback_ADRC)
{
    /*�Կ��ſ�������1��*/
    /********
        **
        **
        **
        **
        **
     ********/
      /*****
      ���Ź��ȹ��̣�����Ϊ����������
      ��TD����΢�����õ���
      ���������ź�x1����������΢���ź�x2
      ******/
      Fhan_ADRC(fhan_Input,expect_ADRC);

    /*�Կ��ſ�������2��*/
    /********
            *
            *
       ****
     *
     *
     ********/
      /************ϵͳ���ֵΪ��������״̬������ESO����״̬�۲���������*********/
      fhan_Input->y=feedback_ADRC;
      /*****
      ����״̬�۲������õ������źŵ�����״̬��
      1��״̬�ź�z1��
      2��״̬�ٶ��ź�z2��
      3��״̬���ٶ��ź�z3��
      ����z1��z2������Ϊ״̬������TD΢�ָ������õ���x1,x2�����
      ���������Ժ���ӳ�䣬����betaϵ����
      ��ϵõ�δ����״̬���ٶȹ����Ŷ�������ԭʼ������u
      *********/
      ESO_ADRC(fhan_Input);//�ͳɱ�MEMS�����Ư�ƣ���չ������z3�����Ư�ƣ�Ŀǰ��ʱδ�뵽�취�����δ�õ�z3
    /*�Կ��ſ�������3��*/
    /********
           **
         **
       **
         **
           **
     ********/
      /********״̬������***/
      fhan_Input->e0+=fhan_Input->e1*fhan_Input->h;//״̬������
      fhan_Input->e1=fhan_Input->x1-fhan_Input->z1;//״̬ƫ����
      fhan_Input->e2=fhan_Input->x2-fhan_Input->z2;//״̬΢���
      /********�������*******/
     /*
      fhan_Input->u0=//fhan_Input->beta_0*fhan_Input->e0
                    +fhan_Input->beta_1*fhan_Input->e1
                    +fhan_Input->beta_2*fhan_Input->e2;
    */
      Nolinear_Conbination_ADRC(fhan_Input);
      /**********�Ŷ�����*******/
      //fhan_Input->u=fhan_Input->u0
      //             -fhan_Input->z3/fhan_Input->b0;
      //����MEMS������Ư�ƱȽ����أ���beta_03ȡֵ�Ƚϴ�ʱ����ʱ��z3Ư�ƱȽϴ�Ŀǰ�������Ŷ�����������
      fhan_Input->u=Constrain_Float(fhan_Input->u0,-200,200);
}
