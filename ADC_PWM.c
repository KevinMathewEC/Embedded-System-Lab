#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifndef GPIO_H_
#include"GPIO.h"
#endif

#include "C:\ti\TivaWare_C_Series-2.1.4.178\inc\tm4c123gh6pm.h"
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
#define COLOR_RED 0x02
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
#define DOT 0x80
#define TIMER_PAUSE 0x01
#define TIMER_RESUME 0x02
#define TIMER_STOP 0x03
#define TIMER_START 0x04
#define TIMER_RELOAD_VALUE 1600000
#define CR   0x0D//Carriage return
#define LF   0x0A//Line feed
#define BS   0x08//Backspace
#define SP   0x20//Space
#define BAUD_RATE 115200

void delayMs(int n);
void LED_off();
void UART0_init();
void UART_cmpInp();//Compute data received from console
void UART_InString(char* , uint16_t);
unsigned char UART_InChar(void);
void OutCRLF(void);
void UART_OutChar(unsigned char );
void UART_OutNum(unsigned int );
void UART_OutString(char*);
void Clear_InpBuff();
void Debounce_Delay(int );
void SSD_init(void);
void LCD_init();
void LCD_disp(short , char );
void LCD_dispString(char *);
void SSD_disp();
void configure_systick();
static inline void enable_irq(void);
static inline void disable_irq(void);
void ADC_init();
void PWM_init();
void timer0_init();
void LED_init();
int unsigned state=0x00;
int unsigned color=0x00;//Color of LED
short all_on=0;//set when LED to be set to 1
short all_off=0;
short inp_done=0;//set when newline received from console
int unsigned delay=MAX_DELAY;
char console_cmd_buffer[50];
int length=0;
int char_cnt=0;
int Color_switch_count=0;//keep track of number of times color has been switched
short Pause=0;
short Stop=0;
long stop_watch_time=0;
int Timer_Status=0;
int Tick_Tack_board_status[3][3];
char Tick_Tack_board[9];
short game_status=0;
int Player_status=1;
int moves_count=0;
int timer_reload;
int pwm_value = 4999;
int disp_adc=0;
int adc_prev_value=0;
int disp_adc_count=0;
volatile int result;
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
char adc_value[10];//for debug purpose
int main(void)
{
    int Led_Color;
    disable_irq(); // Disable interrupts while configuring peripherals
   configure_systick();// Initialise systick to generate interrupt once every 50us
   ADC_init();
   PWM_init();
   UART0_init();
   LED_init();
   SSD_init();

   timer0_init();
   enable_irq();  // Enable interrupts
   timer_reload=TIMER_RELOAD_VALUE;
   Timer_Status=0;



   UART_OutChar('>');



while(1) {
    delay = pwm_value/50;
    if (pwm_value > 4950)
        all_off=1;
    else if (pwm_value < 2)
        all_on=1;
    else{
        all_on=0;
        all_off=0;
    }
//    itoa(pwm_value,adc_value,10);
//    UART_OutString(adc_value);
//    UART_OutString(" ");
    SSD_disp();
    if((!Pause)&&(!Stop)){
    switch(state){
              case 0x00:
                  Led_Color = COLOR_GREEN;

              break;

              case 0x01:
                  Led_Color = COLOR_BLUE;

              break;



              default:
                  state=0x00;
              break;
            }

    }
    if(!Stop && !all_off)
        GPIO_PORTF_DATA_WR = Led_Color;

   delayMs(delay);
   if(Timer_Status!=TIMER_STOP){
    LED_off();
   }


}
}

void configure_systick()
{
    NVIC_ST_CTRL_R      = 0;            // Disable timer during setup
    NVIC_ST_RELOAD_R    = timer_reload-1;       // This should expire once every 0.15625 ms
  //  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x0FFFFFFF) | 0x20000000; /*  priority 1 for SYSTICK*/
    NVIC_ST_CTRL_R      = 0x00000007;   // Clock src set to system clock and enable timer and interrupt

}



