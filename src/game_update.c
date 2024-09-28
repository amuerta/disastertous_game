#include "main.h"

void update(GameContext* const game) {
	GameUpdateProc game_systems[] = {
		GameContext_handle_controls,
		GameContext_check_for_bb_collision,
		GameContext_update_velocity,
		GameContext_apply_drag,
		GameContext_update_position,
	};
	size_t game_sys_count = STATIC_ARRAY_LEN(game_systems);


	for(uint i = 0; i < game_sys_count; i++) {
		game_systems[i](game);
	}
}

void* call_update_timed(void *arg) {
	GameContext* ctx = (GameContext*) arg;
	long long begin, end;

	while(ctx->should_update) {

		begin = time_in_mills();
		update(ctx);
		end = time_in_mills();

		time_ms_t 	update_time = end-begin;
		float 		waittime 	= (1.0 / UPS);
		float 		delay_time 	= get_seconds_from_millis(update_time);

		// TODO: timing handling on update 
		// if it takes too long?
		if (waittime-delay_time > 0.0) {
			usleep(waittime * MICROSECONDS * MILLISECONDS);
		}

		ctx->update_time = waittime-delay_time;
	}
	return 0;
}
