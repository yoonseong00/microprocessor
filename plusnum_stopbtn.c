#include <reg51.h>
int TMR0Count=0;
char displayNumber=0;
void main(void)
{
 unsigned char
seqTable[10]={0x81,0xB7,0xC2,0x92,0xB4,0x98,0x88,0xB1,0x80,0xB0};

 TMOD=0x01;
 TH0=0x3c; TL0=0xb0;
 ET0=1;
 IT0=1;
 EX0=1;
 EA=1;
 TR0=1;

 while(1){
 P1=seqTable[displayNumber];
 }
}
void EXT0_int(void) interrupt 0
{
 EA=0;
 displayNumber=0;
 TH0=0x3c;
 TL0=0xb0;
 TMR0Count=0;
 EA=1;
}
void TMR0_int(void) interrupt 1 
{
 EA=0;
 TMR0Count++;
 if(TMR0Count==20){
 displayNumber++;
 if(displayNumber>9) displayNumber=0;
 TMR0Count=0;
 }
 TH0=0x3c;
 TL0=0xb0;
 EA=1;
}