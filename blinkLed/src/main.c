/*
 * main.c
 *
 *  Created on: Apr 7, 2024
 *      Author: Salma Ashraf and Yassmeen Tarek
 */

// Include the device header file for the STM32F4xx series of microcontrollers
#include "stm32f4xx.h"

// Define the base address for GPIOA
#define GPIOA_BASE_ADDRESS 	0x40020000

// Define the registers for GPIOA using their offsets from the base address
#define GPIOA_MODER 		(*(unsigned long *)(GPIOA_BASE_ADDRESS + 0x00)) // GPIO port mode register
#define GPIOA_OTYPER 		(*(unsigned long *)(GPIOA_BASE_ADDRESS + 0x04)) // GPIO output type register
#define GPIOA_ODR			(*(unsigned long *)(GPIOA_BASE_ADDRESS + 0x14)) // GPIO output data register

// Define the base address for RCC (Reset and Clock Control)
#define RCC_BASE_ADDRESS	0x40023800

// Define the RCC_AHB1ENR (AHB1 peripheral clock enable register) using its offset from the base address
#define RCC_AHB1ENR			(*(unsigned long *)(RCC_BASE_ADDRESS + 0x30))

int main(){
	// Enable the clock for GPIO port A
	RCC_AHB1ENR |= (1);					// Set bit 0 of the RCC_AHB1ENR register to enable the clock for GPIOA

	// Configure GPIOA pin 0 as a general-purpose output
	unsigned char gpioa_pin_mode = 1; 	// Set the mode to 01 (general-purpose output)
	GPIOA_MODER &= ~(0x03);				// Clear the mode bits for pin 0
	GPIOA_MODER |= gpioa_pin_mode;		// Set the mode bits for pin 0 to the desired mode

	// Configure GPIOA pin 0 as a push-pull output
	GPIOA_OTYPER &= ~(0x01);			// Clear the output type bit for pin 0 to set it to push-pull

	// Main loop
	while (1){
		// Set GPIOA pin 0 to high (turn on the LED)
		GPIOA_ODR|= (0x01);

		// Delay loop for keeping the LED on
		for (volatile uint32_t i = 0 ; i < 2000000 ; i++);

		// Set GPIOA pin 0 to low (turn off the LED)
		GPIOA_ODR &= ~(0x01);

		// Delay loop for keeping the LED off
		for (volatile uint32_t i = 0 ; i < 2000000 ; i++);
	}

	return 0;
}
