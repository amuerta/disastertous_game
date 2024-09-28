#include "main.h"

Vector2 Vec2(float x, float y) {
	return (Vector2) {x,y};
}


void ReorederRectanglePoints(Vector2* points) {
	Vector2 temp = points[2];
	points[2] = points[3];
	points[3] = temp;
}


Vector2* GetRotatedRectanglePoints(Rectangle rec,Vector2 origin,float rotation) {
    Vector2 topLeft = { 0 };
    Vector2 topRight = { 0 };
    Vector2 bottomLeft = { 0 };
    Vector2 bottomRight = { 0 };
	// Only calculate rotation if needed
    if (rotation == 0.0f)
    {
        float x = rec.x - origin.x;
        float y = rec.y - origin.y;
        topLeft = (Vector2){ x, y };
        topRight = (Vector2){ x + rec.width, y };
        bottomLeft = (Vector2){ x, y + rec.height };
        bottomRight = (Vector2){ x + rec.width, y + rec.height };
    }
    else
    {
        float sinRotation = sinf(rotation*RADIAN_DEG_RATIO);
        float cosRotation = cosf(rotation*RADIAN_DEG_RATIO);
        float x = rec.x;
        float y = rec.y;
        float dx = -origin.x;
        float dy = -origin.y;

        topLeft.x = x + dx*cosRotation - dy*sinRotation;
        topLeft.y = y + dx*sinRotation + dy*cosRotation;

        topRight.x = x + (dx + rec.width)*cosRotation - dy*sinRotation;
        topRight.y = y + (dx + rec.width)*sinRotation + dy*cosRotation;

        bottomLeft.x = x + dx*cosRotation - (dy + rec.height)*sinRotation;
        bottomLeft.y = y + dx*sinRotation + (dy + rec.height)*cosRotation;

        bottomRight.x = x + (dx + rec.width)*cosRotation - (dy + rec.height)*sinRotation;
        bottomRight.y = y + (dx + rec.width)*sinRotation + (dy + rec.height)*cosRotation;
    }
	static Vector2 points[4] = {0};
	points[0] =	topLeft;
	points[1] =	topRight;
	points[2] =	bottomLeft;
	points[3] =	bottomRight;
	return points;
}

void GetRotatedRectanglePointsStorage(Vector2* s,Rectangle rec,Vector2 origin,float rotation) {
    Vector2 topLeft = { 0 };
    Vector2 topRight = { 0 };
    Vector2 bottomLeft = { 0 };
    Vector2 bottomRight = { 0 };
	// Only calculate rotation if needed
    if (rotation == 0.0f)
    {
        float x = rec.x - origin.x;
        float y = rec.y - origin.y;
        topLeft = (Vector2){ x, y };
        topRight = (Vector2){ x + rec.width, y };
        bottomLeft = (Vector2){ x, y + rec.height };
        bottomRight = (Vector2){ x + rec.width, y + rec.height };
    }
    else
    {
        float sinRotation = sinf(rotation*RADIAN_DEG_RATIO);
        float cosRotation = cosf(rotation*RADIAN_DEG_RATIO);
        float x = rec.x;
        float y = rec.y;
        float dx = -origin.x;
        float dy = -origin.y;

        topLeft.x = x + dx*cosRotation - dy*sinRotation;
        topLeft.y = y + dx*sinRotation + dy*cosRotation;

        topRight.x = x + (dx + rec.width)*cosRotation - dy*sinRotation;
        topRight.y = y + (dx + rec.width)*sinRotation + dy*cosRotation;

        bottomLeft.x = x + dx*cosRotation - (dy + rec.height)*sinRotation;
        bottomLeft.y = y + dx*sinRotation + (dy + rec.height)*cosRotation;

        bottomRight.x = x + (dx + rec.width)*cosRotation - (dy + rec.height)*sinRotation;
        bottomRight.y = y + (dx + rec.width)*sinRotation + (dy + rec.height)*cosRotation;
    }
	s[0] =	topLeft;
	s[1] =	topRight;
	s[2] =	bottomLeft;
	s[3] =	bottomRight;
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


void DrawRectangleLinesPro(
		Rectangle rec,
		Vector2 origin,
		float rotation,
		float thickness,
		Color line_col) 
{
	const size_t count = 4;
	Vector2* points = GetRotatedRectanglePoints(rec,origin,rotation);
	ReorederRectanglePoints(points);

	for(uint i = 0; i < count; i++) {
		DrawLineEx(points[i],points[(i+1) % count],thickness,line_col);
	}
}

#define ALOT 100000000
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

bool CheckCollisionRecsSAT(
		Rectangle r1, 
		Rectangle r2, 
		Vector2 origin_r1,
		Vector2 origin_r2,
		float angle_r1, 
		float angle_r2) 
{
	Vector2 *points_r1, *points_r2;
	points_r1 = GetRotatedRectanglePoints(r1,origin_r1,angle_r1);
	points_r2 = GetRotatedRectanglePoints(r2,origin_r2,angle_r2);

	ReorederRectanglePoints(points_r1);
	ReorederRectanglePoints(points_r2);

	const size_t vertices = 4;

	for(uint shape = 0; shape < 2; shape++) {

		for (uint i = 0; i < vertices; i++) {
			Vector2 p1 = points_r1[	(i)	];
			Vector2 p2 = points_r1[	(i+1)%vertices	];

			Vector2 edge = { p2.x-p1.x, p2.y-p1.y  };
			Vector2 normal = { -edge.y, edge.x	};

			float min_r1 = -ALOT, max_r1 = -ALOT;

		}
	}


	return false;
}
