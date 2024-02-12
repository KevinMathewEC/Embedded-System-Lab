#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifndef GPIO_H_
#include"GPIO.h"
#endif
#ifndef KEYPAD_H_
#include"KEYPAD.h"
#endif
//#include "C:\ti\TivaWare_C_Series-2.1.4.178\inc\tm4c123gh6pm.h"
#include <C:\ti\TivaWare_C_Series-2.1.4.178\inc\hw_memmap.h>
#include <C:\ti\TivaWare_C_Series-2.1.4.178\inc\hw_types.h>
#include <C:\ti\TivaWare_C_Series-2.1.4.178\driverlib\gpio.h>
#include <C:\ti\TivaWare_C_Series-2.1.4.178\driverlib\pin_map.h>
#include <C:\ti\TivaWare_C_Series-2.1.4.178\driverlib\sysctl.h>
#include <C:\ti\TivaWare_C_Series-2.1.4.178\driverlib\uart.h>
# define GPIO_PORTF_DATA_RD (*(( volatile unsigned long *)0x40025044))
# define GPIO_PORTF_DATA_WR (*(( volatile unsigned long *)0x40025038))
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
#define NUM_ZERO 0x3F
#define NUM_ONE 0x06
#define NUM_TWO 0x5B
#define NUM_THREE 0X4F
#define NUM_FOUR 0X66
#define NUM_FIVE 0X6D
#define NUM_SIX 0X7D
#define NUM_SEVEN 0X07
#define NUM_EIGHT 0X7F
#define NUM_NINE 0X6F
#define CR   0x0D//Carriage return
#define LF   0x0A//Line feed
#define BS   0x08//Backspace
#define SP   0x20//Space
#define BAUD_RATE 115200
int unsigned delay=MAX_DELAY;
void delayMs(int n);
void LED_off();
void UART0_init();
void UART_cmpInp();//Compute data received from console
void UART_InString(char* , uint16_t);
unsigned char UART_InChar(void);
void OutCRLF(void);
void UART_OutChar(unsigned char );
void UART_OutString(char*);
void Clear_InpBuff();
void Debounce_Delay(int );
int unsigned state=0x00;
int unsigned color=0x00;//Color of LED
short all_on=0;//set when LED to be set to 1
short inp_done=0;//set when newline received from console
//char string[50];
char console_cmd_buffer[50];
int length=0;
int char_cnt=0;
int Color_switch_count=0;//keep track of number of times color has been switched
short Pause=0;
short Stop=0;
typedef struct Number {
       int number;
       int state_value;
   } Number;
Number number[] = {{ 0x00,NUM_ZERO}, {0x01,NUM_ONE},\
                     {0x02,NUM_TWO} , {0x03,NUM_THREE},\
                     {0x04,NUM_FOUR}, {0x05,NUM_FIVE},\
                     {0x06,NUM_SIX},{0x07,NUM_SEVEN},\
                     {0x08,NUM_EIGHT},{0x09,NUM_NINE}};
typedef struct Color {
    char color_name[7];
    int state_value;
} Color;

Color colors[] = {{"green", 0}, {"blue", 1},\
              {"cyan", 2} , {"red", 3},\
              {"yellow", 4}, {"magenta", 5},\
              {"white", 6}};
