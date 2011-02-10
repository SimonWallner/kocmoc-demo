#include "OggStream.hpp"

#include <iostream>

using std::string;
using std::cout;
using std::endl;

using namespace kocmoc::audio;

OggStream::OggStream(void)
{
}

OggStream::~OggStream(void)
{
}

void OggStream::open(string path, float gain)
{
	int result;
    
    if(!(oggFile = fopen(path.c_str(), "rb")))
        throw string("Could not open Ogg file.");
 
    if((result = ov_open(oggFile, &oggStream, NULL, 0)) < 0)
    {
        fclose(oggFile);
 
        throw string("Could not open Ogg stream. ") + errorString(result);
    }
 
	vorbisInfo = ov_info(&oggStream, -1);
    vorbisComment = ov_comment(&oggStream, -1);
 
    if(vorbisInfo->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;

	alGenBuffers(2, buffers);
    check();
    alGenSources(1, &source);
    check();
    
    alSource3f(source, AL_POSITION,		   0.0, 0.0, 0.0);
    alSource3f(source, AL_VELOCITY,        0.0, 0.0, 0.0);
    alSource3f(source, AL_DIRECTION,       0.0, 0.0, 0.0);
    alSourcef (source, AL_ROLLOFF_FACTOR,  0.0          );
    alSourcei (source, AL_SOURCE_RELATIVE, AL_TRUE      );
	alSourcef (source, AL_GAIN, gain);
}

void OggStream::release()
{
	alSourceStop(source);
    empty();

    alDeleteSources(1, &source);
    check();
    
	alDeleteBuffers(1, buffers);
    check();
 
    ov_clear(&oggStream);
}

void OggStream::display()
{
    cout
        << "version         " << vorbisInfo->version         << endl
        << "channels        " << vorbisInfo->channels        << endl
        << "rate (hz)       " << vorbisInfo->rate            << endl
        << "bitrate upper   " << vorbisInfo->bitrate_upper   << endl
        << "bitrate nominal " << vorbisInfo->bitrate_nominal << endl
        << "bitrate lower   " << vorbisInfo->bitrate_lower   << endl
        << "bitrate window  " << vorbisInfo->bitrate_window  << endl
        << "\n"
        << "vendor " << vorbisComment->vendor << "\n";
        
    for(int i = 0; i < vorbisComment->comments; i++)
        cout << "   " << vorbisComment->user_comments[i] << "\n";
        
    cout << endl;
}

bool OggStream::playback()
{
    if(playing())
        return true;
        
    if(!stream(buffers[0]))
        return false;
 
    if(!stream(buffers[1]))
        return false;
    
    alSourceQueueBuffers(source, 2, buffers);
    alSourcePlay(source);
    
    return true;
}

bool OggStream::playing()
{
    ALenum state;
    
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    
    return (state == AL_PLAYING);
}

bool OggStream::update()
{
    int processed;
    bool active = true;
 
    alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
 
    while(processed--)
    {
        ALuint buffer;
        
        alSourceUnqueueBuffers(source, 1, &buffer);
        check();
 
        active = stream(buffer);
 
        alSourceQueueBuffers(source, 1, &buffer);
        check();
    }
 
    return active;
}

bool OggStream::stream(ALuint buffer)
{    
	char data[BUFFER_SIZE];
    int  size = 0;
    int  section;
    int  result;
 
    while(size < BUFFER_SIZE)
    {
        result = ov_read(&oggStream, data + size, BUFFER_SIZE - size, 0, 2, 1, & section);
    
        if(result > 0)
            size += result;
        else
            if(result < 0)
                throw errorString(result);
            else
                break;
    }
    
    if(size == 0)
        return false;
 
    alBufferData(buffer, format, data, size, vorbisInfo->rate);
    check();
 
    return true;
}

void OggStream::empty()
{
    int queued;
    
    alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
    
    while(queued--)
    {
        ALuint buffer;
    
        alSourceUnqueueBuffers(source, 1, &buffer);
        check();
    }
}

void OggStream::check()
{
    int error = alGetError();
 
    if(error != AL_NO_ERROR)
        throw string("OpenAL error was raised.");
}

string OggStream::errorString(int code)
{
    switch(code)
    {
        case OV_EREAD:
            return string("Read from media.");
        case OV_ENOTVORBIS:
            return string("Not Vorbis data.");
        case OV_EVERSION:
            return string("Vorbis version mismatch.");
        case OV_EBADHEADER:
            return string("Invalid Vorbis header.");
        case OV_EFAULT:
            return string("Internal logic fault (bug or heap/stack corruption.");
        default:
            return string("Unknown Ogg error.");
    }
}