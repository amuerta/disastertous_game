#include "main.h"

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
