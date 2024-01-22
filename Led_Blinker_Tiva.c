#include <stdint.h>
#include <stdio.h>
#include "C:\ti\TivaWare_C_Series-2.1.4.178\inc\tm4c123gh6pm.h"
#define COLOR_GREEN 0x08
#define COLOR_BLUE 0x04
#define COLOR_CYAN 0x0C
#define COLOR_RED 0x02
#define COLOR_YELLOW 0x0A
#define COLOR_MAGNETA 0x06
#define COLOR_WHITE 0x0E
#define MAX_DELAY 500
void delayMs(int n);
int main(void)
{ SYSCTL_RCGC2_R |= 0x00000020; /* enable clock to GPIOF at clock gating
control register */
GPIO_PORTF_LOCK_R = 0x4C4F434B;//Unlock CR register
GPIO_PORTF_CR_R |= 0x01;//Enable pull_up on PF0
GPIO_PORTF_PUR_R |= 0x11;//Pull-up PF4(sw1) and PF0(sw0)
GPIO_PORTF_DIR_R |= 0x0E; /* enable the GPIO pins for the LED (PF3, 2
1) as output */
GPIO_PORTF_DEN_R |= 0x1F; /* enable the GPIO pins for digital function
*/

int unsigned delay=MAX_DELAY;
int unsigned state=0x00;
int unsigned color=0x00;
while(1) {
         if((GPIO_PORTF_DATA_R & 0x10) == 0x00){
             switch(state){
               case 0x00:
                   GPIO_PORTF_DATA_R = COLOR_GREEN;
                   state=state+0x01;
               break;

               case 0x01:
                   GPIO_PORTF_DATA_R = COLOR_BLUE;
                   state=state+0x01;
               break;

               case 0x02:
                   GPIO_PORTF_DATA_R = COLOR_CYAN;
                   state=state+0x01;
               break;

               case 0x03:
                   GPIO_PORTF_DATA_R = COLOR_RED;
                   state=state+0x01;
               break;

               case 0x04:
                   GPIO_PORTF_DATA_R = COLOR_YELLOW;
                   state=state+0x01;
               break;

               case 0x05:
                   GPIO_PORTF_DATA_R = COLOR_MAGNETA;
                   state=state+0x01;
               break;

               case 0x06:
                   GPIO_PORTF_DATA_R = COLOR_WHITE;
                   state=state+0x01;
               break;
               default:
                   state=0x00;
               break;
             }
         }
             else{
                 GPIO_PORTF_DATA_R =color;
             }


        color=GPIO_PORTF_DATA_R;
        delayMs(delay);
        GPIO_PORTF_DATA_R = 0x00; /* turn off all LEDs */
        delayMs(delay);

        if((GPIO_PORTF_DATA_R & 0x01) == 0x00){
            if(delay>8){
                delay=delay/2;
            }
            else{
                delay=MAX_DELAY;
            }

        }

 }
}
/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n)
{
int i, j;
for(i = 0 ; i < n; i++)
for(j = 0; j < 3180; j++) {} /* do nothing for 1 ms */
}
