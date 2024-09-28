#include "main.h"

// NOTE: possible bad perforamance
void GameRenderer_debug_draw_move_vectors
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


void GameRenderer_debug_draw_wireframes(ConstGameContext game) 
{
	Color wireframe_col;

	size_t count = game->entites_count;
	for(uint i = 0; i < count; i++) {
		
		Entity 		ent = game->entites[i];
		Vector2 	pos = ent.position;
		Rectangle	bb	= ent.bounding_box[0];

		wireframe_col = GREEN;
		if (ent.is_clipping) {
			wireframe_col = ORANGE;
		}

		/* Rectangle wireframe = { */
		/* 	.x 		= pos.x + bb.x, */
		/* 	.y 		= pos.y + bb.y, */
		/* 	.width 	= fabs(bb.x) + bb.width, */
		/* 	.height = fabs(bb.y) + bb.height, */
		/* }; */

		Rectangle wf = Entity_get_rect_from_boundingbox(ent); 

		DrawCross(pos,10,10,2,GREEN);
		/* DrawRectangleLinesEx(wireframe,1,GREEN); */
		DrawRectangleLinesPro(
				wf,
				Vec2(bb.width,bb.height),
				ent.rotation_angle / RADIAN_DEG_RATIO,
				1,
				wireframe_col); 
	} 
}


// TODO: finish
void GameRenderer_debug_draw_entity_info_hud
	(ConstGameContext game,Vector2 resolution) 

{	
	char  	fmt_text_buffer[256] = {0};
	Entity* ents 		= game->entites;
	size_t	ents_count = game->entites_count;
	


	for(uint i = 0; i < ents_count; i++) {
		Entity ent 			= ents[i];
		Rectangle bb 		= ent.bounding_box[0];
		Vector2 pos 		= ent.position;
		Vector2 size 		= { 100, 100 };
		float 	pad			= 8;

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

		Rectangle item_bounds = {
			.x 		= info_panel_outline.x + pad,
			.y 		= info_panel_outline.y + pad,
			.width	= size.x - 2 * pad,
			.height = pad,
		};

		sprintf(fmt_text_buffer, "hp: %.1f", ent.properties.hp);
		GuiLabel(item_bounds, fmt_text_buffer);

		item_bounds.y += pad * 1.5;

		sprintf(fmt_text_buffer, "def: %.1f", ent.properties.def);
		GuiLabel(item_bounds, fmt_text_buffer);

		item_bounds.y += pad * 1.5;

		sprintf(fmt_text_buffer, "shl: %.1f", ent.properties.shl);
		GuiLabel(item_bounds, fmt_text_buffer);

		item_bounds.y += pad * 1.5;

		sprintf(fmt_text_buffer, "pos: %.0f, %.0f", ent.position.x,ent.position.y);
		GuiLabel(item_bounds, fmt_text_buffer);

		item_bounds.y += pad * 1.5;

		sprintf(fmt_text_buffer, "vel: %.0f, %.0f", ent.velocity.x,ent.velocity.y);
		GuiLabel(item_bounds, fmt_text_buffer);

		item_bounds.y += pad * 1.5;

		sprintf(fmt_text_buffer, "accel: %.1f, %.1f", 
				ent.acceleration.x,
				ent.acceleration.y);
		GuiLabel(item_bounds, fmt_text_buffer);

		item_bounds.y += pad * 1.5;

		sprintf(fmt_text_buffer, "clips: %s", 
				(ent.is_clipping) ? "true" : "false"
				);
		GuiLabel(item_bounds, fmt_text_buffer);


	}
}

void GameRenderer_debug_draw_perf_stat_overlay
	(ConstGameContext game,const Viewport* const v) 
{
	
	char  fmt_text_buffer[256] = {0};

	const Vector2 origin	= {10,10};
	const float spacing 	= 10.0;
	float offset 			= origin.y;
	Vector2 resolution		= v->screen_resolution;


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

	sprintf(fmt_text_buffer, "Update time: %f", game->update_time);
	GuiLabel(item_bounds, fmt_text_buffer);

	item_bounds.y += spacing;

	sprintf(fmt_text_buffer, "Frame time: %f", GetFrameTime());
	GuiLabel(item_bounds, fmt_text_buffer);

	item_bounds.y += spacing;
	
	sprintf(fmt_text_buffer, "Camera zoom: %.1f", v->camera.zoom);
	GuiLabel(item_bounds, fmt_text_buffer);
}


void GameRenderer_debug_draw_cursor(ConstGameContext game) {
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
			GameRenderer_debug_draw_wireframes(game);
			EndMode2D();
		}

		if (bitmask32_has_flag(debug_flags,EDebug_display_performance)) 
		{
			GameRenderer_debug_draw_perf_stat_overlay(
					game,
					view		
			);
		}

		if (bitmask32_has_flag(debug_flags,EDebug_draw_movement_vectors)) 
		{
			BeginMode2D(view->camera);
			GameRenderer_debug_draw_move_vectors(game,view);
			EndMode2D();
		}

		if (bitmask32_has_flag(debug_flags,EDebug_draw_entity_data)) 
		{
			BeginMode2D(view->camera);
			GameRenderer_debug_draw_entity_info_hud(game,Vec2(0,0));
			EndMode2D();
		}


		GameRenderer_debug_draw_cursor(game);
}

