
#include "config.h"
#include "main.h"
#include <stdarg.h>


GameConfig GameConfig_default(void) {
	return (GameConfig) {
		.ups		= 30			,
		.resolution = { 1440, 810 }	,

		/* .resolution = { 800, 600 } */
	};
}

GameContext GameContext_init(void) {
	return (GameContext) {
		.should_update 		= true						,
		.debug_flags 		= 0							,
		.entites 			= calloc(
									DEFAULT_ENT_COUNT,
									sizeof(Entity)
								)						,
		.entites_capacity 	= DEFAULT_ENT_COUNT			,
		.entites_count 		= 0							,
	};
}

Entity* GameContext_get_ent(ConstGameContext ctx,relative_ptr p) {
	for(uint i = 0; i < ctx->entites_count; i++) {
		if (i == p) {
			return &ctx->entites[i];
		}
	}
	return NULL;
}

Viewport Viewport_init(GameContext* ctx, GameConfig config) {
	
	Entity* player = GameContext_get_ent(ctx,ctx->player_rptr);

	if (!player) {
		ctx->entites[0] = (Entity) {
			.valid = 1, 
			.position = {400,400},
			.bounding_box = {-10,-10,10,10},
		};
		ctx->player_rptr = 0;
		player = &ctx->entites[0];
	}

	return (Viewport) {
		.screen_resolution = config.resolution,
		.camera = {
			.target = player->position,
			.offset = Vector2Divide(config.resolution,(Vector2){2,2}),
			.zoom 	= 1.0f,
		},
	};
}

void Viewport_update(Viewport* view, GameContext* ctx) {
	view->camera.target = (
			ctx->entites[ctx->player_rptr].position
	);

	// if window resized
}

void GameContext_deinit(GameContext* ctx) {
	// free entities pool
	if (ctx->entites) {
		free(ctx->entites);
		ctx->entites = 0;
	}
}

bool has_flag(uint param, uint flag) {
	return ((param & flag) > 0);
}

uint set_debug_flags(uint n, ... ) {
	va_list ptr;
    va_start(ptr, n);
	bitmask32_t	mask = 0;

    for (int i = 0; i < n; i++)
        mask += va_arg(ptr, uint);
 
    va_end(ptr);
    return mask;
}

void update(GameContext* const game) {
	const float delta = GetFrameTime();
	game->last_delta = delta;


}

void DrawCross(	Vector2 position, 
				float width, 
				float height, 
				float thickness, 
				Color color) 
{
	DrawRectangle(
			position.x - thickness/2,
			position.y - height/2,
			thickness, height,
			color
	);

	DrawRectangle(
			position.x - width/2,
			position.y - thickness/2,
			width, thickness,
			color
	);
}

void GameContext_draw_perf_stat_overlay(ConstGameContext game,Vector2 resolution) {
	
	char  fmt_text_buffer[256] = {0};

	const Vector2 origin	= {10,10};
	const float spacing 	= 10.0;
	float offset 			= origin.y;

	const Vector2 screen_size_ratio = {
		.x = sqrt(resolution.x/100) * 50,
		.y = sqrt(resolution.y/100) * 50
	};

	Rectangle pso_background = {
		.x = 10,
		.y = 10,
		.width = screen_size_ratio.x,
		.height = screen_size_ratio.y,
	};

	Rectangle item_bounds = {
		.x 		= origin.x + spacing,
		.y 		= origin.y + spacing,
		.width	= screen_size_ratio.x,
		.height = 10,
	};

	DrawRectangleRec(pso_background, GetColor(0x202020AA));
	DrawRectangleLinesEx(pso_background,1,GetColor(0x404040FF));


	sprintf(fmt_text_buffer, "FPS: %d", GetFPS());
	GuiLabel(item_bounds, fmt_text_buffer);
	
	item_bounds.y += spacing;

	sprintf(fmt_text_buffer, "Frame time: %.4f", game->last_delta);
	GuiLabel(item_bounds, fmt_text_buffer);
}

void GameContext_draw_wireframes(ConstGameContext game) 
{
	size_t count = game->entites_count;
	for(uint i = 0; i < count; i++) {
		
		Entity 		ent = game->entites[i];
		Vector2 	pos = ent.position;
		Rectangle	bb	= ent.bounding_box[0];

		Rectangle wireframe = {
			.x 		= pos.x + bb.x,
			.y 		= pos.y + bb.y,
			.width 	= fabs(bb.x) + bb.width,
			.height = fabs(bb.y) + bb.height,
		};

		DrawCross(pos,10,10,2,GREEN);
		DrawRectangleLinesEx(wireframe,1,GREEN); 
	} 
}

void GameRenderer_render(
		const GameContext* const game, 
		const GameRenderer* const renderer,
		Viewport* view
	) 

{
		// clear frame
		ClearBackground(GetColor(0x181818FF));

		if (IsWindowResized()) {
			view->screen_resolution = (Vector2){ 
				GetScreenWidth(), 
				GetScreenHeight() 
			};
		}

		// debug
		uint debug_flags = game->debug_flags;

		if (has_flag(debug_flags,Debug_draw_wireframes)) {
			BeginMode2D(view->camera);
			GameContext_draw_wireframes(game);
			EndMode2D();
		}

		if (has_flag(debug_flags,Debug_display_performance)) {
			GameContext_draw_perf_stat_overlay(
					game,
					view->screen_resolution
			);
		}

}


void GameContext_append_entity_pool(GameContext *ctx, Entity ent) {
	assert(ctx->entites && "ENTITIES POOL POINTER CANNOT BE NULL");
	if (ctx->entites_count >= ctx->entites_capacity) {
		ctx->entites = reallocarray(
				ctx->entites,
				ctx->entites_capacity + 128,
				sizeof(Entity)
		);
		ctx->entites_capacity += 128;
	}

	ctx->entites[ctx->entites_count] = ent;
	ctx->entites_count++;
}

// bin mask example
/* if (3 == (Debug_draw_wireframes | Debug_draw_movement_vectors)) */
/* 	printf("\ntrue\n"); */

void* call_update_timed(void *arg) {
	GameContext* ctx = (GameContext*) arg;

	while(ctx->should_update) {
		update(ctx);
		float waittime = 1.0 / UPS;
		sleep(waittime);
	}
	return 0;
}

int main(void) {
	
	pthread_t 		update_thread_id;
	
	GameConfig 		game_config;
	GameContext		game_ctx;
	GameRenderer	game_gfx;
	Viewport		game_view;

	game_config = GameConfig_default();
	game_ctx	= GameContext_init();
	game_view 	= Viewport_init(&game_ctx,game_config);

	game_ctx.debug_flags = set_debug_flags(2,
			Debug_draw_wireframes,
			Debug_display_performance
	);

	Entity player = { 
		.position = {400, 400},
		.bounding_box = {-10,-10,10,10},
	};

	GameContext_append_entity_pool(&game_ctx,player);

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
			&call_update_timed,
			&game_ctx
	);

	while(!WindowShouldClose()) {
		BeginDrawing();
		Viewport_update(&game_view,&game_ctx);
		GameRenderer_render(&game_ctx,&game_gfx, &game_view);
		EndDrawing();
	}

	CloseWindow();
	GameContext_deinit(&game_ctx);
	return 0;
}
