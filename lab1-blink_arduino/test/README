# Lab 1: MEHDI BENNANI

### Morse code

1. Listing of C code which repeats one "dot" and one "comma" (BTW, in Morse code it is letter `A`) on a LED. Always use syntax highlighting, meaningful comments, and follow C guidelines:

```c
#define LED_GREEN PB5   // PB5 is AVR pin where green on-board LED is connected
#define PB5 13                        
#define SHORT_DELAY 250     // Delay in millisecond for a dot, or the delay between two signals
#define LONG_DELAY 750   // Delay in millisecond for a comma, or the delay between two characters
#ifndef F_CPU
# define F_CPU 16000000 // CPU frequency in Hz required for delay funcs
#endif

int main(void)
{
    // Set pin where on-board LED is connected as output
    pinMode(LED_GREEN, OUTPUT);

    // Infinite loop
    while (1)
    {
        // Generate a letter `A` Morse code

        // dot
        digitalWrite(LED_GREEN, HIGH);
        _delay_ms(SHORT_DELAY);

        // delay between dot and comma
        digitalWrite(LED_GREEN, LOW);
        _delay_ms(SHORT_DELAY);

        // comma
        digitalWrite(LED_GREEN, HIGH);
        _delay_ms(LONG_DELAY);

        // delay before the next A
        digitalWrite(LED_GREEN, LOW);
        _delay_ms(LONG_DELAY);

    }

    // Will never reach this
    return 0;
}
```

2. Scheme of Morse code application, i.e. connection of AVR device, LED, resistor, and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![your figure](./Shematics.png)
