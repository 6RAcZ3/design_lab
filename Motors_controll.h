#pragma once
#include "MKL25Z4.h"
#define MAX_PWM_VAL 2400
#define AIN1 (1 << 5)
#define AIN2 (1 << 4)
#define BIN1 (1 << 9)
#define BIN2 (1 << 8)

volatile uint16_t speed = 50;
void Motor_Set_Speed(void);
void Motor_Init(void){
	
	PORTA->PCR[5] = PORT_PCR_MUX(1); // AIN1
  PORTA->PCR[4] = PORT_PCR_MUX(1); // AIN2
  PORTC->PCR[9] = PORT_PCR_MUX(1); // BIN1
  PORTC->PCR[8] = PORT_PCR_MUX(1); // BIN2
	PORTC->PCR[3] = PORT_PCR_MUX(1);
	
	PTA->PDDR |= AIN1 | AIN2;
  PTC->PDDR |= BIN1 | BIN2 | (1 << 3);
	PTC->PSOR = (1 << 3);
	
	
	PORTA->PCR[12] = PORT_PCR_MUX(3);
  PORTA->PCR[13] = PORT_PCR_MUX(3);
	
	Motor_Set_Speed();
	
	TPM1->SC = 0;                         
  TPM1->CNT = 0;
  TPM1->MOD = MAX_PWM_VAL;
	
	TPM1->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
  TPM1->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
	
	TPM1->SC = TPM_SC_CMOD(1) | TPM_SC_PS(0);
	
}

void Motor_Set_Speed(void){

		TPM1->CONTROLS[0].CnV = (MAX_PWM_VAL * speed) / 100;
		TPM1->CONTROLS[1].CnV = (MAX_PWM_VAL * speed) / 100;
}

void Motor_Stop(void){

	PTA->PCOR = AIN1 | AIN2;
  PTC->PCOR = BIN1 | BIN2;
	speed = 50;
	Motor_Set_Speed();
}

void Motor_Forward(void){
	
	PTA->PSOR = AIN1;
	PTA->PCOR = AIN2;
	
	PTC->PSOR = BIN1;
	PTC->PCOR = BIN2;
}

void Motor_Backward(void){
	
	PTA->PSOR = AIN2;
	PTA->PCOR = AIN1;
	
	PTC->PSOR = BIN2;
	PTC->PCOR = BIN1;
}

void Motor_TurnLeft(void){
    PTA->PCOR = AIN1 | AIN2;
    PTC->PSOR = BIN1;
    PTC->PCOR = BIN2;
}

void Motor_TurnRight(void){
    PTC->PCOR = BIN1 | BIN2;
    PTA->PSOR = AIN1;
    PTA->PCOR = AIN2;
}

void Motor_Speed_Up(void){
	
	speed += 5;
	
	if (speed >= 80)
			speed = 80;

	Motor_Set_Speed();
}

void Motor_Speed_Down(void){
	
	speed -= 5;
	
	if (speed <= 30)
			speed = 30;

	Motor_Set_Speed();
}