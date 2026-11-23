#pragma once
#include "MKL25Z4.h"
#include "IR_REMOTE.h"
#include "Motors_controll.h"

volatile uint32_t ir_data = 0;       
volatile uint8_t  overflow_count = 0;     
volatile uint8_t  bit_index = 0;
volatile uint32_t raw_frame = 0;
volatile uint8_t  start_control = 0;


void IR_Init(void){
			// TMP0 clock: MCGFLLCLK / PLL = 48 MHz
			SIM->SOPT2 = (SIM->SOPT2 & ~SIM_SOPT2_TPMSRC_MASK) | SIM_SOPT2_TPMSRC(1);
			
			//PTD4 setup
			PORTD->PCR[4] = PORT_PCR_MUX(1) |      // Turn on GPIO
											PORT_PCR_PE_MASK |     // Turn on pull-up resistor
											PORT_PCR_PS_MASK |     // Pull-up 
											PORT_PCR_IRQC(0x0A);   // Interups on falling edge
	
			PTD->PDDR &= ~(1 << 4); //Set PTD4 as input
	
			//TMP0 setup: preskaler /32 (48 MHz / 32 = 1.5 MHz -> 1 tick = 0.667 us)
			TPM0->SC = 0;                          
			TPM0->CNT = 0;                         
			TPM0->MOD = 0xFFFF; 
			
			// Turn on interups control for overflow
			TPM0->SC = TPM_SC_CMOD(1) | TPM_SC_PS(5) | TPM_SC_TOIE_MASK; 

}

void TPM0_IRQHandler(void) {
    if (TPM0->SC & TPM_SC_TOF_MASK) {
        TPM0->SC |= TPM_SC_TOF_MASK; // Clear overflow flag
        if (overflow_count < 255) {
            overflow_count++;
        }
    }
}

uint8_t IR_read()
{
				volatile uint8_t ir_ready = 0;
				// Calculate signal time and clearing counters 
				uint32_t elapsed = TPM0->CNT + ((uint32_t)overflow_count * 65536UL);
				TPM0->CNT = 0;                      
				overflow_count = 0;                

        // if time is equal 20ms clear frame
        if (elapsed > 30000) {
            bit_index = 0;
            raw_frame = 0;
        } 
        else if (bit_index > 0 && bit_index <= 32) {
            // if time is longer than 1000 tick set 1 else set 0
            if (elapsed > 1000) {
                raw_frame |= (1UL << (bit_index - 1));
            }
            //	checking if frame is ready
            if (bit_index == 32) {
                ir_data = raw_frame;
                ir_ready = 1;
                bit_index = 0;
                raw_frame = 0;
            } else {
                bit_index++;
            }
        } 
        else {
            // setup new frame 
            bit_index = 1;
            raw_frame = 0;
        }
				
				return ir_ready;
}

void IR_encode()
{
		if (bt_play == ir_data) {
				start_control = !start_control;
				Motor_Stop();
		}
	
		if (start_control) {
			switch (ir_data){
				
				case bt_vol_plus:
						Motor_Speed_Up();
					break;
				
				case bt_vol_min:
						Motor_Speed_Down();
					break;
				
				case bt_eq:
						PTB->PTOR = (1 << 18);
					break;
				
				case bt_2:
						Motor_Forward();
					break;
				
				case bt_5:
							Motor_Stop();
					break;
				
				case bt_4:
							Motor_TurnLeft();
					break;
				
				case bt_6:
							Motor_TurnRight();
					break;
				
				case bt_8:
							Motor_Backward();
					break;
				
				default:
					return;
			}
		}
		
		
}
