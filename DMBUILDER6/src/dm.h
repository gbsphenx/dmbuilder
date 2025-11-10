//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Master enums
//------------------------------------------------------------------------------

#ifndef DUNGEON_TYPES_H
#define DUNGEON_TYPES_H

enum DUNGEONTYPE
{
	dungeon_Unknown = -1,
	dungeon_Master = 0,
	dungeon_Chaos = 1,
	dungeon_TheronQuest = 2,
	dungeon_Skullkeep = 3,
};

enum TILESETTYPE
{
	_DM2_Tileset_Void		= 0,
	_DM2_Tileset_Thicket	= 1,
	_DM2_Tileset_Cave		= 2,
	_DM2_Tileset_Keep		= 3,
	_DM2_Tileset_KeepRoof	= 4,
	_DM2_Tileset_FogThicket = 5,
};

enum FACINGTYPE
{
	facing_North = 0,
	facing_East  = 1,
	facing_South = 2,
	facing_West  = 3
};

enum PORTRAITTYPE
{
	portrait_Elija	= 0,
	portrait_Halk	= 1,
	portrait_Syra	= 2,
	portrait_Hissssa	= 3,
	portrait_Zed	= 4,
	portrait_Chani	= 5,
	portrait_Hawk	= 6,
	portrait_Boris	= 7,
	portrait_Mophus	= 8,
	portrait_Leif	= 9,
	portrait_WuTse	= 10,
	portrait_Alex	= 11,
	portrait_Linflas	= 12,
	portrait_Azizi	= 13,
	portrait_Aido	= 14,
	portrait_Gando	= 15,
	portrait_Stamm	= 16,
	portrait_Leyla	= 17,
	portrait_Tiggy	= 18,
	portrait_Sonja	= 19,
	portrait_Nabi	= 20,
	portrait_Gothmog	= 21,
	portrait_Wuuf	= 22,
	portrait_Daroou	= 23,
	portrait_Max	= 24
};

enum TILETYPE
{
 tile_Wall		= 0,
 tile_Floor		= 1,
 tile_Pit		= 2,
 tile_Stairs	= 3,
 tile_Door		= 4,
 tile_Teleport	= 5,
 tile_Trickwall	= 6,
 tile_Extern	= 7,
 tile_Max		= 8
};

enum DOORTYPE
{
 door_IronGate	= 0,
 door_Wooden	= 1,
 door_Metal		= 2,
 door_ForceField= 3,
 door_Max		= 4
};

enum ORNATETYPE
{
 ornate_SquareGrid		=  0,
 ornate_MetalBars		=  1,
 ornate_Jewels			=  2,
 ornate_WoodVen			=  3,
 ornate_ArchedGrid		=  4,
 ornate_BlockLock		=  5,
 ornate_CornerMetal		=  6,
 ornate_Ebony			=  7,
 ornate_RedTriangleLock	=  8,
 ornate_TriangleLock	=  9,	// never used in DM
 ornate_BlueField		= 10,
 ornate_DamagedMetal	= 11,
 ornate_Max				= 12
};

enum FLOORTYPE
{
 floor_SquareGrid	= 0,
 floor_SquarePad	= 1,
 floor_PoisonPuddle	= 2,
 floor_RoundGrid	= 3,
 floor_HexagonalPad	= 4,
 floor_FirePot		= 5,
 floor_GreyMark		= 6,
 floor_TinyPad		= 7,
 floor_WaterPuddle	= 8,
 floor_Max			= 9
};

