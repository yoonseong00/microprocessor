// 신호등 재현
// 상황: 5초동안 차량신호가 초록불->2초동안 노란불 점멸->차량신호 빨간불이 되고, 보행자 신호는 10초동안 지속 -> 보행자 신호가 끝나면 2초동안 보행자,차량 신호 빨간불

#include <reg51.h>

bit SW1_state=1;

unsigned char seqTable[10]={0x81,0xB7,0xC2,0x92,0xB4,0x98,0x88,0xB1,0x80,0xB0};
unsigned char seg = 0xFF;


unsigned char P0_Out_1 = 0xEE; //보행자 신호 빨강불, 차량신호 초록불 1번 상황
unsigned char P0_Out_2 = 0xF6; //보행자 신호 빨강불, 차량신호 노란불 2번 상황
unsigned char P0_Out_3 = 0xF9; //보행자 신호 초록불, 차량신호 빨간불 3번 상황
unsigned char P0_Out_4 = 0xFB; //보행자 신호 꺼짐, 차량신호 빨간불 4번 상황
unsigned char P0_Out_5 = 0xFA; //보행자 신호 빨강불, 차량신호 빨간불 5번 상황

int TMR0Count=0;
int i=0;
int state=0;
char displayNumber=9;

void delay(unsigned int i)
{
   while(i--);
}

void main(void)
{
   TMOD=0x01;
   TH0=0x4B;
   TL0=0xFD;
   ET0=1;
   IT0=1;
   EX0=1;
   EA=1;
   TR0=1;
   
   while(1)
   {
      if(SW1_state==1)
      {
      P1=seg;
      P0=P0_Out_1;
      }
      if(SW1_state==0)
      {
         switch(state)
         {
            case(0): // swtich on->0~5sec 1번 case
              P1=seg;
              P0=P0_Out_1;
              continue;
            case(1): // 5~7sec 2번 case
              P0=P0_Out_2;
              continue;
            case(2): //세그먼트 9~5(7~12sec)3번 case
                     P1=seqTable[displayNumber];
              P0=P0_Out_3;
              continue;
            case(3): //세그먼트 4~0second(12~17sec) 보행자 신호 점멸  3,4번 case 반복
              P1=seqTable[displayNumber];
              P0=P0_Out_3;
              delay(0xb100);
              P1=seg;
              P0=P0_Out_4;
              delay(0xb100);
              continue;
            case(4): //17~19sec 5번 case = end
              P0=P0_Out_5;
              P1=seg;
              continue;
         }
      }
   }
}

void TMR0_Interrput(void) interrupt 1
{
   EA=0;
   TMR0Count++;
   if(TMR0Count==100)
   {
      state=1;
   }
   if(TMR0Count==140)
   {
         displayNumber=9;
      state=2;
   }
   if(TMR0Count==240)
   {
      state=3;
   }
   if(TMR0Count==340)
   {
      state=4;
   }
   if(TMR0Count==380)
   {
      SW1_state=1;
      TMR0Count=0;
   }
   if((TMR0Count%20)==0 && (TMR0Count>=160))
   {
      displayNumber--;
      if(displayNumber<0)
      {
         displayNumber=9;
      }
   }
   TH0=0x4B;
   TL0=0xFD;
   EA=1;
}

void EXT0_Interrupt(void) interrupt 0
{
   EA=0;
   SW1_state=~SW1_state;
   state=0;
   TH0=0x4B;
   TL0=0xFD;
   TMR0Count=0;
   EA=1;
}