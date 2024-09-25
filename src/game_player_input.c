#include "main.h"


bitmask32_t GameInput_get_player_core_input
	(short keyscheme[CORE_INPUTS_COUNT]) 
{

	bitmask32_t player_input = 0;

	// for each character in control scheme
	for (uint i = 0; i < CORE_INPUTS_COUNT; i++) {
		// check if the key is pressed
		if (IsKeyDown(keyscheme[i])) {
			// if so find appropiate flag from 
			// control commands lookup table
			uint flag = COREINPUTS_LOOKUP_TABLE[i];
			
			// add flag to bitmask
			bitmask32_add_flag(&player_input, flag);
		}
	}

	return player_input;
}

void GameInput_send_inputs_to_ctx(
		GameContext* ctx, 
		bitmask32_t input_mask) 

{
	ctx->player_input = input_mask;
}
