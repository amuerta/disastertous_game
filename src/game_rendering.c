#include "main.h"

// NOTE: possible bad perforamance
void GameRenderer_draw_move_vectors
	(ConstGameContext game,Viewport* view) 
{
	Entity* ents 		= game->entites;
	size_t	ents_count = game->entites_count;
	
	for(uint i = 0; i < ents_count; i++) {
		Entity ent = ents[i];
		size_t line_len = 50;

		Vector2 pos 	= ent.position;
		Rectangle line = {pos.x,pos.y,2,line_len};
		DrawRectanglePro(
				line,
				Vec2(1,line_len),
				ent.rotation_angle / RADIAN_DEG_RATIO,
				RED
		);
	}
}

void GameRenderer_draw_wireframes(ConstGameContext game) 
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


// TODO: finish
void GameRenderer_draw_entity_info_hud
	(ConstGameContext game,Vector2 resolution) 
{
	Entity* ents 		= game->entites;
	size_t	ents_count = game->entites_count;
	
	for(uint i = 0; i < ents_count; i++) {
		Entity ent 			= ents[i];
		Rectangle bb 		= ent.bounding_box[0];
		Vector2 pos 		= ent.position;
		Vector2 size 		= { 100, 50 };
		float 	pad			= 8;

		/* Rectangle wireframe = { */
		/* 	.x 		= pos.x + bb.x, */
		/* 	.y 		= pos.y + bb.y, */
		/* 	.width 	= fabs(bb.x) + bb.width, */
		/* 	.height = fabs(bb.y) + bb.height, */
		/* }; */

		Rectangle info_panel = {pos.x,pos.y,size.x,size.y};
		Rectangle info_panel_outline = info_panel;
		info_panel_outline.x += pad + bb.width;
		info_panel_outline.y += pad + bb.height;

		DrawRectanglePro(
				info_panel,
				Vec2(-pad - bb.width,-pad - bb.height),
				0,
				GetColor(0x3A3A3A1A)
		);
		DrawRectangleLinesEx(
				info_panel_outline,
				1,
				GetColor(0x7A7A7AAA)
		);
	}
}

void GameRenderer_draw_perf_stat_overlay
	(ConstGameContext game,Vector2 resolution) 
{
	
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

	sprintf(fmt_text_buffer, "Frame time: %f", game->update_time);
	GuiLabel(item_bounds, fmt_text_buffer);
}


void GameRenderer_draw_cursor(ConstGameContext game) {
	Vector2 mouse_pos = Vec2(GetMouseX(), GetMouseY());
	DrawCross(mouse_pos,1,1,80,WHITE);
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

		if (bitmask32_has_flag(debug_flags,EDebug_draw_wireframes)) 
		{
			BeginMode2D(view->camera);
			GameRenderer_draw_wireframes(game);
			EndMode2D();
		}

		if (bitmask32_has_flag(debug_flags,EDebug_display_performance)) 
		{
			GameRenderer_draw_perf_stat_overlay(
					game,
					view->screen_resolution
			);
		}

		if (bitmask32_has_flag(debug_flags,EDebug_draw_movement_vectors)) 
		{
			BeginMode2D(view->camera);
			GameRenderer_draw_move_vectors(game,view);
			EndMode2D();
		}

		if (bitmask32_has_flag(debug_flags,EDebug_draw_entity_data)) 
		{
			BeginMode2D(view->camera);
			GameRenderer_draw_entity_info_hud(game,Vec2(0,0));
			EndMode2D();
		}


		GameRenderer_draw_cursor(game);
}

