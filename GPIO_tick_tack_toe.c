/*
 * GPIO.c
 *
 *  Created on: 06-Feb-2024
 *      Author: Kevin
 */
#include<stdio.h>
#include "C:\ti\TivaWare_C_Series-2.1.4.178\inc\tm4c123gh6pm.h"
#include "GPIO.h"

void GPIOA_INIT(void)
{
    SYSCTL_RCGC2_R |= 0x00000001 ;       /* Enable clock to GPIO_A_ at clock gating control register */

    SYSCTL_RCGCGPIO_R |= 0x00000001 ;    /* Enable and provide a clock to GPIO Port_A_ in Run mode */

    while((SYSCTL_PRGPIO_R & 0x01) == 0){}; // Wait for clock to stabilize


//    GPIO_PORTA_DIR_R &= ~ 0x000000E1 ;

    GPIO_PORTA_LOCK_R = 0x4C4F434B;     /* unlock commit register */

    GPIO_PORTA_DIR_R |= 0x000000C0 ;    /*  GPIO Direction | 0 -> INPUT | 1 -> OUTPUT */

    GPIO_PORTA_DEN_R |= 0x000000C0 ;

    GPIO_PORTA_AFSEL_R |= 0x00000000 ;  /* Disable alternate function on PORT */

    GPIO_PORTA_AMSEL_R |= 0x00000000 ;  /* Disable analog function on PORT */

    GPIO_PORTA_PCTL_R &= ~ 0xFFFFFF00 ; /* Regular digital function */

    /* DRIVE STRENGTH of PORT A */

//    GPIO_PORTA_DR2R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 2-mA drive */
//
//    GPIO_PORTA_DR4R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 4-mA drive */
//
//    GPIO_PORTA_DR8R_R |= 0x000000F0 ;   /* 1 -> The corresponding GPIO pin has 8-mA drive */
//
//    GPIO_PORTA_ODR_R |= 0x00000008 ;    /* 1 -> The corresponding pin is configured as open drain */
//
//    GPIO_PORTA_PUR_R |= 0x00000000 ;    /* 1 -> The corresponding pin's weak pull-up resistor is enabled */
//
//    GPIO_PORTA_PDR_R |= 0x000000F0 ;    /* 1 -> The corresponding pin's weak pull-down resistor is enabled */

    GPIO_PORTA_DEN_R |= 0xC0;       /* enable the GPIO pins for digital function */

    GPIO_PORTA_DATA_R |= 0x00000000 ;

    /* INTERRUPTS on PORT A */

//    EnableInterrupts() ;             /* Enable global Interrupt flag (I) */
//
//    NVIC_PRI0_R |= (NVIC_PRI0_R & 0xFFFFFF1F) | 0x000000A0 ; /*  priority 5 */
//
//    NVIC_EN0_R |= 0x01 ;        /*  Enable interrupt 16 in NVIC */
//
//
//    GPIO_PORTA_IS_R &= ~0xFF ;      /* 0 -> The edge on the corresponding pin is detected (edge-sensitive)
//                                       1 -> The level on the corresponding pin is detected (level-sensitive)*/
//
//    GPIO_PORTA_IBE_R &= ~0xFF ;     /* 0 -> Interrupt generation is controlled by the GPIO Interrupt Event (GPIOIEV) register (see page 666).
//                                       1 -> Both edges on the corresponding pin trigger an interrupt */
////    GPIO_PORTA_IBE_R|= 0x08 ;
//
//    GPIO_PORTA_IEV_R &= 0xFF ;      /* 0 -> A falling edge or a Low level on the corresponding pin triggers an interrupt.
//                                       1 -> A rising edge or a High level on the corresponding pin triggers an interrupt */
//    GPIO_PORTA_IEV_R |= 0xE0 ;
//
//    GPIO_PORTA_IM_R |= 0xE0 ;       /* 0 -> The interrupt from the corresponding pin is masked.
//                                       1 -> The interrupt from the corresponding pin is sent to the interrupt controller */
//
//    GPIO_PORTA_ICR_R |= 0xFF ;      /* 0 -> The corresponding interrupt is unaffected.
//                                       1 -> The corresponding interrupt is cleared */
}


