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

static void projectRectangleOntoAxis(Vector2* vertices, Vector2 axis, float* min, float* max) {
    *min = Vector2DotProduct(vertices[0], axis);
    *max = *min;

    for (int i = 1; i < 4; i++) {
        float projection = Vector2DotProduct(vertices[i], axis);
        if (projection < *min) *min = projection;
        if (projection > *max) *max = projection;
    }
}

static bool overlapOnAxis(Vector2* verticesA, Vector2* verticesB, Vector2 axis) {
    float minA, maxA, minB, maxB;

    projectRectangleOntoAxis(verticesA, axis, &minA, &maxA);
    projectRectangleOntoAxis(verticesB, axis, &minB, &maxB);

    return !(maxA < minB || maxB < minA);
}

static bool SATCheck(Vector2* verticesA, Vector2* verticesB) {

    Vector2 axes[8] = {
        Vector2Subtract(verticesA[0], verticesA[1]), 
        Vector2Subtract(verticesA[1], verticesA[3]), 
        Vector2Subtract(verticesA[3], verticesA[2]), 
        Vector2Subtract(verticesA[2], verticesA[0]), 
        Vector2Subtract(verticesB[0], verticesB[1]), 
        Vector2Subtract(verticesB[1], verticesB[3]), 
        Vector2Subtract(verticesB[3], verticesB[2]), 
        Vector2Subtract(verticesB[2], verticesB[0])  
    };

    for (int i = 0; i < 8; i++) {
        if (!overlapOnAxis(verticesA, verticesB, axes[i])) {
            return false;  
		}
	}
	return true;  
}

// TODO: make all procedures work at 100% freq only for entites 
// within screen space, for every other one, update lazily dependeing
// on distance, is_being_viewed, of other factors.

// NOTE: slow performance

void GameContext_check_for_bb_collision(GameContext* const game) {
	for(uint i = 0; i < game->entites_count; i++) {
		for(uint o = 0; o < game->entites_count; o++) {
			Entity *target, *other;
			Rectangle rec1, rec2;
			Vector2 vertecies[2][4] = {0};

			if (i==o)
				continue;

			target 	= &game->entites[i];
			other	= &game->entites[o];

			rec1 = Entity_get_rect_from_boundingbox(*target); 
			rec2 = Entity_get_rect_from_boundingbox(*other); 

			GetRotatedRectanglePointsStorage(
					vertecies[0],
					rec1,
					target->position,
					target->rotation_angle / RADIAN_DEG_RATIO
				);

			GetRotatedRectanglePointsStorage(
					vertecies[1],
					rec2,
					other->position,
					other->rotation_angle / RADIAN_DEG_RATIO
				);
			

			printf("Vertecies for rec1: (%.1f,%.1f), (%.1f,%.1f), (%.1f,%.1f), (%.1f,%.1f)\n",
					vertecies[0][0].x, vertecies[0][0].y,
					vertecies[0][1].x, vertecies[0][1].y,
					vertecies[0][2].x, vertecies[0][2].y,
					vertecies[0][3].x, vertecies[0][3].y
			);

			bool collide = SATCheck(vertecies[0],vertecies[1]);

			target->is_clipping = collide;
			other->is_clipping = collide;
		}
	}
}


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

