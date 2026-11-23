#include "MKL25Z4.h"
#include "IR_REMOTE.h"
#include "IR_controller.h"
#include "Motors_controll.h"

volatile uint8_t ir_data_ready = 0; 

void Init(void) {
    // Turn on clock for ports and tmp
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTB_MASK |SIM_SCGC5_PORTA_MASK |SIM_SCGC5_PORTC_MASK;
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
		SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;

    // test leds
    PORTB->PCR[18] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[18] |= PORT_PCR_MUX(1);
    PTB->PDDR |= (1 << 18);
    PTB->PSOR = (1 << 18);

    PORTB->PCR[19] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[19] |= PORT_PCR_MUX(1);
    PTB->PDDR |= (1 << 19);
    PTB->PSOR = (1 << 19); 

		// Initialization IR receiver
		IR_Init();
	
		Motor_Init();


    // Set up interupts
		NVIC_SetPriority(PORTD_IRQn, 3);
    NVIC_ClearPendingIRQ(PORTD_IRQn);
    NVIC_EnableIRQ(PORTD_IRQn);
    
		NVIC_SetPriority(PORTD_IRQn, 2);
    NVIC_ClearPendingIRQ(TPM0_IRQn);
    NVIC_EnableIRQ(TPM0_IRQn);
}



void PORTD_IRQHandler(void) {
    if (PORTD->ISFR & (1 << 4)) {      
				ir_data_ready = IR_read();

        PORTD->ISFR = (1 << 4);
    }
}

int main(void) {
	
    SystemCoreClockUpdate();
    Init();
    

    while (1) {

			if(ir_data_ready)
			{
				ir_data_ready = 0;
				IR_encode();
			}
			
    }
}
