# LED Illuminator: Unified LED and RGB Control System with Button Interaction

## Project Overview
This project involves the development of a comprehensive LED control system using a microcontroller. The system is designed to understand GPIO features in datasheets and the super loop architecture. It combines the functionalities of LED blinking and RGB LED control into one integrated project.

## Components
The system consists of an STM32 microcontroller, a single push button and two types of LEDs: a standard LED and an RGB LED. The standard LED is connected to PORTA pin 0, while the RGB LED pins are connected to port B pins 5, 6, and 7. The push button, which is active low without an external pull-up resistor, is connected to PORTB pin 3.

## Functionality
1. **LED Blinking**: The system performs LED blinking through a main function that initializes the port to control the LED. This function contains an infinite loop that sets the LED pin to high, waits for a predefined time, sets the LED pin to low, waits for another predefined time, and then repeats the process.

2. **RGB LED Control**: The system also scans the push button periodically. Upon each valid button press, detected by a falling edge, the software changes the RGB LED color. A button debouncing feature is included to ensure accurate color changes. The RGB LED provides 8 colors controlled by the enabled color mix (RED + GREEN + BLUE).

## Skills Used
- C Language
- ARM Architecture