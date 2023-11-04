// 외부 인터럽트0를 이용하여 3개의 상태(state) 조정

#include <reg51.h>

unsigned char segment=0;

void delay(unsigned int i)
{
while(i--);
}
void main(void)
{
 char displayNumber=0;
 unsigned char seqTable[10]={0x81,0xB7,0xC2,0x92,0xB4,0x98,0x88,0xB1,0x80,0xB0};

 IT0=1; 
 EX0=1;
 PX0=1; 
 EA=1; 

 while(1){
 P1=seqTable[displayNumber];

 if(segment==0){
 displayNumber++;
 if(displayNumber>9) displayNumber=0;
 }
 if(segment==1){
 displayNumber--;
 if(displayNumber<0) displayNumber=9;
 }if(segment==2)
 {
 displayNumber=displayNumber;
 }

 delay(0xb100);
 delay(0xb100);
 }
}
void EXT0_int(void) interrupt 0 
{
 segment=segment++;
 if(segment=3){
 segment=0;
 }
}