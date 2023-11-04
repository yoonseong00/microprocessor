// 도트매트릭스로 신호등 만들기

#include <reg51.h>
#define No_Op        0x00
#define Decode_Mode  0x09
#define Intensity    0x0A
#define Scan_Limit   0x0B
#define Shutdown     0x0C
#define Display_Test 0x0F

sbit LED_R=P1^0;  sbit LED_G=P1^1;
sbit CLK=P2^0;  sbit LOAD=P2^1; sbit DIN=P2^2;

int TMR0Count=0;
int Time_100ms=0;

unsigned char image[11][8]={
  {0x00,0x7e,0x81,0x81,0x81,0x81,0x7e,0x00},  //0
  {0x00,0x00,0x82,0xff,0xff,0x80,0x00,0x00},  //1
  {0x00,0xe2,0x91,0x91,0x91,0x91,0x8e,0x00},  //2
  {0x00,0x42,0x89,0x89,0x89,0x89,0x76,0x00},  //3
  {0x00,0x30,0x28,0x24,0x22,0xff,0x20,0x00},  //4
  {0x00,0x4f,0x89,0x89,0x89,0x89,0x71,0x00},  //5
  {0x00,0x7e,0x89,0x89,0x89,0x89,0x72,0x00},  //6
  {0x00,0x01,0xc1,0x21,0x11,0x09,0x07,0x00},  //7
  {0x00,0x76,0x89,0x89,0x89,0x89,0x76,0x00},  //8
  {0x00,0x46,0x89,0x89,0x89,0x89,0x76,0x00},  //9
  {0x18,0x04,0xfc,0x1f,0x1f,0xfc,0x04,0x18} // off
};

void Interrupt_INIT(void) {
  //Timer Interrupt Init.
  TMOD=0x01;
  TH0=0x4C;   TL0=0x00;
  ET0=1;
  EA=1;
  TR0=1;
}
void TMR0_int(void) interrupt 1{
  EA=0;
  TMR0Count++;
  if(TMR0Count==2){
    if(Time_100ms<150)  Time_100ms++; //0.1초 증가
    else  Time_100ms=0;
    TMR0Count=0;
  }
  TH0=0x4C;
  TL0=0x00;
  EA=1;
}

void Write_Max7219(unsigned char address, unsigned char dat) {
  int i=0;
  LOAD=0;
  for(i=0;i<8;i++){	
    CLK=0;
    DIN= (bit)(address & 0x80);
    address<<=1;	
    CLK=1;
  }
  for(i=0;i<8;i++){	
    CLK=0;
    DIN= (bit)(dat & 0x80);
    dat<<=1;
    CLK=1;
  }
  LOAD=1;
}

void DotMatrixInit(void){
  Write_Max7219(Decode_Mode,0);   //decode
  Write_Max7219(Intensity,5);     //intensity
  Write_Max7219(Scan_Limit,7);    //scan limit
  Write_Max7219(Shutdown,1);      // shutdown
  Write_Max7219(Display_Test,0);  //display test 
}

void DotMatrixImageWrite(unsigned char image_number) {
  int i=0;
  for(i=0;i<8;i++){
    Write_Max7219(i+1,image[image_number][i]); //정지영상
  }  
}
void main(void) {
  int i=0;
  Interrupt_INIT();
  DotMatrixInit();
  
  while(1){
    if(Time_100ms<50) {   //0~5 sec
      LED_R=0; //LED_R ON
      LED_G=1; //LED_G OFF
      DotMatrixImageWrite(10);  // fixed image
    }
    else if(Time_100ms<100) { //5~10sec
      LED_R=1;  //LED_R OFF
      LED_G=0;  //LED_G ON
      //DotMatrix control
      if(Time_100ms<60) DotMatrixImageWrite(9);
      else if(Time_100ms<70) DotMatrixImageWrite(8);
      else if(Time_100ms<80) DotMatrixImageWrite(7);
      else if(Time_100ms<90) DotMatrixImageWrite(6);
      else if(Time_100ms<100) DotMatrixImageWrite(5);     
    }
    else if(Time_100ms<150) { //10~15sec
      LED_R=1;  //LED_R OFF
      if(Time_100ms<110) {
        DotMatrixImageWrite(4);
        if(Time_100ms<105) LED_G=1; //OFF
        else LED_G=0;               //ON
      }
      else if(Time_100ms<120) {
        DotMatrixImageWrite(3);
        if(Time_100ms<115) LED_G=1; //OFF
        else LED_G=0;               //ON
      }
      else if(Time_100ms<130) {
        DotMatrixImageWrite(2);
        if(Time_100ms<125) LED_G=1; //OFF
        else LED_G=0;               //ON
      }
      else if(Time_100ms<140) {
        DotMatrixImageWrite(1);
        if(Time_100ms<135) LED_G=1; //OFF
        else LED_G=0;               //ON
      }
      else if(Time_100ms<150) {
        DotMatrixImageWrite(0);
        if(Time_100ms<145) LED_G=1; //OFF
        else LED_G=0;               //ON
      }
    }
  }
}