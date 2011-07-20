#pragma once

enum Damage 
{
	Damage_Physical,
	Damage_Fire,
	Damage_Ice,
	Damage_Lighting,
	Damage_Cure,
	Damage_Hack
};

enum Faction 
{
	Faction_Player,
	Faction_Enemy,
	Faction_None,
};

enum Buttons
{
	Button_ToggleFullscreen,
	Button_ToggleSlowMotion,
	Button_ToggleVSync,
	Button_ToggleWideScreen,
	Button_Exit,
	Button_Action,
	Button_Left,
	Button_Right,
	Button_Up,
	Button_Down,
	Button_ForceQuit,
	Button_All,
	Button_SwitchPlayer,
	Button_Items
};

enum Direction
{	
	Direction_None,
	Direction_Left,
	Direction_Right,
	Direction_Up,
	Direction_Down
};

enum DialoguePlacement
{
	DialoguePlacement_None,
	DialoguePlacement_Top,
	DialoguePlacement_Bottom,
	DialoguePlacement_Battle
};