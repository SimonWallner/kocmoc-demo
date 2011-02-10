#ifndef KOCMOC_AUDIO_OGG_STREAM_H_
#define KOCMOC_AUDIO_OGG_STREAM_H_

#include <al/al.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

#include <string>

#define BUFFER_SIZE (4096 * 8)

namespace kocmoc
{
	namespace audio
	{

		/**
		 * This clas provides ogg streaming
		 * code taken from http://www.devmaster.net/articles/openal-tutorials/lesson8.php
		 */
		class OggStream
		{
		 public:

			 OggStream();
			 ~OggStream();
		 
			/**
			 * obtain a handle to the file
			 */
			void open(std::string path, float gain);

			/**
			 * release the file handle
			 */
			void release();

			/**
			 * display some info on the Ogg
			 */
			void display();

			/**
			 * play the Ogg stream
			 */
			bool playback();

			/**
			 * check if the source is playing
			 */
			bool playing();

			/**
			 * update the stream if necessary
			 */
			bool update();

		protected:

			bool stream(ALuint buffer);		// reloads a buffer
			void empty();					// empties the queue
			void check();					// checks OpenAL error state
			std::string errorString(int code);	// stringify an error code

		private:
			FILE* oggFile;	   // file handle
			OggVorbis_File oggStream;	 // stream handle
			vorbis_info* vorbisInfo;	// some formatting data
			vorbis_comment* vorbisComment; // user comments

			ALuint buffers[2];	// front and back buffers
			ALuint source;		// audio source
			ALenum format;		// internal format

		};
	}
}
#endif