/**
 * @file display1602_lib.cpp
 * @author Denys Khmil
 * @brief This file contents display1602 class and functions declarations
 */
#ifndef DISPLAY1602_LIB
#define DISPLAY1602_LIB


#include "main.h"


typedef struct{
	GPIO_TypeDef *dataPorts[4];
	GPIO_TypeDef *rsPort;
	GPIO_TypeDef *ePort;
}LCDports;

typedef struct{
	uint16_t dataPins[4];
	uint16_t rsPin;
	uint16_t ePin;
}LCDpins;



class display1602 {
public:

	display1602(LCDports _ports ,LCDpins _pins);
	void clear();
	void set_cursor(uint8_t x, uint8_t y);
	void print(uint8_t data);
	void printString(const char* data);
	void diplay_cusor(uint8_t state);
	void display_blink(uint8_t state);

private:
	uint8_t settings;

	LCDpins pins;
	LCDports ports;

	static uint8_t gpio_rcc_clk;

	void writeHalfByte(uint8_t command, uint8_t data);
	void writeByte(uint8_t command, uint8_t data);
	void displayInit();

};

#endif
