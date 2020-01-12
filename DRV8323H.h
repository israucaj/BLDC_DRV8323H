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

#include "PWM_ESP32.h"
#include "Module.h"
#include "ESP32_ExtInterrupt.h"

/******************************************************************
 *                 D E F I N E S  &  M A C R O S                   *
 ******************************************************************/
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
/* Constant pin definitions based on support PCB to connect the DRV8323H with the NIMU */
#define INHA 25
#define INHB 32
#define INHC 33
#define INLA 26
#define INLB 27
#define INLC 14
#define INH_NIMU_PINS	{INHA, INHB, INHC}
#define INL_NIMU_PINS	{INLA, INLB, INLC}
/* BLDC Motor´s Sensor (Encoder) pin definitions */
#define ENCA 19
#define ENCB 5
#define ENCC 18
#define ENC_NIMU_PINS	{ENCA, ENCB, ENCC}

 /******************************************************************
  *                        C L A S S E S                           *
  ******************************************************************/
class BLDC
{
public:
	BLDC(const uint8_t inh_pins[3], const uint8_t inl_pins[3], const uint8_t so_pins[3],  uint8_t nfault_pin);
	~BLDC();
	PWM pwmA;
	PWM pwmB;
	PWM pwmC;
	ExternalInterrupt encA;
	ExternalInterrupt encB;
	ExternalInterrupt encC;
	void begin(uint8_t channels[3], double frequency);
	void doSequence(bool direction, float duty_cycle);
	void setCoast(void);
	void setBrake(void);
	void setHigh(uint8_t coil, float duty_cycle);
	void setLow(uint8_t coil);
	void setFloat(uint8_t coil);
	void readSensors(void);
	bool sens_a;
	bool sens_b;
	bool sens_c;
	static moduleDesc_S DRV_desc; 
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