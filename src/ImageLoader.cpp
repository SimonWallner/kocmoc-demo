#include "ImageLoader.hpp"
#include "PropertiesFileParser.hpp"

#include "il/ilu.h"
#include "il/ilut.h"

#include <time.h>
#include <sstream>

ImageLoader::ImageLoader(void)
{
	PropertiesFileParser::GetInstance().getProperty("TexturesRootFolder", &texturePathPrefix);

	ilInit();
	iluInit();
	useAF = false;

	currentTextureQuality = GL_LINEAR_MIPMAP_LINEAR;
	texOptionsChanged = false;

	//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	//cout << "Max anisotropy supported: " << maxAnisotropy << endl;
}

ImageLoader::~ImageLoader(void)
{
}

ImageLoader &ImageLoader::getInstance()
{
	static ImageLoader instance;
	return instance;
}


GLuint ImageLoader::loadImage(string filename)
{
	string fullPath = texturePathPrefix + filename;

	if (_DEBUG)
		cout << "loading image: " << fullPath << "...";
	
	std::map<string, GLuint>::iterator it;
	it = cache.find(filename);
	if (it != cache.end()) // filename found in cache
	{
		if (_DEBUG)
			cout << " found in cache!" << endl;
		return it->second;
	}

	// this code is based on the code found at http://gpwiki.org/index.php/DevIL:Tutorials:Basics
	ILuint texid;
	GLuint textureHandle;

	ilGenImages(1, &texid);
	ilBindImage(texid);

	ilLoadImage(fullPath.c_str());
	ILenum error = ilGetError();

	if (error == IL_NO_ERROR) /* If no error occured: */
	{
get_errors();
		/* Convert every colour component into unsigned byte. If your textureHandle contains
		alpha channel you can replace IL_RGB with IL_RGBA */
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
get_errors();
		glGenTextures(1, &textureHandle); /* Texture name generation */
get_errors();
		glBindTexture(GL_TEXTURE_2D, textureHandle); /* Binding of texture name */
get_errors();		
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData()); /* Texture specification */
get_errors();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, currentTextureQuality);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//if (useAF)
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLint)maxAnisotropy);

		glGenerateMipmap(GL_TEXTURE_2D);

		
		// unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);
		// remove loaded image from memory
		ilDeleteImages(1, &texid);

		if(_DEBUG)
		cout << " done!" << endl;
				
		// add to cache
		cache[filename] = textureHandle;
		return textureHandle;
	} else
	{
		cout << "failed ot load image: " << filename << " (" << iluErrorString(error) << ")" << endl;
		return 0;
	}
}

void ImageLoader::screenShot()
{
	// init
	ilutRenderer(ILUT_OPENGL);

	ILuint image;

	ilGenImages(1, &image);
	ilBindImage(image);

	ilutGLScreen();

	ILenum error = ilGetError();

	char date[9];
	char time[9];
	_strdate(date);
	_strtime(time);

	date[2] = date[5] = '-';
	time[2] = time[5] = '-';

	string fileName = "screenshot_";

	fileName.append(date);
	fileName.append("_");
	fileName.append(time);
	fileName.append(".png");


	if(ilSave(IL_PNG, fileName.c_str()))
		cout << "screenshot taken! (" << fileName << ")";
	else 
		cout << "failed to take screenshot";

	ilDeleteImage(image);
}
