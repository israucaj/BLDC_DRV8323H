/* 
	DRV8323H BLDC Driver for Arduino ESP32
	Date: September 2019
	Author: Israel Cayetano
*/

#include "DRV8323H.h"

BLDC::BLDC(uint8_t inh_pins[3], uint8_t inl_pins[3], uint8_t sensor_pins[3], uint8_t nfault_pin, uint8_t en_pin)
{
	for (uint8_t i = 0; i < 3; i++)
	{
		_inh_pins[i] = inh_pins[i];
		_inl_pins[i] = inl_pins[i];
		_sensor_pins[i] = sensor_pins[i];
	}
	_nfault_pin = nfault_pin;
	pinMode(_nfault_pin, INPUT);
	_en_pin = en_pin;
	pinMode(_en_pin, OUTPUT);
	digitalWrite(_en_pin, HIGH);
}

BLDC::~BLDC()
{
}

void BLDC::begin(uint8_t channels[3], double frequency)
{
	digitalWrite(_en_pin, LOW);
	pwmA.setup(_inh_pins[0], channels[0], frequency, 10, HIGH);
	pwmA.attachPin(_inl_pins[0]);
	pwmB.setup(_inh_pins[1], channels[1], frequency, 10, HIGH);
	pwmB.attachPin(_inl_pins[1]);
	pwmC.setup(_inh_pins[2], channels[2], frequency, 10, HIGH);
	pwmC.attachPin(_inl_pins[2]);
	setCoast();
}

void BLDC::setHigh(uint8_t coil,float duty_cycle)
{
	switch (coil)
	{
	case A:
		GPIO.func_out_sel_cfg[_inl_pins[0]].inv_sel = 1;
		pwmA.setDuty(duty_cycle);
		break;
	case B:
		GPIO.func_out_sel_cfg[_inl_pins[1]].inv_sel = 1;
		pwmB.setDuty(duty_cycle);
		break;
	case C:
		GPIO.func_out_sel_cfg[_inl_pins[2]].inv_sel = 1;
		pwmC.setDuty(duty_cycle);
		break;
	default:
		break;
	}
}

void BLDC::setLow(uint8_t coil)
{
	switch (coil)
	{
	case A:
		GPIO.func_out_sel_cfg[_inl_pins[0]].inv_sel = 1;
		pwmA.setDuty(0);
		break;
	case B:
		GPIO.func_out_sel_cfg[_inl_pins[1]].inv_sel = 1;
		pwmB.setDuty(0);
		break;
	case C:
		GPIO.func_out_sel_cfg[_inl_pins[2]].inv_sel = 1;
		pwmC.setDuty(0);
		break;
	default:
		break;
	}
}

void BLDC::setFloat(uint8_t coil)
{
	switch (coil)
	{
	case A:
		GPIO.func_out_sel_cfg[_inl_pins[0]].inv_sel = 0;
		pwmA.setDuty(0);
		break;
	case B:
		GPIO.func_out_sel_cfg[_inl_pins[1]].inv_sel = 0;
		pwmB.setDuty(0);
		break;
	case C:
		GPIO.func_out_sel_cfg[_inl_pins[2]].inv_sel = 0;
		pwmC.setDuty(0);
		break;
	default:
		break;
	}
}

void BLDC::setCoast()
{
	setFloat(A);
	setFloat(B);
	setFloat(C);
}

void BLDC::setBrake()
{
	setLow(A);
	setLow(B);
	setLow(C);
}

void BLDC::align()
{
	GPIO.func_out_sel_cfg[_inl_pins[0]].inv_sel = 1;
	pwmA.setDuty(10);
	setLow(B);
	setLow(C);
}

uint8_t BLDC::getStep(bool sensor_a, bool sensor_b, bool sensor_c, bool direction)
{
	uint8_t step;
	uint8_t sensor_sequence = 0;
	bitWrite(sensor_sequence, 2, sensor_a);
	bitWrite(sensor_sequence, 1, sensor_b);
	bitWrite(sensor_sequence, 0, sensor_c);
	switch (sensor_sequence)
	{
	case 1:
		if (direction == FORWARD)
			step = CB ;
		else
			step = BC;
		break;
	case 2:
		if (direction == FORWARD)
			step = BA;
		else
			step = AB;
		break;
	case 3:
		if (direction == FORWARD)
			step = CA;
		else
			step = AC;
		break;
	case 4:
		if (direction == FORWARD)
			step = AC;
		else
			step = CA;
		break;
	case 5:
		if (direction == FORWARD)
			step = AB;
		else
			step = BA;
		break;
	case 6:
		if (direction == FORWARD)
			step = BC;
		else
			step = CB;
		break;
	}
	return step;
}

void BLDC::doSequence(bool sensor_a, bool sensor_b, bool sensor_c, bool direction, float duty_cycle)
{
	_step = getStep(sensor_a, sensor_b, sensor_c, direction);
	switch (_step)
	{
	case BC:
		setFloat(A);
		setHigh(B, duty_cycle);
		setLow(C);
		break;
	case AC:
		setHigh(A, duty_cycle);
		setFloat(B);
		setLow(C);
		break;
	case AB:
		setHigh(A, duty_cycle);
		setLow(B);
		setFloat(C);
		break;
	case CB:
		setFloat(A);
		setLow(B);
		setHigh(C, duty_cycle);
		break;
	case CA:
		setLow(A);
		setFloat(B);
		setHigh(C, duty_cycle);
		break;
	case BA:
		setLow(A);
		setHigh(B, duty_cycle);
		setFloat(C);
		break;
	}
}

void BLDC::readSensors()
{
	sens_a = digitalRead(_sensor_pins[0]);
	sens_b = digitalRead(_sensor_pins[1]);
	sens_c = digitalRead(_sensor_pins[2]);
}