void ADC_init(){
    SYSCTL_RCGCADC_R |= 0x00000001; /* enable clock to ADC0 */
    SYSCTL_RCGCGPIO_R |= 0x10; /* enable clock to PE (AIN0 is on PE3) */



    ADC0_ACTSS_R |= 0x00000008; /* enable ADC0 sequencer 3 */
    ADC0_EMUX_R &= ~0xF000; /* software trigger conversion */
    ADC0_SSMUX3_R = 0; /* get input from channel 0 */
    /* initialize PE3 for AIN0 input */
    GPIO_PORTE_AFSEL_R |= 8; /* enable alternate function */
    GPIO_PORTE_DEN_R &= ~8; /* disable digital function */
    GPIO_PORTE_AMSEL_R |= 8; /* enable analog function */
    ADC0_SSCTL3_R |= 6; /* take one sample at a time, set flag at 1st sample */
    ADC0_ACTSS_R |= 8; /* enable ADC0 sequencer 3 */


    //motor direction control
    SYSCTL_RCGCGPIO_R |= 0x08;
    GPIO_PORTD_DIR_R |= (1<<3)|(1<<2);
    GPIO_PORTD_DEN_R |= (1<<3)|(1<<2);
    GPIO_PORTD_DATA_R |= (1<<3);
    GPIO_PORTD_DATA_R &= ~(1<<2);
    //GPIO_PORTA_DATA_R &= ~(1<<3);
    //GPIO_PORTA_DATA_R |= (1<<2);
}

void PWM_init(){

    // Enable Peripheral Clocks
    SYSCTL_RCGCPWM_R |= 1;       // Enable clock to PWM0
    SYSCTL_RCGCGPIO_R |= 0x00000010;        // Enable clock at GPIOE (for ADC0)
    SYSCTL_RCC_R |= 0x001E0000;  // Enable divisor for PWM clock (clk/64)
    // Enable port PE5 for PWM0 M0PWM5
    GPIO_PORTE_AFSEL_R = 0x20;   // PE5 uses alternate function
    GPIO_PORTE_PCTL_R &= ~0x00F00000; // Make PE5 PWM output pin
    GPIO_PORTE_PCTL_R |= 0x00400000;  // Function 4 of pin PE5 -> M0PWM5
    GPIO_PORTE_DEN_R |= 0x20;    // Digital enable for PE5
    PWM0_2_CTL_R = 0;            // Stop counter
    PWM0_2_GENB_R = 0x0000008C;  // M0PWM5 output set when reload
                                 // Clear when match PWMCMPA
    PWM0_2_LOAD_R = 5000;        // Set load value for 50Hz : ((16MHz/64) / 50 Hz) = 5000
    PWM0_2_CMPA_R = 4400;        // set duty cycle to 180 degree for servo
    PWM0_2_CTL_R = 1;            // start timer
    PWM0_ENABLE_R = 0x20;        // Start PWM0 ch5
}



void SSD_disp(){

    long timer_value;
    int i;
    long adc_value;
    GPIO_PORTA_DATA_R=0x00;
    if(disp_adc){
  //      UART_OutString("adc_disp");
        disp_adc_count++;
        adc_value=result;
  for(i=0;i<4;i++){

      GPIO_PORTA_DATA_R = 0xF0 & (0x10 << i);
      GPIO_PORTB_DATA_R = number[adc_value%10].state_value & 0x7F;
      adc_value=adc_value/10;
   Debounce_Delay(2);
   if(disp_adc_count==60){
       disp_adc=0;
       disp_adc_count=0;
   }
   }
    }
    else{

    timer_value=stop_watch_time;

         GPIO_PORTB_DATA_R = number[timer_value%10].state_value & 0x7F ;

         GPIO_PORTA_DATA_R = 0xF0 & 0x10;
         Debounce_Delay(1);


   for(i=0;i<3;i++){
       timer_value=timer_value/10;
       GPIO_PORTA_DATA_R = 0xF0 & (0x20 << i);
       if(i==0)
    GPIO_PORTB_DATA_R = number[timer_value%10].state_value |  DOT ;
       else
           GPIO_PORTB_DATA_R = number[timer_value%10].state_value;

    Debounce_Delay(1);
    }

    }

}


