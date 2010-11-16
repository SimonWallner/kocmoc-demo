#include "ImageLoader.hpp"

#include "il/ilu.h"
#include "il/ilut.h"

#include <time.h>
#include <sstream>
#include "FontTool.h"

ImageLoader::ImageLoader(void)
{
	ilInit();
	iluInit();
	texunit = 0;
	useAF = true;

	currentTextureQuality = GL_LINEAR_MIPMAP_LINEAR;
	texOptionsChanged = false;

	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	cout << "Max anisotropy supported: " << maxAnisotropy << endl;

}

ImageLoader::~ImageLoader(void)
{
}

ImageLoader &ImageLoader::getInstance()
{
	static ImageLoader instance;
	return instance;
}

void ImageLoader::contextThreadApplyChanges()
{
	if (texOptionsChanged)
	{
		std::map<string, GLuint>::iterator it; 
		for (it = cache.begin(); it != cache.end(); it++)
		{
			glBindTexture(GL_TEXTURE_2D, it->second);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, currentTextureQuality);

			if (useAF)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLint)maxAnisotropy);
			else 
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		texOptionsChanged = false;
	}
}

void ImageLoader::toggleAnisotropicFiltering()
{
	useAF = !useAF;
	texOptionsChanged = true;

	std::stringstream stream;
	
	if (useAF)
		stream << "Anisotropic Filtering: ON" << endl;
	else 
		stream << "Anisotropic Filtering: OFF" << endl;

	FontTool::getInstance().print(stream.str());
}

void ImageLoader::changeTextureQuality()
{
	texOptionsChanged = true;

	if (currentTextureQuality == GL_NEAREST) 
	{
		currentTextureQuality = GL_LINEAR;
		FontTool::getInstance().print("GL_LINEAR");
	}
	else if (currentTextureQuality == GL_LINEAR)
	{
		currentTextureQuality = GL_NEAREST;
		FontTool::getInstance().print("GL_NEAREST");
	}

	else if (currentTextureQuality == GL_NEAREST_MIPMAP_NEAREST)
	{
		currentTextureQuality = GL_LINEAR_MIPMAP_NEAREST;
		FontTool::getInstance().print("GL_LINEAR_MIPMAP_NEAREST");
	}

	else if (currentTextureQuality == GL_NEAREST_MIPMAP_LINEAR)
	{
		currentTextureQuality = GL_LINEAR_MIPMAP_LINEAR;
		FontTool::getInstance().print("GL_LINEAR_MIPMAP_LINEAR");
	}

	else if (currentTextureQuality == GL_LINEAR_MIPMAP_NEAREST)
	{
		currentTextureQuality = GL_NEAREST_MIPMAP_NEAREST;
		FontTool::getInstance().print("GL_NEAREST_MIPMAP_NEAREST");
	}

	else if (currentTextureQuality == GL_LINEAR_MIPMAP_LINEAR)
	{
		currentTextureQuality = GL_NEAREST_MIPMAP_LINEAR;
		FontTool::getInstance().print("GL_NEAREST_MIPMAP_LINEAR");
	}
}

void ImageLoader::changeMipMapQuality()
{
	texOptionsChanged = true;

	if (currentTextureQuality == GL_NEAREST)
	{
		currentTextureQuality = GL_NEAREST_MIPMAP_NEAREST;
		FontTool::getInstance().print("GL_NEAREST_MIPMAP_NEAREST");
	}

	else if (currentTextureQuality == GL_NEAREST_MIPMAP_NEAREST)
	{
		currentTextureQuality = GL_NEAREST_MIPMAP_LINEAR;
		FontTool::getInstance().print("GL_NEAREST_MIPMAP_LINEAR");
	}

	else if (currentTextureQuality == GL_NEAREST_MIPMAP_LINEAR)
	{
		currentTextureQuality = GL_NEAREST;
		FontTool::getInstance().print("GL_NEAREST");
	}


	else if (currentTextureQuality == GL_LINEAR)
	{
		currentTextureQuality = GL_LINEAR_MIPMAP_NEAREST;
		FontTool::getInstance().print("GL_LINEAR_MIPMAP_NEAREST");
	}

	else if (currentTextureQuality == GL_LINEAR_MIPMAP_NEAREST)
	{
		currentTextureQuality = GL_LINEAR_MIPMAP_LINEAR;
		FontTool::getInstance().print("GL_LINEAR_MIPMAP_LINEAR");
	}

	else if (currentTextureQuality == GL_LINEAR_MIPMAP_LINEAR)
	{
		currentTextureQuality = GL_LINEAR;
		FontTool::getInstance().print("GL_LINEAR");
	}
}

GLuint ImageLoader::loadImage(string filename)
{
	cout << "loading image: " << filename << "...";
	
	std::map<string, GLuint>::iterator it;
	it = cache.find(filename);
	if (it != cache.end()) // filename found in cache
	{
		cout << " found in cache!" << endl;
		return it->second;
	}

	// this code is based on the code found at http://gpwiki.org/index.php/DevIL:Tutorials:Basics
	ILuint texid;
	GLuint textureHandle;

	ilGenImages(1, &texid);
	ilBindImage(texid);

	ilLoadImage(filename.c_str());
	ILenum error = ilGetError();

	if (error == IL_NO_ERROR) /* If no error occured: */
	{
		/* Convert every colour component into unsigned byte. If your textureHandle contains
		alpha channel you can replace IL_RGB with IL_RGBA */
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		glGenTextures(1, &textureHandle); /* Texture name generation */
		glBindTexture(GL_TEXTURE_2D, textureHandle); /* Binding of texture name */
		
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData()); /* Texture specification */

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, currentTextureQuality);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (useAF)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLint)maxAnisotropy);

		glGenerateMipmap(GL_TEXTURE_2D);

		
		// unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);
		// remove loaded image from memory
		ilDeleteImages(1, &texid);

		cout << " done!" << endl;
		texunit++;
		
		// add to cache
		cache[filename] = textureHandle;
		return textureHandle;
	} else
	{
		cout << "failed ot load image: " << filename << " (" << iluErrorString(error) << ")" << endl;
		return 0;
	}
}

ImageData* ImageLoader::getImageData(string filename){
	cout << "loading image: " << filename << "...";
	ILuint texid;

	ilGenImages(1, &texid);
	ilBindImage(texid);

	ilLoadImage(filename.c_str());
	ILenum error = ilGetError();

	if (error == IL_NO_ERROR) /* If no error occured: */
	{
		ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		ImageData* data = new ImageData();
		GLubyte* tempdata =ilGetData();
		GLint size=ilGetInteger(IL_IMAGE_WIDTH)*ilGetInteger(IL_IMAGE_HEIGHT)*3;
		data->data=new GLubyte[size];
		//sollte i signed int sein
		for(GLint i =0;i<size;i++){
			data->data[i]=tempdata[i];
		}
		data->width=ilGetInteger(IL_IMAGE_WIDTH);
		data->height=ilGetInteger(IL_IMAGE_HEIGHT);

		ilDeleteImages(1, &texid);
		cout << " done!" << endl;
		return data;
	}
	else{
		cout << "failed ot load image: " << filename << " (" << iluErrorString(error) << ")" << endl;
		return NULL;
	}
	//wert zurückgeben
}

GLuint ImageLoader::getTexUnit(){
	return texunit;
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

	std::stringstream stream;
	if(ilSave(IL_PNG, fileName.c_str()))
	{
		stream << "screenshot taken! (" << fileName << ")";
	}
	else 
		stream << "failed to take screenshot";
	FontTool::getInstance().print(stream.str());

	ilDeleteImage(image);
}
