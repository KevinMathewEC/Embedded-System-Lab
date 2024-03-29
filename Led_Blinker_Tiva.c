#include <stdint.h>
#include <stdio.h>
//#include "C:\ti\TivaWare_C_Series-2.1.4.178\inc\tm4c123gh6pm.h"
# define SYSCTL_RCGCGPIO_R (*(( volatile unsigned long *)0x400FE608))
# define GPIO_PORTF_DATA_RD (*(( volatile unsigned long *)0x40025044))
# define GPIO_PORTF_DATA_WR (*(( volatile unsigned long *)0x40025038))
# define GPIO_PORTF_DIR_R (*(( volatile unsigned long *)0x40025400))
# define GPIO_PORTF_DEN_R (*(( volatile unsigned long *)0x4002551C))
# define GPIO_PORTF_PUR_R (*(( volatile unsigned long *)0x40025510 ))
# define GPIO_PORTF_LOCK_R (*((volatile unsigned long *)0x40025520))
# define GPIO_PORTF_CR_R  (*((volatile unsigned long *)0x40025524))
# define SYSCTL_RCGC2_GPIOF 0x0020
# define GPIO_PORTF_PIN3_1_EN 0x0E
# define GPIO_PORTF_PIN0_EN 0x01
# define GPIO_PORTF_PIN4_EN 0x10
#define COLOR_GREEN 0x08
#define COLOR_BLUE 0x04
#define COLOR_CYAN 0x0C
#define COLOR_RED 0x02
#define COLOR_YELLOW 0x0A
#define COLOR_MAGNETA 0x06
#define COLOR_WHITE 0x0E
#define MAX_DELAY 500
int unsigned state=0x00;
int unsigned color=0x00;
short all_on=0;
int unsigned delay=MAX_DELAY;
void delayMs(int n);
void LED_off();
int main(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGC2_GPIOF; // enable clock to GPIOF at clock gating
    GPIO_PORTF_LOCK_R = 0x4C4F434B;//Unlock CR register
    GPIO_PORTF_CR_R |= 0x01;//Enable pull_up on PF0
    GPIO_PORTF_DEN_R |= GPIO_PORTF_PIN3_1_EN +GPIO_PORTF_PIN0_EN+GPIO_PORTF_PIN4_EN;
    GPIO_PORTF_DIR_R |= GPIO_PORTF_PIN3_1_EN;/* enable the GPIO pins for the LED (PF3, 2
//1) as output */
    GPIO_PORTF_DIR_R &= (~(GPIO_PORTF_PIN0_EN+GPIO_PORTF_PIN4_EN));
    GPIO_PORTF_PUR_R |= GPIO_PORTF_PIN0_EN+GPIO_PORTF_PIN4_EN;//Pull-up PF4(sw1) and PF0(sw0)

while(1) {

    switch(state){
              case 0x00:
                  GPIO_PORTF_DATA_WR = COLOR_GREEN;
                  delayMs(delay);
              break;

              case 0x01:
                  GPIO_PORTF_DATA_WR = COLOR_BLUE;
                  delayMs(delay);
              break;

              case 0x02:
                  GPIO_PORTF_DATA_WR = COLOR_CYAN;
                  delayMs(delay);
              break;

              case 0x03:
                  GPIO_PORTF_DATA_WR = COLOR_RED;
                  delayMs(delay);
              break;

              case 0x04:
                  GPIO_PORTF_DATA_WR = COLOR_YELLOW;
                  delayMs(delay);

              break;

              case 0x05:
                  GPIO_PORTF_DATA_WR = COLOR_MAGNETA;
                  delayMs(delay);
              break;

              case 0x06:
                  GPIO_PORTF_DATA_WR = COLOR_WHITE;
                  delayMs(delay);
              break;
              default:
                  state=0x00;
              break;
            }
    LED_off();

 }
}
/* delay n milliseconds (16 MHz CPU clock) */
void switch_press()
{
    if((GPIO_PORTF_DATA_RD & 0x10) == 0x00){
        for(int i = 0 ; i < 50; i++)
            for(int j = 0; j < 3180; j++) {} /* do nothing for 1 ms */
        if((GPIO_PORTF_DATA_RD & 0x10) == 0x00)
            state=state+0x01;
    }


    if((GPIO_PORTF_DATA_RD & 0x01) == 0x00){
        for(int i = 0 ; i < 50; i++)
                    for(int j = 0; j < 3180; j++) {} /* do nothing for 1 ms */
        if((GPIO_PORTF_DATA_RD & 0x01) == 0x00){
         if(delay>=8){
            delay=delay/2;
            all_on=0;
         }
         else{
            all_on=1;
            delay=MAX_DELAY*2;
         }
        }
    }

}
void LED_off(){
    if(!all_on){
    GPIO_PORTF_DATA_WR = 0x00; /* turn off all LEDs */
    delayMs(delay);
    }
}
void delayMs(int n)
{
int i, j;
for(i = 0 ; i < n; i++){
    switch_press();
for(j = 0; j < 3180; j++) {} /* do nothing for 1 ms */
}
}





