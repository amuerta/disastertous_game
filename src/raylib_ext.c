#include "main.h"

Vector2 Vec2(float x, float y) {
	return (Vector2) {x,y};
}


Vector2 Vector2Min(Vector2 v1, Vector2 v2)
{
    Vector2 result = { 0 };

    result.x = fminf(v1.x, v2.x);
    result.y = fminf(v1.y, v2.y);

    return result;
}

Vector2 Vector2Abs(Vector2 v1)
{
    return Vec2(fabs(v1.x),fabs(v1.y));
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
