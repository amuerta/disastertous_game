#include <raylib.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../src/raylib_ext.c"

void DrawGrid2D(float spacing, uint lines,Color color) {
	Vector2 root = {
		-((lines/2) * spacing),
		-((lines/2) * spacing)
	};

	for(uint x = 0; x < lines; x++) {
		for(uint y = 0; y < lines; y++) {
			Vector2 begin_x = { x*spacing, y*spacing };
			Vector2 end_x   = { lines*spacing, y*spacing};

			Vector2 begin_y = { x*spacing, y*spacing };
			Vector2 end_y   = { x*spacing, lines*spacing};
			DrawLineEx(begin_x,end_x,1,color);
			DrawLineEx(begin_y,end_y,1,color);
		}
	}

}

void DrawAnyPolyLines(
		Vector2* points,
		size_t count,
		float thichness,
		Color color) 
{
	for(uint i = 0; i < count; i++) {
		DrawLineEx(points[i],points[(i+1)%count],thichness,color);
	}
}

Vector2 Vector2SubRet(Vector2 v1, Vector2 v2) {
	return (Vector2) {v1.x - v2.x, v1.y - v2.y};
}

void minkowsi_diff(Vector2* poly, Vector2 *shape1, Vector2 *shape2, size_t polyc1, size_t polyc2) {
	uint index = 0;
	for(uint i = 0; i < polyc1; i++) 
	{
		for(uint j = 0; j < polyc2; j++) 
		{
			poly[index] = Vector2Subtract(shape1[i] , shape2[j]);
			index++;
		}
	}
}

bool poly_collide(Vector2* p1, Vector2* p2,size_t vertecies) {
	Vector2 mink[16];
	Vector2 hull[8];
	minkowsi_diff(mink,p1,p2,vertecies,vertecies);

	for(uint i = 0; i < vertecies; i++) {
		//if (mink[i])
	}
}

#define ALOT 100000000

bool check_collision_recs_scuffed(Vector2 *lines1, Vector2 *lines2) {
	const size_t COUNT = 4;

	float min_r1 = -ALOT;
	float min_r2 = -ALOT;
	float max_r1 =  ALOT;
	float max_r2 =  ALOT;
	
	// find projection for REC1
	for(uint i = 0; i < COUNT; i++) {
		Vector2 p1 = lines1[	(i)	];
		Vector2 p2 = lines1[	(i+1)%COUNT];
		Vector2 side = Vector2Subtract(p2,p1);
		Vector2 normal = { -side.y, side.x	};
		float projection = Vector2DotProduct(p1,normal);
		printf("projection from point %i: %f\n",i,projection);

		min_r1 = MIN(min_r1,projection);
		max_r1 = MAX(max_r1,projection);
	}
	
	// find projection for REC2
	for(uint i = 0; i < COUNT; i++) {
		Vector2 p1 = lines2[	(i)	];
		Vector2 p2 = lines2[	(i+1)%COUNT];
		Vector2 side = Vector2Subtract(p2,p1);
		Vector2 normal = { -side.y, side.x	};
		float projection = Vector2DotProduct(p1,normal);
		printf("projection from point %i: %f\n",i,projection);

		min_r2 = MIN(min_r2,projection);
		max_r2 = MAX(max_r2,projection);
	}

	
	if (max_r1 < min_r2 || max_r2 > min_r1) {
		return false;
	} else {

	}

}

// Project a rectangle onto an axis
void projectRectangleOntoAxis(Vector2* vertices, Vector2 axis, float* min, float* max) {
    *min = Vector2DotProduct(vertices[0], axis);
    *max = *min;

    for (int i = 1; i < 4; i++) {
        float projection = Vector2DotProduct(vertices[i], axis);
        if (projection < *min) *min = projection;
        if (projection > *max) *max = projection;
    }
}

// Check if projections overlap on an axis
bool overlapOnAxis(Vector2* verticesA, Vector2* verticesB, Vector2 axis) {
    float minA, maxA, minB, maxB;

    projectRectangleOntoAxis(verticesA, axis, &minA, &maxA);
    projectRectangleOntoAxis(verticesB, axis, &minB, &maxB);

    return !(maxA < minB || maxB < minA);
}

bool SATCheck(Vector2* verticesA, Vector2* verticesB) {

    Vector2 axes[8] = {
        Vector2Subtract(verticesA[0], verticesA[1]),  // Edge from rectA
        Vector2Subtract(verticesA[1], verticesA[3]),  // Edge from rectA
        Vector2Subtract(verticesA[3], verticesA[2]),  // Edge from rectA
        Vector2Subtract(verticesA[2], verticesA[0]),  // Edge from rectA
        Vector2Subtract(verticesB[0], verticesB[1]),  // Edge from rectB
        Vector2Subtract(verticesB[1], verticesB[3]),  // Edge from rectB
        Vector2Subtract(verticesB[3], verticesB[2]),  // Edge from rectB
        Vector2Subtract(verticesB[2], verticesB[0])  // Edge from rectB
    };

    for (int i = 0; i < 8; i++) {
        if (!overlapOnAxis(verticesA, verticesB, axes[i])) {
            return false;  
        }
    }
    return true;  
}