void GPIOPortF_Handler(void)
{

  if (GPIO_PORTF_MIS_R & 0x10) /* check if interrupt causes by PF4/SW1*/
    {
      if (Timer_Status==TIMER_RESUME){
      Timer_Status=TIMER_PAUSE;
      UART_OutString("Status: Timer Paused\n");
    //  LCD_dispString("Timer Pause");
      state=0x01;
      }
      else if(Timer_Status==TIMER_PAUSE){
          state=0x00;
          Timer_Status=TIMER_RESUME;
          UART_OutString("Status: Timer Running\n");
    //      LCD_dispString("Timer Running");
      }
      else if(Timer_Status!=TIMER_STOP){
          Timer_Status=TIMER_PAUSE;
          UART_OutString("Status:Timer Pause\n");
    //     LCD_dispString("Timer Pause");
          state=0x01;
      }
      while(GPIO_PORTF_DATA_R & 0x10 == 0x00);
      GPIO_PORTF_ICR_R |= 0x10; /* clear the interrupt flag */
     }
    else if (GPIO_PORTF_MIS_R & 0x01) /* check if interrupt causes by PF0/SW2 */
    {
        if (Timer_Status==TIMER_STOP){
        Timer_Status=TIMER_START;
        UART_OutString("Status: Timer Running\n");
    //    LCD_dispString("Timer Running");
        state=0x00;
        }
        else if(Timer_Status==TIMER_START){
            Timer_Status=TIMER_STOP;
            UART_OutString("Status: Timer Ready\n");
     //       LCD_dispString("Timer Ready");
            state=0x00;
        }
        else{
       Timer_Status=TIMER_STOP;
       UART_OutString("Status: Timer Ready\n");
     //  LCD_dispString("Timer Ready");
       state=0x00;
        }
        while(GPIO_PORTF_DATA_R & 0x01 == 0x00);
     GPIO_PORTF_ICR_R |= 0x01; /* clear the interrupt flag */
    }
  OutCRLF();
}

void timer0_init(){
    // Enable the clock for Timer0
        SYSCTL_RCGCTIMER_R |= (1U << 0);

        // Disable Timer0
        TIMER0_CTL_R &= ~(1U << 0);

        // Configure Timer0 as a 32-bit timer in periodic mode
        TIMER0_CFG_R = 0x00000000;  // 32-bit mode
        TIMER0_TAMR_R = 0x00000002; // Periodic mode

        // Set the Reload Value for a 500 milliseconds delay (assuming a 16MHz clock)
        TIMER0_TAILR_R = (16000000 / 2) - 1;

        // Enable Timer0 interrupt
        TIMER0_IMR_R |= (1U << 0);

        // Enable the NVIC interrupt for Timer0
        NVIC_EN0_R |= (1U << 19); // Enable interrupt for Timer0A

        // Enable Timer0
        TIMER0_CTL_R |= (1U << 0);
}