int main(void)
{
    int Led_Color;
  //  keypad_init();
    LED_init();

    //UART
   UART0_init();
   keypad_init();
   SSD_init();
   UART_OutChar('>');


while(1) {

    if((!Pause)&&(!Stop)){
    switch(state){
              case 0x00:
                  Led_Color = COLOR_GREEN;

              break;

              case 0x01:
                  Led_Color = COLOR_BLUE;

              break;

              case 0x02:
                  Led_Color = COLOR_CYAN;

              break;

              case 0x03:
                  Led_Color = COLOR_RED;

              break;

              case 0x04:
                  Led_Color = COLOR_YELLOW;


              break;

              case 0x05:
                  Led_Color = COLOR_MAGNETA;

              break;

              case 0x06:
                  Led_Color = COLOR_WHITE;

              break;
              default:
                  state=0x00;
              break;
            }
    }
    if(!Stop)
        GPIO_PORTF_DATA_WR = Led_Color;
    delayMs(delay);
    LED_off();

}
}
void SSD_disp(){

    int Color_num;
    int switch_count[2];
    int clr_cnt=Color_switch_count;
    int i,j;
    char str[20];


         Color_num = number[state%7].state_value;

         GPIO_PORTB_DATA_R = Color_num;//Display current color number
         GPIO_PORTA_DATA_R = 0xF0 & 0x10;
         Debounce_Delay(2);
   for(j=0;j<3;j++){
        for (i=0; i < 10; i++) {
           if ((clr_cnt % 10) == number[i].number) {

                  switch_count[j] = number[i].state_value;

           }
        }
        clr_cnt=clr_cnt/10;
  }

   for(i=0;i<3;i++){
    GPIO_PORTB_DATA_R = switch_count[i];//Display number of switch presses
    GPIO_PORTA_DATA_R = 0xF0 & (0x20 << i);
    Debounce_Delay(1);
    }



}
void Keypad_cmpInp()
{

    int Keypad_inp;

    if(isKeyPressed()){

        Debounce_Delay(30);
        if(isKeyPressed()){
        Keypad_inp = readkey() ;

        if(Keypad_inp == 1){//Key 1

           if(!Pause){
            Pause=1;
            all_on=1;

           }
           else{
             Pause=0;
             all_on=0;

           }
        }
        else if(Keypad_inp == 2){//Key 2
           if(!Stop)
            Stop=1;

           else{
            Stop=0;
            state=0x00;
        }
      }
        while(isKeyPressed());
    }
   }
}
void UART_cmpInp()
{
    short i=0,j=0;
    char cmd_string[10];
    char string[5];

  UART_InString(string,MAX_INP_BUFF_SIZE);

    if(!inp_done){
        if(string[0] == BS){

            if(char_cnt)
              char_cnt--;

        }
        else{

        console_cmd_buffer[char_cnt]=string[0];
        char_cnt++;

       }
    }

   if(inp_done){
     while(console_cmd_buffer[i]!=CR){
         if((console_cmd_buffer[i] > 32 && console_cmd_buffer[i] < 127)||(console_cmd_buffer[i]==NULL)){
         console_cmd_buffer[j] = tolower(console_cmd_buffer[i]);
         j++;
         }
        i++;

    }

    switch(console_cmd_buffer[0]){//Check first character of user input to determine how many characters is the command and how much is the command argument
    case 'c':
        // Copy first 5 characters to first_part string
        strncpy(cmd_string, console_cmd_buffer, 5);
        cmd_string[5] = NULL; // Null-terminate first_part string
        break;
    case 'b':
        // Copy first 5 characters to first_part string
        strncpy(cmd_string, console_cmd_buffer, 5);
        cmd_string[5] = NULL; // Null-terminate first_part string
        break;

    default:
         strcpy(cmd_string,console_cmd_buffer);
         break;

    }


    if(!(strcmp(cmd_string,"stop"))){
        Stop=1;
    }
    else if(!(strcmp(cmd_string,"start"))){
        Stop=0;
        state=0x00;
        delay=MAX_DELAY;
    }
    else if((!(strcmp(cmd_string,"color"))) && (!Stop)){
            strcpy(cmd_string, console_cmd_buffer + 5);

        for (i=0; i < 7; i++) {
           if (strcmp(cmd_string, colors[i].color_name) == 0) {
             state = colors[i].state_value;


    }
   }
  }
    else if((!(strcmp(cmd_string,"blink")))&&(!Stop)){
         strcpy(cmd_string, console_cmd_buffer + 5);
          if(atoi(cmd_string)==0){
            all_on=1;

          }
        else{
        if(!Pause) //Do not change of state of all_on if system is Paused
            all_on=0;
         int delay_div=atoi(cmd_string);
        delay=MAX_DELAY/(delay_div);

        }
    }

    else if((!(strcmp(cmd_string,"pause")))&&(!Stop)){
        Pause=1;
        all_on=1;

    }
    else if((!(strcmp(cmd_string,"resume")))&&(!Stop)){
        Pause=0;
        all_on=0;

    }

    else{
        UART_OutString("Given command is invalid.\n\rCommand Help\n\rValid commands are,\
                  \n\r [1] color <color name> ; color name must be green, blue, cyan, red, yellow, magenta or white.\
                  \n\r [2] blink <blink rate> ; blink rate must be an integer\n\r");
    }
    OutCRLF();
    Clear_InpBuff();
    UART_OutChar('>');
    char_cnt=0;
    length=0;
    inp_done=0;
   }
}