enum MONSTERTYPE
{
 monster_Scorpion		=	0,		// Giant Scorpion		
 monster_Slime			=	1,		// Swamp Slime			// Slime Devil
 monster_Giggler		=	2,		// Giggler				
 monster_Beholder		=	3,		// Wizard Eye			// Gazer
 monster_Rat			=	4,		// Pain Rat				// Hell Hound
 monster_Ruster			=	5,		// Ruster				
 monster_Screamer		=	6,		// Screamer			
 monster_RockPile		=	7,		// Rockpile			
 monster_Ghost			=	8,		// Ghost				// Rive
 monster_StoneGolem		=	9,		// Stone Golem			
 monster_Mummy			=	0x0A,	// Mummy			
 monster_BlackFlame		=	11,		// Black Flame		
 monster_Skeleton		=	12,		// Skeleton			
 monster_Couatl			=	13,		// Couatl			
 monster_Jawa			=	14,		// Vexirk				// Mon Vexirk
 monster_Worm			=	15,		// Magenta Worm			// Armoured Worm
 monster_Trolin			=	0x10,	// Trolin				// Ant Man
 monster_Wasp			=	17,		// Giant Wasp			// Muncher
 monster_Knight			=	18,		// Animated Armour		// Deth Knight
 monster_Fader			=	19,		// Materializer			// Zytaz
 monster_WaterElemental	=	20,		// Water Elemental		
 monster_Oitu			=	21,		// Oitu					// Greater Oitu
 monster_Demon			=	22,		// Demon				// Viper Demon
 monster_LordChaos		=	23,		// Lord Chaos
 monster_Dragon			=	24,		// Red Dragon
 monster_EvilLord		=	25,		// Lord Order
 monster_GreyLord		=	26,		// Grey Lord

 monster_Max			=	27
};

//..............................................................................

enum WALLTYPE
{
 wall_Nothing			=	0x00,
 wall_SquareAlcove		=	0x01,
 wall_ViAltar			=	0x02,
 wall_OrnateAlcove		=	0x03,
 wall_Hook				=	0x04,
 wall_BlockLock			=	0x05,
 wall_WoodRing			=	0x06,
 wall_SmallSwitch		=	0x07,
 wall_Dent1				=	0x08,
 wall_Dent2				=	0x09,
 wall_IronRing			=	0x0a,
 wall_Crack				=	0x0b,
 wall_SlimeOutlet		=	0x0c,
 wall_Point				=	0x0d,
 wall_TinySwitch		=	0x0e,
 wall_GreenButtonOut	=	0x0f,
 wall_BlueButtonOut		=	0x10,
 wall_CoinSlot			=	0x11,
 wall_DoubleLock		=	0x12,
 wall_SquareLock		=	0x13,
 wall_WingedLock		=	0x14,
 wall_OnyxLock			=	0x15,
 wall_StoneLock			=	0x16,
 wall_CrossLock			=	0x17,
 wall_JeweledLock		=	0x18,
 wall_SkeletonLock		=	0x19,
 wall_GoldLock			=	0x1a,
 wall_TurquoiseLock		=	0x1b,
 wall_EmeraldLock		=	0x1c,
 wall_RubyLock			=	0x1d,
 wall_RaLock			=	0x1e,
 wall_MasterLock		=	0x1f,
 wall_GemHole			=	0x20,
 wall_Slime				=	0x21,
 wall_Grate				=	0x22,
 wall_Fountain			=	0x23,
 wall_Menacles			=	0x24,
 wall_DemonFace			=	0x25,
 wall_TorchHolderEmpty	=	0x26,
 wall_DragonClaws		=	0x27,
 wall_PoisonHoles		=	0x28,
 wall_FireballHoles		=	0x29,
 wall_DaggerHoles		=	0x2a,
 wall_HeroMirror		=	0x2b,
 wall_LeverUp			=	0x2c,
 wall_LeverDown			=	0x2d,
 wall_TorchHolderFull	=	0x2e,
 wall_RedButtonOut		=	0x2f,
 wall_EyeSwitch			=	0x30,
 wall_BigSwitchOut		=	0x31,
 wall_CrackSwitchOut	=	0x32,
 wall_GreenButtonIn		=	0x33,
 wall_BlueButtonIn		=	0x34,
 wall_RedButtonIn		=	0x35,
 wall_BigSwitchIn		=	0x36,
 wall_CrackSwitchIn		=	0x37,
 wall_Amalgam			=	0x38,
 wall_PoweredAmalgam	=	0x39,
 wall_BurntAmalgam		=	0x3a,
 wall_EvilOutside		=	0x3b,

 wall_Max				=	0x3c
};