void GPIOB_INIT(void)
{
    SYSCTL_RCGC2_R |= 0x00000002 ;       /* Enable clock to GPIO_B_ at clock gating control register */

    SYSCTL_RCGCGPIO_R |= 0x00000002 ;    /* Enable and provide a clock to GPIO Port_B_ in Run mode */

    while((SYSCTL_PRGPIO_R & 0x02) == 0){}; // Wait for clock to stabilize

    GPIO_PORTB_DIR_R |= 0x000000FF ;    /*  GPIO Direction | 0 -> INPUT | 1 -> OUTPUT */

//    GPIO_PORTB_DIR_R &= ~0xE0 ;

    GPIO_PORTB_LOCK_R = 0x4C4F434B;     /* unlock commit register */

    GPIO_PORTB_AFSEL_R &= ~ 0xFFFFFFFC ;  /* Disable alternate function on PORT */

    GPIO_PORTB_AMSEL_R &= ~ 0xFFFFFFFF ;  /* Disable analog function on PORT */

    GPIO_PORTB_PCTL_R &= ~ 0xFFFFFF00 ; /* Regular digital function */

    /* DRIVE STRENGTH of PORT A */

//    GPIO_PORTB_DR2R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 2-mA drive */
//
//    GPIO_PORTB_DR4R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 4-mA drive */
//
//    GPIO_PORTB_DR8R_R |= 0x00000FC ;   /* 1 -> The corresponding GPIO pin has 8-mA drive */
//
//    GPIO_PORTB_ODR_R |= 0x00000000 ;    /* 1 -> The corresponding pin is configured as open drain */
//
//    GPIO_PORTB_PUR_R |= 0x00000000 ;    /* 1 -> The corresponding pin's weak pull-up resistor is enabled */

//    GPIO_PORTB_PDR_R |= 0x000000E0 ;    /* 1 -> The corresponding pin's weak pull-down resistor is enabled */

    GPIO_PORTB_DEN_R |= 0xFF;       /* enable the GPIO pins for digital function */

    GPIO_PORTB_DATA_R |= 0x00000000 ;

    /* INTERRUPTS on PORT A */

//    EnableInterrupts() ;             /* Enable global Interrupt flag (I) */
//
//    NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFF1FFF) | 0x00008000 ; /*  priority 5 */
//
//    NVIC_EN0_R |= 0x00000002 ;        /*  Enable interrupt 16 in NVIC */
//
//
//    GPIO_PORTB_IS_R &= ~0xA0 ;      /* 0 -> The edge on the corresponding pin is detected (edge-sensitive)
//                                       1 -> The level on the corresponding pin is detected (level-sensitive)*/
//
//    GPIO_PORTB_IBE_R &= ~0xA0 ;     /* 0 -> Interrupt generation is controlled by the GPIO Interrupt Event (GPIOIEV) register (see page 666).
//                                       1 -> Both edges on the corresponding pin trigger an interrupt */
//
//    GPIO_PORTB_IEV_R &= ~0xA0 ;     /* 0 -> A falling edge or a Low level on the corresponding pin triggers an interrupt.
//                                       1 -> A rising edge or a High level on the corresponding pin triggers an interrupt */
//
//    GPIO_PORTB_IM_R |= 0x80 ;       /* 0 -> The interrupt from the corresponding pin is masked.
//                                       1 -> The interrupt from the corresponding pin is sent to the interrupt controller */
//
//    GPIO_PORTB_ICR_R |= 0xA0 ;      /* 0 -> The corresponding interrupt is unaffected.
//                                       1 -> The corresponding interrupt is cleared */
}
void GPIOC_INIT(void)
{
    /* Do no use PC[3:0] */

//    SYSCTL_RCGCGPIO_R |= 0x34;      // Enable clock to GPIO PORTC, PORTE and PORTF
//    GPIO_PORTC_DIR_R |= 0x00;       // Set PC 4-7 as input
//    GPIO_PORTC_DEN_R |= 0xF0;       // Digital Enable PC 4-7
//    GPIO_PORTC_PUR_R |= 0xF0;       // Enable Pull Up Resistors for input PORTC 4-7

    SYSCTL_RCGC2_R |= 0x00000004 ;       /* Enable clock to GPIO_C_ at clock gating control register */

    SYSCTL_RCGCGPIO_R |= 0x00000004 ;    /* Enable and provide a clock to GPIO Port_C_ in Run mode */

//    GPIO_PORTC_CR_R |= 0xFFFFFFFF ;     /* 1 -> The corresponding GPIOAFSEL, GPIOPUR, GPIOPDR, or GPIODEN bits can be written */

//    GPIO_PORTC_LOCK_R &= ~ 0x4C4F434B;     /* unlock commit register */

    GPIO_PORTC_DIR_R &= ~ 0xF0 ;    /*  GPIO Direction | 0 -> INPUT | 1 -> OUTPUT */

//    GPIO_PORTC_LOCK_R = 0x4C4F434B;     /* unlock commit register */

//    GPIO_PORTC_AFSEL_R |= 0x00 ;  /* Disable alternate function on PORT */

//    GPIO_PORTC_AMSEL_R |= 0x00 ;  /* Disable analog function on PORT */

//    GPIO_PORTC_PCTL_R &= ~ 0xF0 ; /* Regular digital function */

    /* DRIVE STRENGTH of PORT A */

//    GPIO_PORTC_DR2R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 2-mA drive */
//
//    GPIO_PORTC_DR4R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 4-mA drive */
//
//    GPIO_PORTC_DR8R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 8-mA drive */
//
    GPIO_PORTC_ODR_R |= 0xF0 ;    /* 1 -> The corresponding pin is configured as open drain */

    GPIO_PORTC_DEN_R |= 0xF0;       /* enable the GPIO pins for digital function */

    GPIO_PORTC_PUR_R |= 0xF0 ;    /* 1 -> The corresponding pin's weak pull-up resistor is enabled */
//
//    GPIO_PORTC_PDR_R |= 0x00 ;    /* 1 -> The corresponding pin's weak pull-down resistor is enabled */

    /* INTERRUPTS on PORT C */

    NVIC_PRI0_R |= (NVIC_PRI0_R & 0xFF1FFFFF) | 0x00800000 ; /*  priority 5 */
//
    NVIC_EN0_R |= 0x00000004;        /*  Enable interrupt 2 in NVIC */
//
    GPIO_PORTC_IS_R &= ~0xF0 ;      /* 0 -> The edge on the corresponding pin is detected (edge-sensitive)
//                                       1 -> The level on the corresponding pin is detected (level-sensitive)*/
//
    GPIO_PORTC_IBE_R &= ~0xF0 ;     /* 0 -> Interrupt generation is controlled by the GPIO Interrupt Event (GPIOIEV) register (see page 666).
//                                       1 -> Both edges on the corresponding pin trigger an interrupt */
//
    GPIO_PORTC_IEV_R &= ~0xF0 ;     /* 0 -> A falling edge or a Low level on the corresponding pin triggers an interrupt.
//                                       1 -> A rising edge or a High level on the corresponding pin triggers an interrupt */
//
    GPIO_PORTC_IM_R |= 0xF0 ;       /* 0 -> The interrupt from the corresponding pin is masked.
//                                       1 -> The interrupt from the corresponding pin is sent to the interrupt controller */
//
    GPIO_PORTC_ICR_R |= 0xF0 ;      /* 0 -> The corresponding interrupt is unaffected.
//                                       1 -> The corresponding interrupt is cleared */
}

