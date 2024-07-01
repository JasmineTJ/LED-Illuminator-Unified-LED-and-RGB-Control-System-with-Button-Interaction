/*
 * main.c
 *
 *  Created on: Apr 7, 2024
 *      Author: Salma Ashraf and Yassmeen Tarek
 */

#include <stdio.h>

// Define base addresses for peripheral registers
#define RCC_BASE        0x40023800
#define GPIOB_BASE      0x40020400
#define EXTI_BASE       0x40013C00

// Define pointers to peripheral registers
#define RCC_AHB1ENR     (*(unsigned int*)(RCC_BASE + 0x30))
#define GPIOB_MODER     (*(unsigned int*)(GPIOB_BASE + 0x00))
#define GPIOB_PUPDR     (*(unsigned int*)(GPIOB_BASE + 0x0C))
#define GPIOB_ODR       (*(unsigned int*)(GPIOB_BASE + 0x14))
#define GPIOB_IDR       (*(unsigned int*)(GPIOB_BASE + 0x10))
#define EXTI_IMR        (*(unsigned int*)(EXTI_BASE + 0x00))
#define EXTI_FTSR       (*(unsigned int*)(EXTI_BASE + 0x0C))
#define EXTI_PR         (*(unsigned int*)(EXTI_BASE + 0x14))
#define NVIC_ISER0      (*(unsigned int*)0xE000E100)

// Define pin numbers
#define BUTTON_PIN      3
#define RED_LED_PIN     5
#define GREEN_LED_PIN   6
#define BLUE_LED_PIN    7

// Define debounce count
#define DEBOUNCE_COUNT  10

// Define global variables
volatile int debounceCounter = 0;
static uint32_t lastButtonState = 1;  // Assume button is initially not pressed
volatile uint32_t ledColorState = 8;  // Initial LED color state

// Interrupt handler for EXTI line 3
void EXTI3_IRQHandler(void) {
    // Check if interrupt flag for line 3 is set
    if (EXTI_PR & (1 << BUTTON_PIN)) {
        // Clear interrupt flag for line 3
        EXTI_PR |= (1 << BUTTON_PIN);
        // Increment debounce counter
        debounceCounter++;
    }
}

int main() {
    // Enable clock for GPIOB
    RCC_AHB1ENR |= (1 << 1);  // Set the second bit of RCC_AHB1ENR to 1 to enable the clock

    // Configure button pin (PB3) as input (no pull-up resistor)
    GPIOB_MODER &= ~(3 << (BUTTON_PIN * 2));  // Clear the mode bits for the button pin
    GPIOB_PUPDR &= ~(3 << (BUTTON_PIN * 2));  // Clear the pull-up/pull-down bits for the button pin

    // Configure LED pins (PB5, PB6, PB7) as outputs
    GPIOB_MODER |= (1 << (RED_LED_PIN * 2)) | (1 << (GREEN_LED_PIN * 2)) | (1 << (BLUE_LED_PIN * 2));  // Set the mode bits for the LED pins to output

    // Enable interrupt on button pin (falling edge trigger)
    EXTI_IMR |= (1 << BUTTON_PIN);  // Set the interrupt mask for the button pin
    EXTI_FTSR |= (1 << BUTTON_PIN);  // Set the falling trigger selection for the button pin

    // Enable interrupt vector in NVIC
    NVIC_ISER0 |= (1 << 9);  // Set the 10th bit of NVIC_ISER0 to enable the EXTI line3 interrupt

    // Main loop
    while (1) {
        // Check for debounced button press
        if (debounceCounter >= DEBOUNCE_COUNT) {  // If the debounce counter has reached the threshold
            debounceCounter = 0;  // Reset the debounce counter
            ledColorState++;  // Increment the LED color state
            if (ledColorState > 8) {  // If the LED color state exceeds the maximum
                ledColorState = 1;  // Reset the LED color state
            }
        }

        // Read current button state
        uint32_t currentButtonState = GPIOB_IDR & (1 << BUTTON_PIN);  // Read the button state into currentButtonState

        // Check for falling edge on button pin
        if (lastButtonState && !currentButtonState) {  // If a falling edge is detected on the button pin
            ledColorState++;  // Increment the LED color state
            if (ledColorState > 8) {  // If the LED color state exceeds the maximum
                ledColorState = 1;  // Reset the LED color state
            }
        }

        // Update last button state
        lastButtonState = currentButtonState;  // Update lastButtonState with the current button state

        // Update LED color based on ledColorState
        switch (ledColorState) {  // Switch on the LED color state
            case 1: // Red
                GPIOB_ODR = (1 << RED_LED_PIN);  // Turn on the red LED
                break;
            case 2: // Green
                GPIOB_ODR = (1 << GREEN_LED_PIN);  // Turn on the green LED
                break;
            case 3: // Blue
                GPIOB_ODR = (1 << BLUE_LED_PIN);  // Turn on the blue LED
                break;
            case 4: // Yellow (Red + Green)
                GPIOB_ODR = (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN);  // Turn on the red and green LEDs
                break;
            case 5: // Magenta (Red + Blue)
                GPIOB_ODR = (1 << RED_LED_PIN) | (1 << BLUE_LED_PIN);  // Turn on the red and blue LEDs
                break;
            case 6: // Cyan (Green + Blue)
                GPIOB_ODR = (1 << GREEN_LED_PIN) | (1 << BLUE_LED_PIN);  // Turn on the green and blue LEDs
                break;
            case 7: // White (Red + Green + Blue)
                GPIOB_ODR = (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN) | (1 << BLUE_LED_PIN);  // Turn on all LEDs
                break;
            case 8: // All LEDs off
                GPIOB_ODR = 0;  // Turn off all LEDs
                break;
            default:
                ledColorState = 8;  // Reset the LED color state if it's an invalid value
                break;
        }
    }

    return 0;  // End of main function
}
