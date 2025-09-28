//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Actuator
//------------------------------------------------------------------------------

#ifndef ACTUATOR_H
#define ACTUATOR_H


/* generated effect by actuator (0-7) */
enum
{
	actuator_effect_open = 0,
	actuator_effect_close = 1,
	actuator_effect_toggle = 2,
	actuator_effect_open_ncw = 3, // open, need constant weight/activation
	actuator_effect_open_out = 4, // open, when item removed or pad stepped out
	actuator_effect_close_out = 5,
	actuator_effect_toggle_out = 6,
	actuator_effect_close_ncw = 7 // close, need constant weight/activation,
};

/* actuator known types */
enum
{
	actuator_floor_none = 0,
	actuator_floor_everything = 1,
	actuator_floor_party_monster = 2,
	actuator_floor_party = 3,
	actuator_floor_pad_item = 4, 
	actuator_floor_monster_generator = 6,
	actuator_floor_monster = 7,
	actuator_floor_carried_item = 8,


	actuator_dm2_wall_champion_cell = 0x7E, /* means -2, used by DM2 */
	actuator_wall_champion_mirror = 0x7F, /* means -1 */
	actuator_wall_none = 0,
	actuator_wall_something = 1,
	actuator_wall_alcove_item = 2,
	actuator_wall_item = 3,
	actuator_wall_item_eater = 4,
	actuator_wall_trigger = 5,
	actuator_wall_counting_pad = 6,
	actuator_wall_spell_shooter = 8,
	actuator_wall_weapon_shooter = 9,
	actuator_wall_double_spell_shooter = 10,
	actuator_wall_item_eater_toggler = 11,
	actuator_wall_mouse_toggler = 12,
	actuator_wall_item_removal_toggler = 13,
	actuator_wall_general_shooter = 14,
	actuator_wall_double_general_shooter = 15,
	actuator_wall_item_exchange = 16,
	actuator_wall_item_eater_disappear = 17,
	actuator_end_pad = 18,
};

enum
{
	actuator_dm2_floor_cross_map_link = 39,
	
	
	actuator_dm2_wall_button_switch = 24,
	actuator_dm2_wall_generic_item_toggler = 26,
	actuator_dm2_wall_counter = 29,
	actuator_dm2_wall_relay_1 = 32,
	actuator_dm2_wall_alcove_item = 42,
	actuator_dm2_wall_monster_generator = 46,
	actuator_dm2_wall_item_generator = 60,

	actuator_dm2_wall_flag_inverse = 67,
	actuator_dm2_wall_flag_test = 68,

	actuator_dm2_item_capture_from_creature = 73,	// used for removing item from cauldron
	actuator_dm2_last = 73,
};


//--- DM2 simple actuator auto-functions
enum
{
	simple_actuator_function__still_image = 0,	/// like a stupid alcove
	simple_actuator_function__creature_transformer = 1,
	simple_actuator_function__auto_animated = 2,	/// fountain, fireplace, lantern
	simple_actuator_function__item_toggler = 4,	/// like torch holder
	simple_actuator_function__ladder_enabled = 6,
	simple_actuator_function__ladder_disabled = 7,
	simple_actuator_function__respawner = 8,	/// blue gem
	simple_actuator_function__bush = 10,	/// bush type
	simple_actuator_function__2state_switch = 13,	/// window : stop / animated
	simple_actuator_function__gdat_text = 14,	/// text into graphics.dat
	simple_actuator_function__teleporter = 15,
	simple_actuator_function__teleporter_flag = 16,
	simple_actuator_function__creature_spawn_on_trigger = 19,
/// there is room for 19 auto-functions not set/coded
};

//------------------------------------------------------------------------------

/* 1st byte */
typedef struct 
{
	unsigned short type:7;
	unsigned short value:9;
} dm_actuator_type;

typedef dm_actuator_type* actuator_p;

/* 2nd byte */
typedef struct 
{
	unsigned short bit1:1;
	unsigned short bit2:1;
	unsigned short once:1;
	unsigned short effect:3;
	unsigned short sound:1;
//	unsigned short cycle:1;
//	unsigned short delay:3;
	unsigned short delay:4;
	unsigned short inactive:1; //won't generate an effect
	unsigned short graphism:4;
} dm_actuator_effect;

typedef dm_actuator_effect* actuator_effect_p;

/* 3rd byte */
typedef struct  /* common use */
{
	unsigned short bit1:1;
	unsigned short bit2:1;
	unsigned short bit3:1;
	unsigned short bit4:1;
	unsigned short facing:2;
	unsigned short xdest:5;
	unsigned short ydest:5;
} dm_actuator_target;

typedef dm_actuator_target* actuator_target_p;

//------------------------------------------------------------------------------
typedef struct
{
	unsigned short unk2:2;
	unsigned short once:1;
	unsigned short effect:3;
	unsigned short unk1:1;
	unsigned short generation:4; /* gives number of generated monsters */
	unsigned short inactive:1; /* ALWAYS SET TO 1 */
	unsigned short graphism:4;
} dm_actuator_monster_generator; /* replaces part of effect */

typedef dm_actuator_monster_generator* a_2_monster_generator_p;

typedef struct
{
	unsigned char thoughness;
	unsigned char pause;
} dm_actuator_monster_regenerator;

typedef dm_actuator_monster_regenerator* a_3_monster_generator_p;

//------------------------------------------------------------------------------
/* Special Activators */
typedef struct 
{
	unsigned short unk:8;
	unsigned short power:4; // F is the most powerful
	unsigned short range:4; // 0 is infinite, F is shortest
} dm_actuator_shooter; /* replaces the target */

typedef dm_actuator_shooter* a_3_shooter_p;

#endif