void LED_init(){
    //  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGC2_GPIOF; // enable clock to GPIOF at clock gating
      SYSCTL_RCGC2_R |= 0x00000020 ;       /* Enable clock to GPIO_C_ at clock gating control register */

      SYSCTL_RCGCGPIO_R |= 0x00000020 ;    /* Enable and provide a clock to GPIO Port_C_ in Run mode */
      GPIO_PORTF_LOCK_R = 0x4C4F434B;//Unlock CR register
      GPIO_PORTF_CR_R |= 0x01;//Enable pull_up on PF0
      GPIO_PORTF_DEN_R |= GPIO_PORTF_PIN3_1_EN +GPIO_PORTF_PIN0_EN+GPIO_PORTF_PIN4_EN;
      GPIO_PORTF_DIR_R |= GPIO_PORTF_PIN3_1_EN;/* enable the GPIO pins for the LED (PF3, 2
  //1) as output */
     GPIO_PORTF_DIR_R &= (~(GPIO_PORTF_PIN0_EN+GPIO_PORTF_PIN4_EN));
     GPIO_PORTF_PUR_R |= GPIO_PORTF_PIN0_EN+GPIO_PORTF_PIN4_EN;//Pull-up PF4(sw1) and PF0(sw0)
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
void SSD_init(void)
{
    SYSCTL_RCGC2_R |= 0x00000003;       // enable clock to GPIOA, GPIOB at clock gating control register
    // Enable the GPIO pins
    // For PORTB, all pins are used to set 7 segment display
    // For PORTA, pins 7 to 4 are used for selecting one of the four 7 segment display
    GPIO_PORTA_DIR_R |= 0xF0;       // PA4 to PA7 set to output
    GPIO_PORTB_DIR_R |= 0xFF;       // PB0 to PB7 set to output
    // enable the GPIO pins for digital function
    GPIO_PORTA_DEN_R |= 0xF0;       // enabling PA4 to PA7
    GPIO_PORTB_DEN_R |= 0xFF;       // enabling PB0 to PB8
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
        *bufPt=BS;
        length--;
        UART_OutChar(BS);
        UART_OutChar(SP);
        UART_OutChar(BS);
      }
    }

    else if((length < max)){//&&(character > 32 && character < 127)
      *bufPt = character;
      bufPt++;
      length++;
      UART_OutChar(character);

    }

    else {
        UART_OutString("Input Buffer full");
        OutCRLF();
        Clear_InpBuff();
        length=0;
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
    short switch_press;
    while((GPIO_PORTF_DATA_RD & 0x10) == 0x00){
        switch_press=1;
        Debounce_Delay(50);
    }

    if(switch_press==1){
            state=state+0x01;
            switch_press=0;
            Color_switch_count++;
    }

    while((GPIO_PORTF_DATA_RD & 0x01) == 0x00){
        switch_press=2;
        Debounce_Delay(50);
    }

    if(switch_press==2){
        switch_press=0;
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
void Clear_InpBuff(){
    for(int i=0;i<MAX_INP_BUFF_SIZE;i++)
        console_cmd_buffer[i]=0;
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
    if(!Stop)
    switch_press();

   if(isKeyPressed())
       Keypad_cmpInp();
    if(UARTCharsAvail(UART0_BASE))
        UART_cmpInp();
    if((i%3)==1)//Reduce freq of calling SSD_disp to reduce LED blink latency
    SSD_disp();
for(j = 0; j < 3000; j++) {} /* do nothing for 1 ms(run till 3180 for 1ms) */
}
}
void Debounce_Delay(int n)
{
    for(int i = 0 ; i < n; i++)
                for(int j = 0; j < 3180; j++) {} /* do nothing for 1 ms */
}



