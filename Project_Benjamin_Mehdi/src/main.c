/***********************************************************************
 * 
 * Stopwatch by Timer/Counter2 on the Liquid Crystal Display (LCD)
 *
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2017 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 * Components:
 *   16x2 character LCD with parallel interface
 *     VSS  - GND (Power supply ground)
 *     VDD  - +5V (Positive power supply)
 *     Vo   - (Contrast)
 *     RS   - PB0 (Register Select: High for Data transfer, Low for Instruction transfer)
 *     RW   - GND (Read/Write signal: High for Read mode, Low for Write mode)
 *     E    - PB1 (Read/Write Enable: High for Read, falling edge writes data to LCD)
 *     D3:0 - NC (Data bits 3..0, Not Connected)
 *     D4   - PD4 (Data bit 4)
 *     D5   - PD5 (Data bit 5)
 *     D6   - PD6 (Data bit 3)
 *     D7   - PD7 (Data bit 2)
 *     A+K  - Back-light enabled/disabled by PB2
 * 
 **********************************************************************/
////////////////////////////////////////////////////////////////////////////////////
#define JOY_PUSH PB2  //Encoder push button definition

////////////////////////////////////////////////////////////////////////////////////
/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Update stopwatch value on LCD screen when 8-bit 
 *           Timer/Counter2 overflows.
 * Returns:  none
 **********************************************************************/
int main(void)
{   
    // Initialize display
    lcd_init(LCD_DISP_ON);
////////////////////////////////////////////////////////////////////////////////////
    GPIO_mode_input_pullup(&DDRB, JOY_PUSH);

////////////////////////////////////////////////////////////////////////////////////

    lcd_gotoxy(1, 0); lcd_puts("Y:");
    lcd_gotoxy(1, 1); lcd_puts("X:");


    lcd_command(1<<LCD_CGRAM);       // Set addressing to CGRAM (Character Generator RAM)
                                     // ie to individual lines of character patterns
    /*for (uint8_t i = 0; i < 8; i++)  // Copy new character patterns line by line to CGRAM
        lcd_data(customChar[i]);
    */lcd_command(1<<LCD_DDRAM);       // Set addressing back to DDRAM (Display Data RAM)
                                     // ie to character codes
   
    GPIO_mode_output(&DDRB, PB2);
    GPIO_write_high(&PORTB,PB2);

    // Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
    ADMUX = ADMUX | (1<<REFS0);
    // Select input channel ADC0 (voltage divider pin)
    ADMUX = ADMUX & ~(1<<MUX3 | 1<<MUX2 | 1<<MUX1 | 1<<MUX0); //here we are clearing the four bits
    // Enable ADC module
    ADCSRA = ADCSRA | (1<<ADEN);
    // Enable conversion complete interrupt
    ADCSRA = ADCSRA | (1<<ADIE);
    // Set clock prescaler to 128
    ADCSRA = ADCSRA | (1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0);
    // Configuration of 8-bit Timer/Counter2 for Stopwatch update
    // Set the overflow prescaler to 16 ms and enable interrupt
    TIM1_overflow_33ms();
    TIM1_overflow_interrupt_enable();

    TIM2_overflow_4ms();
    TIM2_overflow_interrupt_enable();
    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines, ISRs */
    }

    // Will never reach this
    return 0;
}


/* Interrupt service routines ----------------------------------------*/

/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Use single conversion mode and start conversion every 100 ms.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    // Start ADC conversion
    ADCSRA = ADCSRA | (1<<ADSC);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Interrupt service routines ----------------------------------------*/
/**********************************************************************************
 * Function: Timer/Counter2 overflow interrupt
 * Purpose: Every 4ms, read encoder's and push buttons' values and display them
 *          on the LCD screen according to application.
 *          Timer has been set to 4ms for one reason: Time of pulse of the encoder
 *          is estimated 10ms. If a Timer is used which overflows in more than 10ms,
 *          we can lose changes of state of the encoder.
 *********************************************************************************/
ISR(TIMER2_OVF_vect)
{

      //Encoder definitions
    static uint8_t aLastState = 0;
    static uint8_t aState=0;
    static uint8_t bState=0;

    uint8_t Push_Joystick;

    Push_Joystick = GPIO_read(&PINB, JOY_PUSH);

     if (Push_Joystick == 0){
      lcd_clrscr();
      lcd_gotoxy(1,6);
      lcd_puts("BUZZER");
     }
/*
     //Loop to know in which direction encoder is turning
    aState = GPIO_read(&PINB, ENCODER_OA);
     // If the previous and the current state of the outputA are different, that means a pulse has occured.
    if(aLastState != aState){
        lcd_clrscr();
        bState = GPIO_read(&PINB, ENCODER_OB);
      
        // Let's compare a and b states
        if(aState != bState){
          //If a and b states are different, this means encoder is rotating clockwise.
          lcd_gotoxy(7, 0);
          lcd_puts("Accelerate");
        }else{
          //Encoder is rotating counter clockwise.
          lcd_gotoxy(7, 0);
          lcd_puts("Deccelerate");   
        }
        aLastState = aState;
      }  
      
    }
    */
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display converted value on LCD screen.
 **********************************************************************/
ISR(ADC_vect)
{
    uint16_t value;
    
    char string[4];  // String for converted numbers by itoa()
    static int channel = 0;
    value = ADC;

  if (channel==1){
    channel =0;
  }else{
    channel=1;
  }

    switch (channel)
    {
    case 1:
      ADMUX = ADMUX | 1<<MUX0;

      itoa(value, string, 10);
      lcd_gotoxy(3,0);
      lcd_puts("    ");
      lcd_gotoxy(3,0);
      lcd_puts(string);

        if( value > 530){
        lcd_gotoxy(7, 1);
        lcd_puts("      ");
        lcd_gotoxy(7, 1);
        lcd_puts("DOWN");
    }
    if (value <530 && value>500){
        lcd_gotoxy(7, 1);
        lcd_puts("      ");
        lcd_gotoxy(7, 1);
        lcd_puts("CENTER");
    }
    if( value < 500){
        lcd_gotoxy(7, 1);
        lcd_puts("      ");
        lcd_gotoxy(7, 1);
        lcd_puts("UP");
    }
      break;
    case 0:
      ADMUX = ADMUX & ~(1<<MUX3 | 1<<MUX2 | 1<<MUX1 | 1<<MUX0);

      itoa(value, string, 10);
      lcd_gotoxy(3,1);
      lcd_puts("    ");
      lcd_gotoxy(3,1);
      lcd_puts(string);

        if( value > 505){
        lcd_gotoxy(13, 1);
        lcd_puts("      ");
        lcd_gotoxy(13, 1);
        lcd_puts("-L");
    }
    if( value < 480){
        lcd_gotoxy(13, 1);
        lcd_puts("      ");
        lcd_gotoxy(13, 1);
        lcd_puts("-R");
    }
    if ( value < 520 && value > 500){
        lcd_gotoxy(13, 1);
        lcd_puts("      ");
    }
    break;
    }

    
    // Read converted value
    // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
    
    // Convert "value" to "string" and display it







}