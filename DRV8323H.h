/* 
	DRV8323H BLDC Driver for Arduino ESP32
	Date: September 2019
	Author: Israel Cayetano
*/

#ifndef _DRV8323H_h
#define _DRV8323H_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <PWM_ESP32.h>

#define PWM_6 0
#define PWM_3 1
#define PWM_1 2
#define FORWARD 0
#define REVERSE 1
#define A 0
#define B 1
#define C 2
#define BC 1
#define AC 2
#define AB 3
#define CB 4
#define CA 5
#define BA 6

class BLDC
{
public:
	BLDC(uint8_t inh_pins[3], uint8_t inl_pins[3], uint8_t so_pins[3],  uint8_t nfault_pin, uint8_t en_pin);
	~BLDC();
	PWM pwmA;
	PWM pwmB;
	PWM pwmC;
	void begin(uint8_t channels[3], double frequency);
	void doSequence(bool sensor_a, bool sensor_b, bool sensor_c, bool direction, float duty_cycle);
	void setCoast();
	void setBrake();
	void setHigh(uint8_t coil, float duty_cycle);
	void setLow(uint8_t coil);
	void setFloat(uint8_t coil);
	void readSensors();
	bool sens_a;
	bool sens_b;
	bool sens_c;
protected:
	void align();
	uint8_t getStep(bool sensor_a, bool sensor_b, bool sensor_c, bool direction);
	uint8_t _inh_pins[3];
	uint8_t _inl_pins[3];
	uint8_t _sensor_pins[3];
	uint8_t _nfault_pin;
	uint8_t _en_pin;
	uint8_t _step;
};

#endif