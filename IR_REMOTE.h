#pragma once
/*
	This file contains specifications for the "Car MP3" IR remote control, which uses a non-standard protocol (different from standard NEC).
	TPM0 Frequency: 1.5 MHz (1 tick ~=0.667us)
	Pulse timings (determined empirically via debugging):
	Logic 0: ~580–800 ticks
	Logic 1: ~1450–1550 ticks
*/
//IR REMOTE CONTROLER BUTTONS CODE
#define bt_ch_minus 0xBA45FF00
#define bt_ch 0xB946FF00
#define bt_ch_plus 0xB847FF00
#define bt_prev 0xBB44FF00
#define bt_next 0xBF40FF00
#define bt_play 0xBC43FF00
#define bt_vol_min 0xF807FF00
#define bt_vol_plus 0xEA15FF00
#define bt_eq 0xF609FF00
#define bt_0 0xE916FF00
#define bt_100 0xE619FF00
#define bt_200 0xF20DFF00
#define bt_1 0xF30CFF00
#define bt_2 0xE718FF00
#define bt_3 0xA15EFF00
#define bt_4 0xF708FF00
#define bt_5 0xE31CFF00
#define bt_6 0xA55AFF00
#define bt_7 0xBD42FF00
#define bt_8 0xAD52FF00
#define bt_9 0xB54AFF00
