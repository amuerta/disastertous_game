#include "main.h"


void Entity_apply_force(Entity* ent, Vector2 force) {
	ent->acceleration = Vector2Add(ent->acceleration,force);
	//printf("applying acceleration\n");
}

void Entity_move_as_free(Entity* ent) {
	ent->position = Vector2Add(ent->position,ent->velocity);
}

void Entity_move_to_face(Entity* ent) {
	/* float vel = Vector2Length(ent->velocity); */
	/* ent->position = Vector2Scale(ent->facing_direction,vel); */
	Vector2 move_vec = Vector2Rotate(ent->velocity,ent->rotation_angle);
	ent->position = Vector2Add(ent->position,move_vec);
}

void Entity_move_to_point(Entity* ent) {
}

Rectangle Entity_get_rect_from_boundingbox(Entity ent) {
	Vector2 	pos = ent.position;
	Rectangle	bb	= ent.bounding_box[0];
	Rectangle rec = {
		.x = pos.x,
		.y = pos.y,
		.width 	= fabs(bb.x) + bb.width,
		.height = fabs(bb.y) + bb.height,
	};
	return rec;
}
