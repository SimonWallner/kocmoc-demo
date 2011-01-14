#include "ImageLoader.hpp"
#include "PropertiesFileParser.hpp"

#define ILUT_USE_OPENGL
#include "IL/ilu.h"
#include "IL/ilut.h"

#include <time.h>
#include <sstream>

using namespace kocmoc;

ImageLoader::ImageLoader(void)
{
	util::PropertiesFileParser::GetInstance().getProperty("TexturesRootFolder", &texturePathPrefix);

	ilInit();
	iluInit();

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

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


GLuint ImageLoader::loadImage(string filename, bool degamma)
{
	string fullPath = texturePathPrefix + filename;

	//if (_DEBUG)
	//	cout << "loading image: " << fullPath << "...";
	
	std::map<string, GLuint>::iterator it;
	it = cache.find(filename);
	if (it != cache.end()) // filename found in cache
	{
		//if (_DEBUG)
		//	cout << " found in cache!" << endl;
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
		/* Convert every colour component into unsigned byte. If your textureHandle contains
		alpha channel you can replace IL_RGB with IL_RGBA */
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		glGenTextures(1, &textureHandle); /* Texture name generation */
		glBindTexture(GL_TEXTURE_2D, textureHandle); /* Binding of texture name */


		GLint internalFormat;
		if (degamma)
			internalFormat = GL_SRGB8_ALPHA8;
		else
			internalFormat = GL_RGBA;// ilGetInteger(IL_IMAGE_FORMAT);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData()); /* Texture specification */

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


	time_t t = time(0);
	struct tm * now = localtime(&t);

	std::stringstream fileName;
	fileName << "screenshot_" << (now->tm_year + 1900) << "_" << (now->tm_mon + 1) << "_" << now->tm_mday <<
			"-" << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << ".png";

	std::string foo = fileName.str();
	std::string bar = fileName.str().c_str();

	char buffer[50];
	int length = sprintf(buffer, "screenshot_%d_%d_%d-%d-%d-%d.png",
		now->tm_year + 1900,
		now->tm_mon + 1,
		now->tm_mday,
		now->tm_hour,
		now->tm_min,
		now->tm_sec);


	if(ilSave(IL_PNG, buffer))
		cout << "screenshot taken! (" << fileName << ")";
	else 
	{
		std::cout << "failed to take screenshot: ";
		if (ilGetError() == IL_COULD_NOT_OPEN_FILE)
			std::cout << "could not open file '" << fileName << "' for writing";

		std::cout << std::endl;
	}

	ilDeleteImage(image);
}
