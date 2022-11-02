/***********************************************************************
 * 
 * Use USART unit and transmit data between ATmega328P and computer.
 * 
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2018 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/


/* Defines -----------------------------------------------------------*/
#ifndef F_CPU
# define F_CPU 16000000  // CPU frequency in Hz required for UART_BAUD_SELECT
#endif


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <uart.h>           // Peter Fleury's UART library
#include <stdlib.h>         // C library. Needed for number conversions


/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and transmit UART data four times 
 *           per second.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize USART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600, F_CPU));
    
    // Configure 16-bit Timer/Counter1 to transmit UART data
    // Set prescaler to 262 ms and enable overflow interrupt

    TIM1_overflow_262ms();
    TIM1_overflow_interrupt_enable();


    // Enables interrupts by setting the global interrupt mask
    sei();

    // Put strings to ringbuffer for transmitting via UART
    uart_puts("Print one line...\r\n");
    uart_puts("done\r\n");

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Transmit UART data four times per second.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    // Transmit UART string(s)
    // uart_puts("Paris\r\n");

    uint8_t value;
    uint8_t even=0;
    char string[8];  // String for converted numbers by itoa()

    value = uart_getc();
    if (value != '\0') {  // Data available from UART
        // Display ASCII code of received character
        uart_putc(value);
        itoa(value,string,10);
        uart_puts("\t");
        uart_puts(string);

        itoa(value,string,16);
        uart_puts("\t");
        uart_puts(string);

        itoa(value,string,2);
        uart_puts("\t0b ");
        uart_puts(string);

      //get the even parity

        // EVen = b0 xor b01 xor b02 xor ... b07
        // value = b1 b2 B3 b4 b5 B6 B7
        // cycle 8 times : 
        //    --extract just one bit : LSB
        //    --apply binary masl and logic AND 
        //    --even = even xor b0
        //    -- shift even to the right 

        for (uint8_t i = 0; i<8;i++){

          even = even ^ (value & 0x01);  // 0x01 is 0b0000_0001 which is our mask 
          value = value >> 1;
        }
          uart_puts("\tEven : ");
          itoa(value,string,10);
        uart_puts(string);
        uart_puts("\r\n");


    }
}