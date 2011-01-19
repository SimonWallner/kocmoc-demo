#ifndef _AUDIO_PLAYER_H_
#define _AUDIO_PLAYER_H_

#include "OggStream.hpp"
#include <map>
#include <string>

namespace kocmoc
{

	/**
	 * A struct to pass arguments to the glfw thread
	 */
	struct params {
		std::string filename;
		double gain;
	};


	/**
	 * This is a simple singleton audio player, meant to play off files, for 
	 * either looped playback (background music) and game sounds.
	 * 
	 * Filenames are always meant to be without a path prefix. If you
	 * want to play the file foo.ogg in the directory bar/ you only use
	 * "foo.ogg" as the filename not "bar/foo.ogg" (unless it is in a subdirectory
	 * bar/)
	 * 
	 * The code based on http://www.devmaster.net/articles/openal-tutorials/lesson8.php
	 */
	class AudioPlayer
	{

	public:
		/**
		 * Get the singleton instance
		 */
		static AudioPlayer &getInstance(void);

		/**
		 * Destroy the singleton instance
		 */
		static void Destroy(void);

		/**
		 * Play a sound
		 * @param filename
		 * @param gain the gain of the sound.
		 */
		void play(std::string filename, float gain = 1.0f);

		/**
		 * Mute the player
		 * @param isMuted iff true the player is muted, if not, not
		 */
		void mute(bool _isMuted);

		static void playThread(void *args);

	private:

		static AudioPlayer* instance;

		bool isMuted;
		static std::string pathPrefix;

		// protect the c'tor, copy c'tor and the d'tor
		AudioPlayer(void);
		AudioPlayer(const AudioPlayer &cc);
		~AudioPlayer(void);

		std::map<unsigned int, OggStream*> bufferMap;
		unsigned int counter;

		params currentParams;
	};
}

#endif