enum WEAPONTYPE
{
 weapon_EyeOfTime	=	0x00,
 weapon_StormRing	=	0x01,
 weapon_Torch		=	0x02,
 weapon_Flamitt		=	0x03,
 weapon_StaffOfClaws= 0x04,
 weapon_BoltBlade	=	0x05,
	weapon_Storm	= 5,
 weapon_Fury		=	0x06,
	weapon_RaBlade      =  6,
 weapon_TheFirestaffA=0x07,
 weapon_Dagger		=	0x08,
 weapon_Falchion	=	0x09,
 weapon_Sword		=	0x0a,
 weapon_Rapier		=	0x0b,
 weapon_Sabre		=	0x0c,
	weapon_Biter        = 12,
 weapon_SamuraiSword=	0x0d,
 weapon_Delta		=	0x0e,
	weapon_SideSplitter = 14,
 weapon_DiamondEdge=	0x0f,
 weapon_VorpalBlade=	0x10,
 weapon_TheInquisitor=	0x11,
	 weapon_DragonFang   = 17,
 weapon_Axe			=	0x12,
 weapon_HardCleave	=	0x13,
	weapon_Executioner  = 19,
 weapon_Mace		=	0x14,
 weapon_MaceOfOrder=	0x15,
 weapon_Morningstar	=	0x16,
 weapon_Club		=	0x17,
 weapon_StoneClub	=	0x18,
 weapon_Bow			=	0x19,
	weapon_ClawBow      = 25,
 weapon_Crossbow	=	0x1a,
 weapon_Arrow		=	0x1b,
 weapon_Slayer		=	0x1c,
 weapon_Sling		=	0x1d,
 weapon_Rock		=	0x1e,
 weapon_PoisonDart	=	0x1f,
 weapon_ThrowingStar=	0x20,
 weapon_Stick		=	0x21,
 weapon_Staff		=	0x22,
 weapon_Wand		=	0x23,
 weapon_TeoWand		=	0x24,
 weapon_YewStaff	=	0x25,
 weapon_StaffOfManar=	0x26,
	weapon_StaffOfIrra  = 38,
 weapon_SnakeStaff	=	0x27,
	weapon_CrossOfNeta  = 39,
 weapon_TheConduit	=	0x28,
	weapon_SerpentStaff = 40,
 weapon_DragonSpit	=	0x29,
 weapon_SceptreOfLyf=	0x2a,
 weapon_HornOfFear=	0x2b,
 weapon_Speedbow	=	0x2c,
 weapon_TheFirestaffB=0x2d,

 weapon_Max			=	0x2e
};
//------------------------------------------------------------------------------

enum CLOTHINGTYPE
{
 clothing_Cape          =  0,
 clothing_CloakOfNight  =  1,
 clothing_BarbarianHide=0x02,
	clothing_TatteredPants =  2,
 clothing_Sandals       =  3,
  clothing_LeatherBoots  =  4,
  
  clothing_RobeA	=	0x05,
	clothing_TatteredShirt =  5,
  
 clothing_RobeB          =  6,
  clothing_FineRobeA     =  7,
  clothing_FineRobeB     =  8,
 clothing_Kirtle        =  9,
  clothing_SilkShirt     = 10,
  clothing_Tabard        = 11,
  clothing_Gunna         = 12,
  clothing_ElvenDoublet  = 13,
  clothing_ElvenHuke     = 14,
  clothing_ElvenBoots    = 15,
  clothing_LeatherJerkin = 16,
  clothing_LeatherPants  = 17,
  clothing_SuedeBoots    = 18,
  clothing_BluePants     = 19,
  clothing_Tunic         = 20,
  clothing_Ghi           = 21,
  clothing_GhiTrousers   = 22,
  clothing_Calista       = 23,
  clothing_CrownOfNerra  = 24,
  clothing_BezerkerHelm  = 25,
  clothing_Helmet        = 26,
  clothing_Basinet       = 27,
 
  clothing_Buckler		=	0x1c,
	clothing_NetaShield    = 28,
  
 clothing_HideShield	=	0x1d,
	clothing_CrystalShield = 29,
  
clothing_WoodenShield=	0x1e,
 clothing_SmallShield   = 31,
  clothing_MailAketon    = 32,
  clothing_LegMail       = 33,
  clothing_MithralAketon = 34,
  clothing_MithralMail   = 35,
  clothing_CasqueNCoif   = 36,
  clothing_Hosen         = 37,
  clothing_Armet         = 38,
  clothing_TorsoPlate    = 39,
  clothing_LegPlate      = 40,
  clothing_FootPlate     = 41,
  
  clothing_LargeShield=	0x2a,
	 clothing_SarShield     = 42,
 
