#include <allegro.h>
#include <aldumb.h>
#include <dumb.h>

#include "music.hpp"

static DUH *playing = NULL;
static AL_DUH_PLAYER *player;
static std::string playingFile;

void initMusic()
{
	atexit(&dumb_exit);
	dumb_register_stdfiles();
}

void playMusic(std::string file)
{
	if (playingFile == file) {
		return;
	}

	if (playing != NULL) {
		al_stop_duh(player);
		unload_duh(playing);
	}

	playing = dumb_load_xm_quick(file.c_str());
	player = al_start_duh(playing, 2, 0, 0.5f, 4096, 44100);
	playingFile = file;

	pollMusic();
}

void pollMusic() {
	al_poll_duh(player);
}
