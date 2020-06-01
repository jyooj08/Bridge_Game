#include "sound.h"

// sound objects
irrklang::ISoundEngine* engine;
irrklang::ISoundSource* mp3_src = nullptr;
const char* basic_mp3_path = "../bin/sounds/music.mp3";

bool init_sound() {
	engine = irrklang::createIrrKlangDevice();
	if (!engine) return false;
	mp3_src = engine->addSoundSourceFromFile(basic_mp3_path);
	mp3_src->setDefaultVolume(0.5f);
	return true;
}

void add_sound(char* mp3_path) {
	mp3_src = engine->addSoundSourceFromFile(mp3_path);
}

void play_sound() {
	engine->play2D(mp3_src, true);
}

void stop_sound() {
	engine->stopAllSounds();
}

void finalize_sound() {
	engine->drop();
}