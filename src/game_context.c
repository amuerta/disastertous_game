#include "main.h"

// TODO:
//
// ? world renderer interpolates inbetween update ticks to 
// make game look smoother
//
// - make movement have unlimited growth with sqrt calulations for 
// maximum velocity (velocity overall)
//
// - make big variatey of entity move schemes:
// 		has a face -> moves to look dir with keyboard
// 		moves independent of look dir -> ufo style
// 		moves to where mouse set the position -> torpedo style

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



void GameContext_handle_controls(GameContext* const game) {
	Entity* player = &game->entites[game->player_rptr];

	if (bitmask32_has_flag(game->player_input, ECoreInputs_move_up)) {
		Entity_apply_force(player,Vec2(0,-0.1));	
	}

	if (bitmask32_has_flag(game->player_input,ECoreInputs_move_down)) {
		Entity_apply_force(player,Vec2(0,0.1));	
	}

	if (bitmask32_has_flag(game->player_input, ECoreInputs_move_left)) {
		//Entity_apply_force(player,Vec2(-0.1,0));	
		player->rotation_angle -= 0.1;
	}

	if (bitmask32_has_flag(game->player_input,ECoreInputs_move_right)) {
		//Entity_apply_force(player,Vec2(0.1,0));	
		player->rotation_angle += 0.1;
	}

}

// TODO: make both proc work at 100% freq only for entites 
// within screen space

// NOTE: possible slow performance


void GameContext_update_position(GameContext* const game) {
	for(uint i = 0; i < game->entites_count; i++) {
		Entity* ent = &game->entites[i];
		switch(ent->move_type) {

			case EMove_free: 
				Entity_move_as_free(ent);
				break;

			case EMove_face:
				Entity_move_to_face(ent);
				break;

			default: 
				assert(false && "TODO!");
				break;
		}
	}
}

void GameContext_update_velocity(GameContext* const game) {
	for(uint i = 0; i < game->entites_count; i++) {
		Entity* ent = &game->entites[i];
		
		ent->velocity = Vector2Add(ent->velocity,ent->acceleration);
		
		bool min_x = fabs(ent->velocity.x) < 0.001;
		bool min_y = fabs(ent->velocity.y) < 0.001;
		

		if (min_x) {
			ent->velocity.x = 0;
		}
		if (min_y) {
			ent->velocity.y = 0;
		}

		/* printf("ent@%d.vel = %f,%f\n",i, */
		/* 		ent->velocity.x, */
		/* 		ent->velocity.y */
		/* ); */
	}
}


// NOTE: possible slow performance
void GameContext_apply_drag(GameContext* const game) {
	for(uint i = 0; i < game->entites_count; i++) {
		Entity* ent = &game->entites[i];

		ent->velocity = Vector2Multiply(
				ent->velocity,
				Vec2(0.9,0.9)
		);

		ent->acceleration = Vector2Multiply(
				ent->acceleration,
				Vec2(0.75,0.75)
		);

		bool non_zero_vel = ent->velocity.x > 0.01 && ent->velocity.y > 0.01;
		bool min_x = fabs(ent->acceleration.x) < 0.001;
		bool min_y = fabs(ent->acceleration.y) < 0.001;

		if (min_x) {
			ent->acceleration.x = 0;
		}
		if (min_y) {
			ent->acceleration.y = 0;
		}
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

void GameContext_deinit(GameContext* ctx) {
	// free entities pool
	if (ctx->entites) {
		free(ctx->entites);
		ctx->entites = 0;
	}
}