int main(void) {
	Vector2 screen = {1000,1000};
	Camera2D cam = {
		.target = {0,0},
		.offset = {screen.x/2,screen.y/2},
		.zoom = 1,
	};

	Rectangle r1 = {0,0,100,100};
	Rectangle r2 = {0,00,40,40};
	Vector2 sub_r[4] = {0};
	float rot = 0;

	SetTargetFPS(60);
	InitWindow(screen.x,screen.y,"");
	while(!WindowShouldClose()) {
		
		Vector2 mpos = {GetMouseX(), GetMouseY()};

		Vector2 pos1 	= {50,50};
		Vector2 pos2 	= {20,20};

		Vector2 storage1[4] = {0};
		Vector2 storage2[4] = {0};
		/*  */
		/* Vector2 *points_r1 = GetRotatedRectanglePointsStorage(storage1,r2,pos1,1); */
		/* Vector2 *points_r2 = GetRotatedRectanglePointsStorage(storage2,r1,pos2,0); */

		rot += GetFrameTime();

		GetRotatedRectanglePointsStorage(storage1,r2,pos1,rot / 0.0174);
		GetRotatedRectanglePointsStorage(storage2,r1,pos2,0/0.0174);

		Vector2* points_r1 = storage1;
		Vector2* points_r2 = storage2;


		Vector2 mink[4*4] = {0};
		minkowsi_diff(mink,points_r1,points_r2,4,4);
		//minkowski_diff_rect(mink, points_r1, points_r2);

		/* bool mcol_r1 = CheckCollisionPointPoly(GetScreenToWorld2D(mpos,cam),points_r1,4); */
		/* bool mcol_r2 = CheckCollisionPointPoly(GetScreenToWorld2D(mpos,cam),points_r2,4); */

		bool mcol_r1 = CheckCollisionPointRec(GetScreenToWorld2D(mpos,cam),r1);
		bool mcol_r2 = CheckCollisionPointRec(GetScreenToWorld2D(mpos,cam),r2);

		for (uint i = 0; i < 4; i++) {
			printf("r1_p %i:\t %f,%f\n",i,points_r1[i].x,points_r1[i].y);
			printf("r2_p %i:\t %f,%f\n",i,points_r2[i].x,points_r2[i].y);
			sub_r[i] = Vector2Add(points_r1[i],points_r2[i]);
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))  {
			Vector2 delta = GetMouseDelta();

			if ( mcol_r1) {
				r1.x += delta.x;
				r1.y += delta.y;
			} else if (mcol_r2) {
				r2.x += delta.x;
				r2.y += delta.y;
			}
		}

		printf("sub_rec: (%.1f, %.1f) (%.1f, %.1f) (%.1f, %.1f) (%.1f, %.1f)\n",
				sub_r[0].x,
				sub_r[0].y,

				sub_r[1].x,
				sub_r[1].y,

				sub_r[2].x,
				sub_r[2].y,

				sub_r[3].x,
				sub_r[3].y
			);
		
		BeginDrawing();
		ClearBackground(GetColor(0x181818FF));
		//ClearBackground(RAYWHITE);
		
		DrawGrid2D(20,50,GetColor(0x303030FF));
		{

			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				DrawCircle(GetMouseX(),GetMouseY(),2,RED);

			/* if (check_collision_recs_scuffed(points_r1,points_r2)) */
			if (SATCheck(points_r1,points_r2))
				DrawText("FUCK",30,30,30,RED);

			BeginMode2D(cam);
			
			DrawCircle(0,0,2,RED);
			DrawRectangleLinesEx(r1,1,GREEN);
			DrawRectangleLinesEx(r2,1,GREEN);

			ReorederRectanglePoints(points_r1);
			ReorederRectanglePoints(points_r2);
			DrawAnyPolyLines(points_r1,4,1,ORANGE);
			DrawAnyPolyLines(points_r2,4,1,ORANGE);
			DrawAnyPolyLines(sub_r,4,1,RED);
			DrawAnyPolyLines(mink,16,1,BLUE);
			

			for(uint i = 0; i < 16; i++) {
				if (false) {
					DrawCircleV(mink[i],2,WHITE);
				} else 
					DrawCircleV(mink[i],2,YELLOW);
			}

			DrawCircle( r1.x-r2.x,r1.y-r2.y,2,BLUE);

			EndMode2D();
		}
		EndDrawing();


		
	}
	CloseWindow();
}
