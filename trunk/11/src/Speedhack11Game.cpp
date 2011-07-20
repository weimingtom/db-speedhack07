#include "Speedhack11Game.hpp"

#include "Constants.hpp"
#include "Input.hpp"
#include "Music.hpp"
#include "ScreenManager.hpp"
#include "Sound.hpp"
#include "SplashScreen2.hpp"

Speedhack11Game::Speedhack11Game()
:Game("Speedhack 11", true)
{
	Input::registerKey(Button_ToggleFullscreen, KEY_F);
	Input::registerKey(Button_ToggleSlowMotion, KEY_S);
	Input::registerKey(Button_Exit, KEY_ESC);
	Input::registerKey(Button_ToggleVSync, KEY_V);
	Input::registerKey(Button_ToggleWideScreen, KEY_W);
	Input::registerKey(Button_Action, KEY_LCONTROL);
	Input::registerKey(Button_Up, KEY_UP);
	Input::registerKey(Button_Down, KEY_DOWN);
	Input::registerKey(Button_Left, KEY_LEFT);
	Input::registerKey(Button_Right, KEY_RIGHT);
	Input::registerKey(Button_ForceQuit, KEY_Q);

	//Sound::registerSample("data/sound/gui_select.wav", 255);

	float masterVolume = 1.0f;
	Music::registerSong("data/music/splash.xm", 1.0f * masterVolume);
	Music::registerSong("data/music/splash2.xm", 2.0f * masterVolume);
	
	ScreenManager::add(new SplashScreen2());	
}

void Speedhack11Game::onLogic()
{
	if (Input::isPressed(Button_ToggleFullscreen))
	{
		toggleFullscreen();
	}

	if (Input::isPressed(Button_ToggleSlowMotion))
	{
		toggleSlowMotion();
	}

	if (Input::isPressed(Button_ToggleVSync))
	{
		toggleVSync();
	}

	if (Input::isPressed(Button_ToggleWideScreen))
	{
		toggleWideScreen();
	}

	if (Input::isPressed(Button_ForceQuit) || ScreenManager::isEmpty())
	{
		halt();
	}
}
