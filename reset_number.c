// 외부 인터럽트 1번을 사용하여 자동으로 증가하는 숫자 초기화

#include <reg51.h>
char displayNumber=0; //전역변수로 설정

void delay(unsigned int i)
{
while(i--);
}
void main(void)
{
 unsigned char
seqTable[10]={0x81,0xB7,0xC2,0x92,0xB4,0x98,0x88,0xB1,0x80,0xB0};

 IT1=1; // 외부 인터럽트 1 조건 설정 : edge trigger
 EX1=1; // 외부 인터럽트 1 활성화
 PX1=1; // 외부 인터럽트 1 우선순위를 1로
 EA=1; // 전체 인터럽트 활성화

 while(1){
 P1=seqTable[displayNumber];

 displayNumber++;
 if(displayNumber>9) displayNumber=0;

 delay(0xb100);
 delay(0xb100);
 }
}
void EXT1_int(void) interrupt 2 // 외부인터럽트 1 서비스 루틴
{
 displayNumber=0;
}