void GPIOE_INIT(void)
{
    SYSCTL_RCGC2_R |= 0x00000010 ;       /* Enable clock to GPIO_E_ at clock gating control register */

    SYSCTL_RCGCGPIO_R |= 0x00000010 ;    /* Enable and provide a clock to GPIO Port_E_ in Run mode */

    GPIO_PORTE_DIR_R |= 0x0F ;    /*  GPIO Direction | 0 -> INPUT | 1 -> OUTPUT */

//    GPIO_PORTE_AFSEL_R &= ~ 0x0F ;  /* Disable alternate function on PORT */
//
//    GPIO_PORTE_AMSEL_R &= ~ 0x0F ;  /* Disable analog function on PORT */
//
//    GPIO_PORTE_PCTL_R &= ~ 0x0F ; /* Regular digital function */

    /* DRIVE STRENGTH of PORT A */

//    GPIO_PORTE_DR2R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 2-mA drive */
//
//    GPIO_PORTE_DR4R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 4-mA drive */
//
//    GPIO_PORTE_DR8R_R |= 0x00000000 ;   /* 1 -> The corresponding GPIO pin has 8-mA drive */
//
//    GPIO_PORTE_ODR_R |= 0x00000000 ;    /* 1 -> The corresponding pin is configured as open drain */
//
//    GPIO_PORTE_PUR_R |= 0x00000000 ;    /* 1 -> The corresponding pin's weak pull-up resistor is enabled */
//
//    GPIO_PORTE_PDR_R |= 0x00000000 ;    /* 1 -> The corresponding pin's weak pull-down resistor is enabled */

    GPIO_PORTE_DEN_R |= 0x0F;       /* enable the GPIO pins for digital function */

    GPIO_PORTE_DATA_R = 0x00 ;

    /* INTERRUPTS on PORT E */

//    EnableInterrupts() ;             /* Enable global Interrupt flag (I) */
//
 //   NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFFFF1F) | 0x0000000C ; /*  priority 6 */
//
 //   NVIC_EN0_R |= (INT_GPIOE - 15) ;        /*  Enable interrupt 16 in NVIC */
//
//
//    GPIO_PORTE_IS_R &= ~0xFF ;      /* 0 -> The edge on the corresponding pin is detected (edge-sensitive)
//                                       1 -> The level on the corresponding pin is detected (level-sensitive)*/
//
//    GPIO_PORTE_IBE_R &= ~0xFF ;     /* 0 -> Interrupt generation is controlled by the GPIO Interrupt Event (GPIOIEV) register (see page 666).
//                                       1 -> Both edges on the corresponding pin trigger an interrupt */
//
//    GPIO_PORTE_IEV_R &= 0xF0 ;     /* 0 -> A falling edge or a Low level on the corresponding pin triggers an interrupt.
//                                       1 -> A rising edge or a High level on the corresponding pin triggers an interrupt */
//
//    GPIO_PORTE_IM_R |= 0x0F ;       /* 0 -> The interrupt from the corresponding pin is masked.
//                                       1 -> The interrupt from the corresponding pin is sent to the interrupt controller */
//
//    GPIO_PORTE_ICR_R |= 0x11 ;      /* 0 -> The corresponding interrupt is unaffected.
//                                       1 -> The corresponding interrupt is cleared */
}



