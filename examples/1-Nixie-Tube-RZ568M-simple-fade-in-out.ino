/*----------------------------------------------------------------------*
 * A simple example of fade in-out effect                               *
 * of 1 RZ568M Nixie Tube                                              *
 *                                                                      *
 * How to connect 1x Pro Nixie Tube Sockets to the Arduino:             *
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
#include <SoftPWM.h>                            //https://github.com/bhagman/SoftPWM

#define PWM_PIN         9                       //PWM output
#define EN_PIN         10                       //Required to control the SPI data transfer


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
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);
    
  SPI.begin();

  SoftPWMBegin();                               //Initialize
  SoftPWMSet(PWM_PIN, 1);                       //Setup PWM output and set default state
  SoftPWMSetFadeTime(PWM_PIN, 500, 400);        //Setup time effect: fade-in 500ms, fade-out 400ms
}

void loop() 
{
  for(int i = 0; i <= 9; i++)
  {
    SoftPWMSetPercent(PWM_PIN, 0);              //Set PWM to 0% - turn off nixie tube
    NixieDisplay(i);
    delay(500);
    SoftPWMSetPercent(PWM_PIN, 100);            //Set PWM to 100% - turn on nixie tube
    delay(500);
  }
}

void NixieDisplay(uint8_t digit)
{
  digitalWrite(EN_PIN, LOW);                    //Transfer data begin
  SPI.transfer(nixieDigitArray[digit] >> 8);
  SPI.transfer(nixieDigitArray[digit]);
  digitalWrite(EN_PIN, HIGH);                   //Transfer data complete
}
