//버튼 누르면 숫자 감소 증가 바꾸기

#include <reg51.h>
bit state=1; //전역변수로 설정

void delay(unsigned int i)
{
while(i--);
}
void main(void)
{
 char displayNumber=0;
 unsigned char seqTable[10]={0x81,0xB7,0xC2,0x92,0xB4,0x98,0x88,0xB1,0x80,0xB0};
 //INT0
 IT0=1; // 외부 인터럽트 0 조건 설정 : edge trigger
 EX0=1; // 외부 인터럽트 0 활성화
 PX0=1; // 외부 인터럽트 0 우선순위를 1로
 EA=1; // 전체 인터럽트 활성화

 while(1){
 P1=seqTable[displayNumber];

 if(state==1){
 displayNumber++;
 if(displayNumber>9) displayNumber=0;
 }
 else{
 displayNumber--;
 if(displayNumber<0) displayNumber=9;
 }

 delay(0xb100);
 delay(0xb100);
 }
}
void EXT0_int(void) interrupt 0 // 외부인터럽트 0 서비스 루틴
{
 state=~state;
}