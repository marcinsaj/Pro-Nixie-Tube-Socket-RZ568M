/*----------------------------------------------------------------------*
 * A simple cross fade effect example for 1 Pro Nixie Socket            *
 *                                                                      *
 * How to connect 1 Pro Nixie Socket to the Arduino:                    *
 * https://bit.ly/PRO-1-ARD                                             *
 *                                                                      *
 * PWM_PIN is dedicated to control tube brightness,                     *
 * fade in/out and crossfade effect.                                    *
 * Can also be used to simply turn on/off the tube.                     *
 *                                                                      *
 * The GPL3 License                                                     *
 * Marcin Saj 1 NOV 2022                                                *
 * https://github.com/marcinsaj/Pro-Nixie-Tube-Socket-RZ568M            *
 *----------------------------------------------------------------------*/


#include <SPI.h>

#define PWM_PIN         9                       //PWM or simple on/off function
#define EN_PIN         10                       //Required to control the SPI data transfer

#define TURN_ON        LOW
#define TURN_OFF       HIGH

uint16_t nixieDigitArray[10]
{
  0b0000000000100000,  // 0
  0b0000010000000000,  // 1
  0b0000000001000000,  // 2
  0b1000000000000000,  // 3
  0b0000000010000000,  // 4
  0b0001000000000000,  // 5
  0b0000000000010000,  // 6
  0b0100000000000000,  // 7
  0b0000001000000000,  // 8
  0b0000100000000000   // 9
};

void setup() 
{
  pinMode(PWM_PIN, OUTPUT);
  NixieDisplayStatus(TURN_OFF);                 //Turn OFF nixie tube
  
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);

  SPI.begin();

  ClearNixieDisplay();
  NixieDisplayStatus(TURN_ON);                  //Turn ON nixie tube
}

void loop() 
{
  for(int i = 0; i <= 9; i++)
  {
    simpleCrossFade(i); 
    delay(1000);
  }
}

void simpleCrossFade(uint8_t digit)
{
  uint8_t nextdigit;
  
  if(digit == 9) nextdigit = 0;
  else nextdigit = digit +1;

  for(int i = 0; i <= 30; i++)                  //30 brightness steps for cross fade effect
  {
    for(int j = 0; j <= 1; j++)
    {
      NixieDisplay(digit);                      //Display digit
      delay((30-i)/3);                          //Wait the first part
      NixieDisplay(nextdigit);                  //Then display next digit
      delay(i/3);                               //Wait the second part
    }
  }
}

void NixieDisplay(uint8_t digit)
{
  digitalWrite(EN_PIN, LOW);                    //Transfer data begin
  
  SPI.transfer(nixieDigitArray[digit] >> 8);    //Send first byte
  SPI.transfer(nixieDigitArray[digit]);         //Send second byte
  
  digitalWrite(EN_PIN, HIGH);                   //Transfer data complete
}

void NixieDisplayStatus(bool nixiestatus)
{
  digitalWrite(PWM_PIN, nixiestatus);           //Turn ON/OFF nixie tube 
}

void ClearNixieDisplay(void)
{
  NixieDisplay(0);                              //Clear shift registers  
}