 clothing_HelmOfLyte=	0x2b,
 clothing_HelmOfRa      = 43,
  
 clothing_PlateOfLyte=	0x2c,
 clothing_PlateOfRa     = 44,
  
 clothing_PoleynOfLyte=	0x2d,
 clothing_PoleynOfRa    = 45,
  
 clothing_GreavesOfLyte=	0x2e,
 clothing_GreaveOfRa    = 46,
  
 clothing_ShieldOfLyte=	0x2f,
clothing_ShieldOfRa    = 47,
  
 clothing_HelmOfDarc=	0x30,
 clothing_DragonHelm    = 48,
  
 clothing_PlateOfDarc=	0x31,
 clothing_DragonPlate   = 49,
  
 clothing_PoleynOfDarc=	0x32,
 clothing_DragonPoleyn  = 50,
  
 clothing_GreavesOfDarc=	0x33,
 clothing_DragonGreave  = 51,
  
 clothing_ShieldOfDarc=	0x34,
 clothing_DragonShield  = 52,
  
 clothing_Dexhelm       = 53,
  clothing_Flamebain     = 54,
  clothing_PowerTowers   = 55,
  clothing_BootsOfSpeed  = 56,
  clothing_Halter        = 57,

 clothing_Max			=	0x3a,
};


//------------------------------------------------------------------------------


enum
{
	potion_MonPotion	=  0,	// ven potion gfx, doesn't work 
	potion_UmPotion		=  1,	// ?
	potion_DesPotion	=  2,	// ?
	potion_VenPotion	=  3,
	potion_SarPotion	=  4,	// ?
	potion_ZoPotion		=  5,	// ?
	potion_RosPotion	=  6,
	potion_KuPotion		=  7,
	potion_DanePotion	=  8,
	potion_NetaPotion	=  9,
	potion_AntiVenin	= 10,
	potion_MaPotion		= 11,	// Mon Symbol
	potion_YaPotion		= 12,
	potion_EePotion		= 13,	// Bro Symbol
	potion_ViPotion		= 14,
	potion_WaterFlask	= 15,
	potion_KathBomb		= 16,	// ?
	potion_PewBomb		= 17,	// ?
	potion_RaBomb		= 18,	// ?
	potion_FulBomb		= 19,
	potion_EmptyFlask	= 20,

	potion_Max			= 21
};
//------------------------------------------------------------------------------


enum MISCTYPE
{
	misc_Compass        = 0,
 misc_Waterskin      = 1,
 misc_JewelSymal     = 2,
 misc_Illumulet      = 3,
 misc_Ashes          = 4,
 misc_BonesOfHero    = 5, //value = character index
 	misc_CopperCoin =		0x06,
misc_SarCoin        = 6,
 
misc_SilverCoin     = 7,
 	misc_GoldCoin =		0x08,
misc_GorCoin        = 8,
 
	misc_IronKey        = 9,
 misc_KeyOfB         = 10,
 misc_SolidKey       = 11,
 misc_SquareKey      = 12,
 misc_TourquoiseKey  = 13,
 misc_CrossKey       = 14,
 misc_OnyxKey        = 15,
 misc_SkeletonKey    = 16,
 misc_GoldKey        = 17,
 misc_WingedKey      = 18,
 misc_TopazKey       = 19,
 misc_SapphireKey    = 20,
 misc_EmeraldKey     = 21,
 misc_RubyKey        = 22,
 misc_RaKey          = 23,
 misc_MasterKey      = 24,
 misc_Boulder        = 25,
 misc_BlueGem        = 26,
 misc_OrangeGem      = 27,
 misc_GreenGem       = 28,
 misc_Apple          = 29,
 misc_Corn           = 30,
 misc_Bread          = 31,
 misc_Cheese         = 32,
 misc_ScreamerSlice  = 33,
 misc_WormRound      = 34,
 	misc_Drumstick =		0x23,
	misc_Shank          = 35,
 misc_DragonSteak    = 36,
	misc_GemOfAges      = 37,
 misc_EkkhardCross   = 38,
 misc_Moonstone      = 39,
 misc_TheHellion     = 40,
 misc_PendantFeral   = 41,
 misc_MagicalBoxBlue   = 42,
 misc_MagicalBoxGreen    = 43,
 misc_MirrorOfDawn   = 44,
 misc_Rope           = 45,
 misc_RabbitsFoot    = 46,
misc_Corbamite =		0x2f,
misc_Corbum         = 47,
 misc_Choker         = 48,
 misc_LockPicks      = 49,
 misc_Magnifier      = 50,
 misc_ZokathraSpell  = 51,
 misc_Bones          = 52,
	misc_Max =				53
	// soit 7 bits
};

