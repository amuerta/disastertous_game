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
#include <stdint.h>

// time
#include <sys/time.h>

// pthreads functionality
// and sleep
#include <pthread.h>
#include <unistd.h>

// cJSON
#include "../lib/cJSON/cJSON.h"

#pragma once

// consts
//
#define MICROSECONDS 1000
#define MILLISECONDS 1000
#define UPS 30
#define MAX_BB_COUNT  32
#define MAX_HB_COUNT  32
#define MAX_SBN_COUNT 16
#define MAX_ENT_FLAGS 64
#define DEFAULT_ENT_COUNT 256
#define RADIAN_DEG_RATIO 0.01745329

// types

typedef long long 		time_ms_t;
typedef size_t			relative_ptr;
typedef unsigned char 	byte;
typedef uint 			bitmask32_t;
typedef float 			multiplier;


// MACROS
#define STATIC_ARRAY_LEN(x)  (sizeof(x) / sizeof((x)[0]))

// structs :: units

typedef enum {
	EMove_free,
	EMove_face,
	EMove_point,
	EMove_face_to_point
} MoveScheme;

typedef struct {
	uint16_t 	id;
	enum {
		flag_int,
		flag_float,
		flag_bool
	} type;
	union { 
		int 	as_int; 
		float 	as_float;
		bool 	as_bool;
	} data;
} Flag;

typedef struct {
	float 	   	hp;
	float 		def;
	float 		shl;
	multiplier 	acceleration;
	multiplier 	collision_recoil;
	multiplier 	mass;
	multiplier 	dash_vel_mul;
} Stats;

typedef struct Entity {

	// if 0, then entity is unitilized
	byte			valid				;
	uint			unique_id			;
	MoveScheme 		move_type			;
	float			rotation_angle		;
	bool			is_clipping			;
	Vector2 	
				position,
				velocity,
				acceleration,
				move_destenation
			;
	Stats			properties						;
	Flag			flags[MAX_ENT_FLAGS]			;
	Rectangle 		bounding_box	[MAX_BB_COUNT]	;
	Rectangle 		hitbox			[MAX_HB_COUNT]	;
	struct Entity* 	subnodules	[MAX_SBN_COUNT]		;
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
	EDebug_draw_wireframes 			= 1,
	EDebug_draw_movement_vectors 	= 2,
	EDebug_draw_entity_data 		= 4,
	EDebug_display_performance 		= 8,
} DebugOptions;

typedef struct {
	// Window
	Vector2 	position;
	Vector2 	resolution;
	// Debug
	
	// GAMEPLAY
	float		base_acceleration;
	float		base_maximum_velocity;
	float 		base_dask_mul;
	float 		base_maximum_dash_mul;

} GameConfig;

typedef struct {
	// stuff like textures, shaders, models
} GameRenderer;

typedef struct {
	// TODO
} GameSoundEngine;


typedef enum {
	ECoreInputsIndexes_move_up				,
	ECoreInputsIndexes_move_down			,
	ECoreInputsIndexes_move_left			,
	ECoreInputsIndexes_move_right			,
	ECoreInputsIndexes_move_dash			,
	ECoreInputsIndexes_action_use_0		,
	ECoreInputsIndexes_action_use_1		,
	ECoreInputsIndexes_action_open_menu_0	,
	ECoreInputsIndexes_action_open_menu_1	,
	ECoreInputsIndexes_action_open_menu_2	,
	ECoreInputsIndexes_action_open_menu_3	,
	ECoreInputsIndexes_input_other			,
	ECoreInputsIndexes_Terminator,
} GameCoreInputsIndexes;

typedef enum {
	ECoreInputs_move_up				=	1,
	ECoreInputs_move_down			=	2,
	ECoreInputs_move_left			=	4,
	ECoreInputs_move_right			=	8,
	ECoreInputs_move_dash			=	16,
	ECoreInputs_action_use_0		=	32,
	ECoreInputs_action_use_1		=	64,
	ECoreInputs_action_open_menu_0	=	128,
	ECoreInputs_action_open_menu_1	=	256,
	ECoreInputs_action_open_menu_2	=	512,
	ECoreInputs_action_open_menu_3	= 	1024,
	ECoreInputs_input_other			= 	2048,
	ECoreInputs_Terminator,
} GameCoreInputs;

#define CORE_INPUTS_COUNT ((size_t)ECoreInputsIndexes_Terminator)


const GameCoreInputs COREINPUTS_LOOKUP_TABLE[] = {
	ECoreInputs_move_up				,
	ECoreInputs_move_down			,
	ECoreInputs_move_left			,
	ECoreInputs_move_right			,
	ECoreInputs_move_dash			,
	ECoreInputs_action_use_0		,
	ECoreInputs_action_use_1		,
	ECoreInputs_action_open_menu_0	,
	ECoreInputs_action_open_menu_1	,
	ECoreInputs_action_open_menu_2	,
	ECoreInputs_action_open_menu_3	,
	ECoreInputs_input_other			
};

typedef struct {
	bool			should_update;
	float 			update_time;
	float 			wait_time;
	
	bitmask32_t		debug_flags;
	bitmask32_t		player_input;
	

	relative_ptr 	player_rptr;

	Entity*			entites;
	size_t			entites_count;
	size_t 			entites_capacity;

	MapData			map;
	GameConfig 		config;
} GameContext;

typedef const GameContext* const 		ConstGameContext;
typedef void 							(*GameUpdateProc) 
											(GameContext* const game);
