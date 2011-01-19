#include "AudioPlayer.hpp"
#include "Property.hpp"
#include "common.hpp"

#include "al/alut.h"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

using namespace kocmoc;

// force the audio playing/streaming thread to take a short break
// to allow the input thread to steal a few cycles from the processor.
// thread priorities seem to be inexistant in glfw, *sigh
#define THREAD_SLEEP_TIME 50.0/1000


AudioPlayer *AudioPlayer::instance = NULL;
string AudioPlayer::pathPrefix = "";

AudioPlayer::AudioPlayer(void)
	: isMuted(util::Property("muteAudio"))
	, counter(0)
{
	pathPrefix = util::Property("soundsRootFolder");
	alutInit(NULL, NULL);
	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
}

AudioPlayer::~AudioPlayer(void)
{
	alutExit();
}


AudioPlayer& AudioPlayer::getInstance()
{
	if(!instance)
		instance = new AudioPlayer();

	return *instance;
}

void AudioPlayer::Destroy()
{
	delete instance;
	instance = NULL;
}

void AudioPlayer::play(string filename, float gain)
{
	if (!isMuted)
	{
		// use this field to give the thread enought time to cast the params
		currentParams.filename = filename;
		currentParams.gain = gain;
		
		GLFWthread thread = glfwCreateThread(playThread, &currentParams);
		//glfwWaitThread(thread, GLFW_WAIT);
	}
}

void AudioPlayer::playThread(void *args)
{
	params par = *(params*)args;

	cout << "now playing: " << par.filename << endl;
	
	OggStream ogg;
	
	try
	{
		ogg.open(pathPrefix + par.filename, par.gain);
		ogg.display();

		if (!ogg.playback())
			cout << "Ogg refused to play: " << par.filename << endl;
		while (ogg.update())
		{
			if(!ogg.playing())
			{
				if(!ogg.playback())
					throw string("Ogg abruptly stopped.");
				else
					cout << "Ogg stream was interrupted.\n";
			}
			glfwSleep(THREAD_SLEEP_TIME);
		}
	}
	catch(string error)
	{
		cout << error;
	}

	ogg.release();
}


void AudioPlayer::mute(bool _isMuted)
{
	isMuted = _isMuted;
}