enum SPELLTYPE
{
	spell_Fireball = 0,
	spell_PoisonBlob = 1,
	spell_Lightning = 2,
	spell_Antimaterial = 3,
	spell_ZoSpell = 4,
	spell_Fuse = 5,
	spell_PoisonBolt = 6,
	spell_PoisonCloud = 7,

	spell_Max = 8
};

enum POWERTYPE
{
	power_None = 0,
	power_Lo = 1,
	power_Um = 2,
	power_On = 3,
	power_Ee = 4,
	power_Pal = 5,
	power_Mon = 6,
	power_Max = 7,
};

/* Catégories des objets */
enum CATEGORYTYPE
{
	category_Door		= 0x00,
	category_Teleport	= 0x01,
	category_Text		= 0x02,
	category_Actuator	= 0x03,
	category_Monster	= 0x04,
	category_Weapon		= 0x05,
	category_Clothing	= 0x06,
	category_Scroll		= 0x07,
	category_Potion		= 0x08,
	category_Chest		= 0x09,
	category_Miscs		= 0x0A,
	category_Unused1	= 0x0B,
		category_Actuator2	= 0x0B,
	category_Unused2	= 0x0C,
	category_Unused3	= 0x0D,
	category_Missile	= 0x0E,
	category_Cloud		= 0x0F
};

enum ACTIVATORSITEMTYPE
{
	item_actuators_CompassNorth = 0,
	item_actuators_CompassEast = 1,
	item_actuators_CompassSouth = 2,
	item_actuators_CompassWest = 3,
	item_actuators_Torch = 4,
	item_actuators_TorchAlmostNew = 5,
	item_actuators_TorchAlmostBurnt = 6,
	item_actuators_TorchBurntOut = 7,
	item_actuators_WaterSkin = 8,
	item_actuators_WaterSkinEmpty = 9,
	item_actuators_JewelSymal = 0xa,
	item_actuators_JewelSymalUsed = 0xB,
	item_actuators_Illumulet = 0xC,
	item_actuators_IllumuletUsed = 0xD,
	item_actuators_Flamitt = 0xe,
	item_actuators_FlamittUsed = 0xf,
	item_actuators_EyeOfTime = 16,
	item_actuators_EyeOfTimeUsed = 17,
	item_actuators_StormRing = 18,
	item_actuators_StormRingUsed = 19,
	item_actuators_StaffOfClaws = 20,
	item_actuators_StaffOfClawsUsed = 21,
	item_actuators_StaffOfClawsYellow =22,
	item_actuators_BoltBlade =23,
	item_actuators_BoltBladeUsed =24,
	item_actuators_Fury =25,
	item_actuators_FuryUsed =26,
	item_actuators_TheFirestaffA =27,
	item_actuators_TheFirestaffB =28,
	
	special_Chest = 29,
	special_Scroll = 31,
	special_OpenScroll = 32,
	special_OpenChest = 30,

	special_Max = 33,

	special_TextWall = 34,
	special_TextScroll1 = 35,
	special_TextScroll2 = 36,

	item_actuators_Max = 0xc7,
};

enum TELEPORTERSCOPE
{
	//{"OBJECT","CREATURE","PARTY or OBJECT","EVERYTHING"};
	teleporter_Scope_Object = 0,
	teleporter_Scope_Creature = 1,
	teleporter_Scope_PartyObject = 2,
	teleporter_Scope_Anything = 3,
};

enum TELEPORTERROTABS
{
	teleporter_Rotation_Relative = 0,
	teleporter_Rotation_Absolute = 1,
};

enum ACTIVATION_STATUS
{
	ACT_OFF = 0,
	ACT_ON = 1,
};


enum ACTUATORSCOPE	// specifically for PAD
{
	actuator_Scope_None = 0,
	actuator_Scope_Any = 1,
	actuator_Scope_Party = 3,
	actuator_Scope_Item = 4,
};

#endif