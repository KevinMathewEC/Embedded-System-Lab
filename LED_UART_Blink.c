#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "C:\ti\TivaWare_C_Series-2.1.4.178\inc\tm4c123gh6pm.h"
#include <C:\ti\TivaWare_C_Series-2.1.4.178\inc\hw_memmap.h>
#include <C:\ti\TivaWare_C_Series-2.1.4.178\inc\hw_types.h>
#include <C:\ti\TivaWare_C_Series-2.1.4.178\driverlib\gpio.h>
#include <C:\ti\TivaWare_C_Series-2.1.4.178\driverlib\pin_map.h>
#include <C:\ti\TivaWare_C_Series-2.1.4.178\driverlib\sysctl.h>
#include <C:\ti\TivaWare_C_Series-2.1.4.178\driverlib\uart.h>
//# define SYSCTL_RCGCGPIO_R (*(( volatile unsigned long *)0x400FE608))
# define GPIO_PORTF_DATA_RD (*(( volatile unsigned long *)0x40025044))
# define GPIO_PORTF_DATA_WR (*(( volatile unsigned long *)0x40025038))
//# define GPIO_PORTF_DIR_R (*(( volatile unsigned long *)0x40025400))
//# define GPIO_PORTF_DEN_R (*(( volatile unsigned long *)0x4002551C))
//# define GPIO_PORTF_PUR_R (*(( volatile unsigned long *)0x40025510 ))
//# define GPIO_PORTF_LOCK_R (*((volatile unsigned long *)0x40025520))
//# define GPIO_PORTF_CR_R  (*((volatile unsigned long *)0x40025524))
//# define SYSCTL_RCGC2_GPIOF 0x0020
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
#define MAX_INP_BUFF_SIZE 30
#define CR   0x0D//Carriage return
#define LF   0x0A//Line feed
#define BS   0x08//Backspace
#define SP   0x20//Space
#define BAUD_RATE 115200
int unsigned delay=MAX_DELAY;
void delayMs(int n);
void LED_off();
void UART0_Transmitter(char);
void UART0_init();
void UART_cmpInp();//Compute data received from console
void UART_InString(char* , uint16_t);
void OutCRLF(void);
void UART_OutChar(unsigned char );
void UART_OutString(char*);
void Clear_InpBuff();
int unsigned state=0x00;
int unsigned color=0x00;//Color of LED
short all_on=0;//set when LED to be set to 1
short inp_done=0;//set when newline received from console
char string[50];
char Inp_buffer[50];
int length=0;
int char_cnt=0;
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

    //UART
   UART0_init();



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
void UART_cmpInp()
{
    short i=0;

    UART_InString(string,10);

    if(!inp_done){
        Inp_buffer[char_cnt]=string[0];
        char_cnt++;
    }

   if(inp_done){
       char_cnt=0;
       length=0;
       inp_done=0;
    while(Inp_buffer[i]!=CR){
        Inp_buffer[i] = tolower(Inp_buffer[i]);
        i++;
    }
    UART_OutString(Inp_buffer);
    OutCRLF();
    char* token = strtok(Inp_buffer, " ");

    typedef struct Color {
        char color_name[7];
        int state_value;
    } Color;

    Color colors[] = {{"green", 0}, {"blue", 1},\
                      {"cyan", 2} , {"red", 3},\
                      {"yellow", 4}, {"magenta", 5},\
                      {"white", 6}};
    if(!(strcmp(token,"color"))){

        UART_OutString("color name");
        token = strtok(NULL, " ");
         UART_OutString(token);
        OutCRLF();
        for (i=0; i < 7; i++) {
           if (strcmp(token, colors[i].color_name) == 0) {
             state = colors[i].state_value;

    }
   }
  }
    else if(!(strcmp(token,"blink"))){
        token = strtok(NULL, " ");
        UART_OutString("blink rate");

        if(((int)token)==0)
            all_on=1;
        else{
         all_on=0;
         int test=atoi(token);
        delay=MAX_DELAY/(test);

        OutCRLF();
        }
    }
    else{
        UART_OutString("Given command is invalid.\n\rCommand Help\n\rValid commands are,\
                  \n\r [1] color <color name> ; color name must be green, blue, cyan, red, yellow, magenta or white.\
                  \n\r [2] blink <blink rate> ; blink rate must be an integer\n\r");
    }

    UART_OutString("done");
   }
}
void UART0_init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), BAUD_RATE,(UART_CONFIG_WLEN_8\
                        | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

}


void OutCRLF(void){
  UART_OutChar(CR);
  UART_OutChar(LF);
}
unsigned char UART_InChar(void){
  while((UART0_FR_R&UART_FR_RXFE) != 0);
  return((unsigned char)(UART0_DR_R&0xFF));
}
void UART_OutChar(unsigned char data){
  while((UART0_FR_R&UART_FR_TXFF) != 0);
  UART0_DR_R = data;
}
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART_OutString(char *pt){
  char* ptr = pt;
  while(*ptr){
    UART_OutChar(*ptr);
    ptr++;
  }
}
void UART_InString(char *bufPt, uint16_t max) {

char character;

 character = UART_InChar();

  if(character != CR){
    if(character == BS){
      if(length){
        bufPt--;
        length--;
        UART_OutChar(BS);
      }
    }
    else if(length>=max){
        UART_OutString("Input Buffer full");
        OutCRLF();
        Clear_InpBuff();
        length=0;
    }
    else if((length < max)&&(character >= 32 && character < 127)){

      *bufPt = character;
      bufPt++;
      length++;
      UART_OutChar(character);

    }
    else{
        UART_OutChar(character);
        length++;
    }
  }
  else{
  *bufPt = 0;
   inp_done=1;

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
void Clear_InpBuff(){
    for(int i=0;i<MAX_INP_BUFF_SIZE;i++)
        Inp_buffer[i]=0;
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
    if(UARTCharsAvail(UART0_BASE))
        UART_cmpInp();
for(j = 0; j < 3180; j++) {} /* do nothing for 1 ms */
}
}





