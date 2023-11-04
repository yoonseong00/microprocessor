// 알파벳 표시하기(7세그먼트)

#include <reg51.h>

void delay(unsigned int i)
{
while(i--);
}

void main(void)
{
unsigned char seqTable[5]={0xA0,0x8C,0xCE,0x86,0xC8}; char displayNumber=0;

while(1){ P3=seqTable[displayNumber]; delay(0xb100); delay(0xb100); displayNumber++;
if(displayNumber>4) displayNumber=0;
}
}