
#include "config.h"
#include "main.h"
#include <stdarg.h>

#include "config.h"
#include "time.c"
#include "raylib_ext.c"


#include "game_entity.c"

#include "game_debug.c"
#include "game_context.c"
#include "game_player_input.c"
#include "game_viewport.c"
#include "game_update.c"
#include "game_rendering.c"


GameConfig GameConfig_default(void) {
	return (GameConfig) {
		.resolution = { 1440, 810 }	,

		/* .resolution = { 800, 600 } */
	};
}

short control_scheme[CORE_INPUTS_COUNT] = {
	KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, 
	KEY_ENTER, KEY_F,
	KEY_ONE,
	KEY_TWO,
	KEY_THREE,
	KEY_FOUR,
	KEY_ZERO
};

int main(void) {
	
	pthread_t 		update_thread_id;
	
	GameConfig 		game_config;
	GameContext		game_ctx;
	GameRenderer	game_gfx;
	Viewport		game_view;

	game_config = GameConfig_default();
	game_ctx	= GameContext_init();
	game_view 	= Viewport_init(&game_ctx,game_config);

	game_ctx.debug_flags = bitmask32_set_flags(4,
			EDebug_draw_wireframes,
			EDebug_draw_movement_vectors,
			EDebug_draw_entity_data,
			EDebug_display_performance
	);

	Entity player = { 
		.position = {400, 400},
		.move_type = EMove_face,
		.bounding_box = {-10,-10,10,10},
	};

	Entity enemy = { 
		.position = {100, 100},
		.bounding_box = {-20,-20,20,20},
	};

	GameContext_append_entity_pool(&game_ctx,player);
	GameContext_append_entity_pool(&game_ctx,enemy);

	SetTargetFPS(60);
	InitWindow(
			game_config.resolution.x,
			game_config.resolution.y,
			"game"
	);

	// create thread
	pthread_create(
			&update_thread_id,
			NULL,
			(void*)&call_update_timed,
			(void*)&game_ctx
	);

	while(!WindowShouldClose()) {
		bitmask32_t player_inputs = GameInput_get_player_core_input(
				control_scheme
		);
		
		BeginDrawing();
		Viewport_update(&game_view,&game_ctx);
		GameRenderer_render(&game_ctx,&game_gfx, &game_view);
		GameInput_send_inputs_to_ctx(&game_ctx, player_inputs);
		EndDrawing();
	}
	CloseWindow();
	game_ctx.should_update = false;
	pthread_exit(&update_thread_id);
	GameContext_deinit(&game_ctx);
	return 0;
}
