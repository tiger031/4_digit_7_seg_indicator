
//timer library for making 2ms intervals
#include <MsTimer2.h>

//   digits array          0    1    2   3    4    5    6    7    8    9
uint8_t digit_masks[] = {0x7E,0x0C,0xB6,0x9E,0xCC,0xDA,0xFA,0x0E,0xFE,0xDE};  


#define clockPin        12                                                    
#define dataPin         10                                                    
#define latchPin        11                                                    
#define pinBeginStrob   3                                                     
#define pinEndStrob     6                                                     
#define numberDigits    4                                                     //  digits quantity
#define durationPause   100                                                     
#define maxCount        1000                                             

uint8_t mass[numberDigits];                                                   //  buffer indicator
int count = 0;                                                     

void setup()
{
  pinMode(clockPin, OUTPUT);                                                  
  pinMode(dataPin, OUTPUT);                                                   
  pinMode(latchPin, OUTPUT);                                                  
  digitalWrite(latchPin, HIGH);                                               

  for (uint8_t i = pinBeginStrob;i <= pinEndStrob;i ++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  
  MsTimer2::set(2, timerInterrupt);                                           //  2 ms interrupts
  MsTimer2::start();                                                           
}

void loop()
{
  count ++;                                                                   //  + 1
  if (count >= maxCount) count = 0;                                           //  maxlimit
  convert(count, mass);                                                       
  delay(durationPause);   
}

//Interrupt handler
void  timerInterrupt()
{
//   indicator regenerating 
static uint8_t index = 0;
//  indicator turn  off
  for (uint8_t i = pinBeginStrob;i <= pinEndStrob;i ++)
  {
     digitalWrite(i, LOW);
  }
//  download digit into indicator
  digitalWrite(latchPin, LOW);                                                
  shiftOut(dataPin, clockPin, MSBFIRST, mass[index]);                         
  digitalWrite(latchPin, HIGH);                                               
  
 digitalWrite(index + pinBeginStrob, HIGH);

  index ++;
  if (index == numberDigits) index = 0;
}


void convert(int value, uint8_t *m)
{
  for (uint8_t i = 0;i < numberDigits;i ++)
  {
    *(m + i) = digit_masks[value %10];
    value /= 10;
  }
}
