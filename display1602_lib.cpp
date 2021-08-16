/**
 * @file display1602_lib.cpp
 * @author Denys Khmil
 * @brief This file contents all the display1602 library functions
 */

#include "display1602_lib.h"

/**
 * @brief display1602 constructor
 * @note 	Creates display1602 instance.
 * 			Constructor sets pins and ports working mode, so no need to preset.
 * @param _ports: LCDports variable with all all the ports specified.
 * @param _pins: LCDpins variable with all the pins specified.
 */
display1602::display1602(LCDports _ports ,LCDpins _pins){
	/*Save ports and pins*/
	ports = _ports;
	pins = _pins;

	settings = 0;
	static uint8_t gpio_rcc_clk = 0;

	for ( uint8_t i = 0;  i < 4; i++) {
		if((ports.dataPorts[i] == GPIOA)&!(gpio_rcc_clk&(1<<0))){
			__HAL_RCC_GPIOA_CLK_ENABLE();
			gpio_rcc_clk = gpio_rcc_clk|(1<<0);
		}
		if((ports.dataPorts[i] == GPIOB)&!(gpio_rcc_clk&(1<<1))){
			__HAL_RCC_GPIOB_CLK_ENABLE();
			gpio_rcc_clk = gpio_rcc_clk|(1<<1);
		}
		if((ports.dataPorts[i] == GPIOC)&!(gpio_rcc_clk&(1<<2))){
			__HAL_RCC_GPIOC_CLK_ENABLE();
			gpio_rcc_clk = gpio_rcc_clk|(1<<2);
		}
		if((ports.dataPorts[i] == GPIOD)&!(gpio_rcc_clk&(1<<3))){
			__HAL_RCC_GPIOD_CLK_ENABLE();
			gpio_rcc_clk = gpio_rcc_clk|(1<<3);
		}


		if((ports.rsPort == GPIOA)&!(gpio_rcc_clk&(1<<0))){
			__HAL_RCC_GPIOA_CLK_ENABLE();
			gpio_rcc_clk = gpio_rcc_clk|(1<<0);
		}
		if((ports.rsPort == GPIOB)&!(gpio_rcc_clk&(1<<1))){
			__HAL_RCC_GPIOB_CLK_ENABLE();
			gpio_rcc_clk = gpio_rcc_clk|(1<<1);
		}
		if((ports.rsPort == GPIOC)&!(gpio_rcc_clk&(1<<2))){
			__HAL_RCC_GPIOC_CLK_ENABLE();
			gpio_rcc_clk = gpio_rcc_clk|(1<<2);
		}
		if((ports.rsPort == GPIOD)&!(gpio_rcc_clk&(1<<3))){
			__HAL_RCC_GPIOD_CLK_ENABLE();
			gpio_rcc_clk = gpio_rcc_clk|(1<<3);
		}


		if((ports.ePort == GPIOA)&!(gpio_rcc_clk&(1<<0))){
			__HAL_RCC_GPIOA_CLK_ENABLE();
			gpio_rcc_clk = gpio_rcc_clk|(1<<0);
		}
		if((ports.ePort == GPIOB)&!(gpio_rcc_clk&(1<<1))){
			__HAL_RCC_GPIOB_CLK_ENABLE();
			gpio_rcc_clk = gpio_rcc_clk|(1<<1);
		}
		if((ports.ePort == GPIOC)&!(gpio_rcc_clk&(1<<2))){
			__HAL_RCC_GPIOC_CLK_ENABLE();
			gpio_rcc_clk = gpio_rcc_clk|(1<<2);
		}
		if((ports.ePort == GPIOD)&!(gpio_rcc_clk&(1<<3))){
			__HAL_RCC_GPIOD_CLK_ENABLE();
			gpio_rcc_clk = gpio_rcc_clk|(1<<3);
		}
	}

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/*Configure GPIO pin Output Level */
	for (uint8_t i = 0; i < 4; ++i) {
		HAL_GPIO_WritePin(ports.dataPorts[i], pins.dataPins[i], GPIO_PIN_RESET);
	}
	HAL_GPIO_WritePin(ports.rsPort, pins.rsPin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ports.ePort, pins.ePin, GPIO_PIN_RESET);

	/*Configure GPIO pins*/
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	for (uint8_t i = 0; i < 4; ++i) {
		GPIO_InitStruct.Pin = pins.dataPins[i];
		HAL_GPIO_Init(ports.dataPorts[i], &GPIO_InitStruct);
	}

	GPIO_InitStruct.Pin = pins.rsPin;
	HAL_GPIO_Init(ports.rsPort, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = pins.ePin;
	HAL_GPIO_Init(ports.ePort, &GPIO_InitStruct);

	/*Initialize display*/
	this->displayInit();
}


/**
 * @brief Writes 4 bits
 * @note This function writes 4 bits to display pins.
 * @param command: Specify if you want to send data or command
 * 					@arg 0: Data
 * 					@arg 1: Command
 * @param data: 4 bits to send (only 0-3 bits will be send)
 * @retval None
 */
void display1602::writeHalfByte(uint8_t command, uint8_t data){
	//choose between command or data
	if (command) {
		HAL_GPIO_WritePin(ports.rsPort, pins.rsPin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(ports.rsPort, pins.rsPin, GPIO_PIN_RESET);
	}

	//HAL_GPIO_WritePin(ports.ePort, pins.ePin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

	//send data
	for (uint8_t i = 0; i < 4; ++i) {
		HAL_GPIO_WritePin(ports.dataPorts[i], pins.dataPins[i], (GPIO_PinState)(data&(1<<i)));
	}

	HAL_GPIO_WritePin(ports.ePort, pins.ePin, GPIO_PIN_RESET);
}


/**
 * @brief Writes byte
 * @note This function writes 8 bits to display pins (Uses writeHalfByte function twice)
 * @param command: Specify if you want to send data or command
 * 					@arg 0: Data
 * 					@arg 1: Command
 * @param data: 4 bits to send (only 0-3 bits will be send)
 * @retval None
 */
void display1602::writeByte(uint8_t command, uint8_t data){
	uint8_t buf = 0;

	buf = data >> 4;

	this->writeHalfByte(command, buf);
	this->writeHalfByte(command, data);
}


/**
 * @brief Starts display initialization routine
 * @note This function initializes display to 4bit mode
 * @retval None
 */
void display1602::displayInit(){

	HAL_Delay(15);
	this->writeHalfByte(0,0b00000011);
	HAL_Delay(5);
	this->writeHalfByte(0,0b00000011);
	HAL_Delay(1);
	this->writeHalfByte(0,0b00000011);

	HAL_Delay(1);
	this->writeHalfByte(0,0b00000010);

	this->writeByte(0, 0b00101000);
	HAL_Delay(1);
	this->writeByte(0, 0b00001100);
	HAL_Delay(1);
	this->writeByte(0, 0b00000001);
	HAL_Delay(1);
	this->writeByte(0, 0b00000110);

	settings = 0b00001100;
}


/**
 * @brief Display clear function
 * @note This function cleares display content
 * @retval None
 */
void display1602::clear(){
	this->writeByte(0, 0b00000001);
	HAL_Delay(100);
}


/**
 * @brief Sets cursor on a specified coordinates
 * @param x: X coordinate (vertical)
 * @param y: Y coordinate (horisontal)
 * @retval None
 */
void display1602::set_cursor(uint8_t x, uint8_t y){
	this->writeByte(0,0b10000000|(x+0x40*y));
	HAL_Delay(100);
}


/**
 * @brief Turn cursor on/off 
 * @param state: Specify if you want to turn cursor on or off
 * 					@arg 0: OFF
 * 					@arg 1: ON
 * @retval None
 */
void display1602::diplay_cusor(uint8_t state){
	if(state){
		settings |= (1 << 1);
		this->writeByte(0,settings);
	}
	else{
		settings &= ~(1 << 1);
		this->writeByte(0,settings);
	}
}


/**
 * @brief Turn blink on/off 
 * @param state: Specify if you want to turn blink on or off
 * 					@arg 0: OFF
 * 					@arg 1: ON
 * @retval None
 */
void display1602::display_blink(uint8_t state){
	if(state){
			settings |= (1 << 0);
			this->writeByte(0,settings);
		}
		else{
			settings &= ~(1 << 0);
			this->writeByte(0,settings);
		}
}


/**
 * @brief Print one symbol on a display 
 * @param data: Symbol to print(uint8_t (char))
 * @retval None
 */
void display1602::print(uint8_t data){
	this->writeByte(1, data);
	HAL_Delay(2);
}


/**
 * @brief Print a string on a display
 * @param data: String to print
 * @retval None
 */
void display1602::printString(const char* data){
	uint8_t i=0;
	while(data[i]!=0)
	{
		this->print(data[i]);
		HAL_Delay(1);
		i++;
	}
}