// Timer0A interrupt service routine
void Timer0A_ISR(void) {


           float temp;
           int temp2;

     //      Timer_Status=0;
    // Clear Timer0A timeout interrupt
    TIMER0_ICR_R |= (1U << 0);

   ADC0_PSSI_R |= 8; /* start a conversion sequence 3 */
   while((ADC0_RIS_R & 8) == 0)
  ; /* wait for conversion complete */
  result = ADC0_SSFIFO3_R; /* read conversion result */

  ADC0_ISC_R = 8; /* clear completion flag */
  temp=(((result)/4095.0)*4999.0);
  temp2=(int)temp;
  //    itoa(temp2,adc_value,10);
  //    UART_OutString(adc_value);
  //    UART_OutString(" ");
  pwm_value=4999-temp2;

  PWM0_2_CMPA_R=pwm_value;
  disp_adc=((abs(adc_prev_value - result) > 20));
  adc_prev_value=result;

}
void SysTick_Handler(void)
{


    if(Timer_Status==TIMER_STOP)
        stop_watch_time=0;


    else if((Timer_Status!=TIMER_PAUSE)&&(Timer_Status!=TIMER_STOP))
    stop_watch_time=stop_watch_time+1;

    if( stop_watch_time>9999)
        stop_watch_time=0;



}
void UART_cmpInp()
{
    short i=0,j=0;
  //  char cmd_string[10];
    char *cmd_string;
    unsigned int mem_addr;
    unsigned int num_bytes;
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
         if((console_cmd_buffer[i] >= 32 && console_cmd_buffer[i] < 127)||(console_cmd_buffer[i]==NULL)){
         console_cmd_buffer[j] = tolower(console_cmd_buffer[i]);
         j++;
         }
        i++;

    }

     cmd_string = strtok(console_cmd_buffer, " ");



    if(!(strcmp(cmd_string,"timer"))){
        cmd_string=strtok(NULL, " ");
        if(!(strcmp(cmd_string,"pause"))){
            Timer_Status=TIMER_PAUSE;
            UART_OutString("Timer paused\n");
        }
            else if(!(strcmp(cmd_string,"stop"))){
                Timer_Status=TIMER_STOP;
                UART_OutString("Timer Ready\n");

                state=0x00;
            }
            else{
                if(!(strcmp(cmd_string,"resume")))
                Timer_Status=TIMER_RESUME;
                if(!(strcmp(cmd_string,"start")))
                    Timer_Status=TIMER_START;
                if(!(strcmp(cmd_string,"set"))){
                    cmd_string = strtok(NULL, " ");
                    stop_watch_time=atoi(cmd_string);
                    UART_OutString(cmd_string);
                }
                UART_OutString("Timer Running\n");

                state=0x00;
            }

    }

    else{
        UART_OutString("Given command is invalid.\n\rCommand Help\n\rValid commands are,\
                  \n\r [1] Timer <timer status> ; Timer status should be either Pause, Resume, Stop or Start\n\r");
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
      SYSCTL_RCGC2_R |= 0x00000020 ;       /* Enable clock to GPIO_F_ at clock gating control register */

      SYSCTL_RCGCGPIO_R |= 0x00000020 ;    /* Enable and provide a clock to GPIO Port_F_ in Run mode */
      GPIO_PORTF_LOCK_R = 0x4C4F434B;//Unlock CR register
      GPIO_PORTF_CR_R |= 0x01;//Enable pull_up on PF0
      GPIO_PORTF_DEN_R |= GPIO_PORTF_PIN3_1_EN +GPIO_PORTF_PIN0_EN+GPIO_PORTF_PIN4_EN;
      GPIO_PORTF_DIR_R |= GPIO_PORTF_PIN3_1_EN;/* enable the GPIO pins for the LED (PF3, 2
  //1) as output */
     GPIO_PORTF_DIR_R &= (~(GPIO_PORTF_PIN0_EN+GPIO_PORTF_PIN4_EN));
     GPIO_PORTF_PUR_R |= GPIO_PORTF_PIN0_EN+GPIO_PORTF_PIN4_EN;//Pull-up PF4(sw1) and PF0(sw0)
     GPIO_PORTF_IS_R &= ~0x11;       /*  PF is edge-sensitive */
      GPIO_PORTF_IBE_R &= ~0x11;      /*  PF is not both edges */
      GPIO_PORTF_IEV_R &= ~0x11;      /*  PF falling edge event */
      GPIO_PORTF_ICR_R |= 0x11;        /*  Clear flag */
      GPIO_PORTF_IM_R |= 0x11;        /*  arm interrupt on PF */
      NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF1FFFFF) | 0x00A00000; /*  priority 5 for Port F*/
      NVIC_EN0_R = 0x40000000;        /*  Enable interrupt 30 in NVIC */
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

void UART_OutNum(unsigned int data){
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

    else if((length < max)&&(character >= 32 && character < 127)){//
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
  //  if(!Stop)
 //   switch_press();

 // if(isKeyPressed())
 //     Keypad_cmpInp();

   if(UARTCharsAvail(UART0_BASE))
        UART_cmpInp();
    if((i%3)==1)//Reduce freq of calling SSD_disp to reduce LED blink latency
    SSD_disp();
for(j = 0; j < 1000; j++) {} /* do nothing for 1 ms(run till 3180 for 1ms) */
}
}
void Debounce_Delay(int n)
{
    for(int i = 0 ; i < n; i++)
                for(int j = 0; j < 3180; j++) {} /* do nothing for 1 ms */
}
static inline void enable_irq()
{
    __asm ("    CPSIE  I\n");
}

static inline void disable_irq()
{
    __asm ("    CPSID  I\n");
}


