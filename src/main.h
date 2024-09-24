// raylib
#include <raylib.h>
#include <raymath.h>
#define RAYGUI_IMPLEMENTATION
#include "../lib/raygui/src/raygui.c"

// libc
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// pthreads functionality
// and sleep
#include <pthread.h>
#include <unistd.h>

// cJSON
#include "../lib/cJSON/cJSON.h"


// consts
#define UPS 30
#define MAX_BB_COUNT 32
#define MAX_SBN_COUNT 16
#define DEFAULT_ENT_COUNT 256

// types

typedef size_t			relative_ptr;
typedef unsigned char 	byte;
typedef uint 			bitmask32_t;


// structs :: units
typedef struct Entity {
	byte	valid; // if 0, then entity is unitilized
	Vector2 	
		position,
		velocity,
		acceleration;
	Rectangle bounding_box		[MAX_BB_COUNT]	;
	struct Entity* subnodules	[MAX_SBN_COUNT]	;
} Entity;

typedef struct {
	Camera2D camera;
	Vector2  screen_resolution;
	// TODO: 
	// 	overlay stuff?
	//	shaders?
} Viewport ;

typedef struct {} MapData;

typedef struct {
	
} GameState;


typedef enum {
	Debug_draw_wireframes 		= 1,
	Debug_draw_movement_vectors = 2,
	Debug_display_performance 	= 4,
} DebugOptions;

typedef struct {
	// Window
	Vector2 	position;
	Vector2 	resolution;
	// Debug
	byte 		debug_info;
	// Game
	uint 		ups;
} GameConfig;

typedef struct {
	// stuff like textures, shaders, models
} GameRenderer;

typedef struct {
	// TODO
} GameSoundEngine;

typedef struct {
	bool			should_update;
	float			last_delta;
	bitmask32_t		debug_flags;

	relative_ptr 	player_rptr;

	Entity*			entites;
	size_t			entites_count;
	size_t 			entites_capacity;

	MapData			map;
} GameContext;
typedef const GameContext* const ConstGameContext;

