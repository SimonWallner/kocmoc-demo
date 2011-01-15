#ifndef _IMAGE_LOADER_H_
#define _IMAGE_LOADER_H_

#include "IL/il.h"
#include "common.hpp"

#include <map>

namespace kocmoc
{

	struct ImageData{
		GLuint width;
		GLuint height;
		GLubyte* data;
	};

	struct CacheInfo {
		GLuint handle;
		bool degamma;
	};
	
	typedef std::map<string, CacheInfo> ImageCache;
	/**
	 * A basic devIL image Loader. Now as Singleton!
	 */
	class ImageLoader
	{
	public:

		/**
		 * Get the singleton instance.
		 */
		static ImageLoader &getInstance();

		~ImageLoader(void);

		/**
		 * Load the image from the given filename.
		 * This method loads the given texture into the graphic card's memory
		 * and returns a texture handle. The images are removem from the system
		 * memory after they have been pushed to the gpu.
		 * 
		 * Only files are supported that are supported by the underlying devIl lib.
		 *
		 * @param filename the filename of the texture including the extension
		 *		like 'texture.png'
		 * @return The texture handle. If an error occurs, <code>0</code> is
		 *		returned instead
		 */
		GLuint loadImage(string filename, bool degamma = false);
		GLuint loadImage3D(string filename, bool degamma = false);


		/**
		 * Take a screenshot and save it to disk
		 */
		void screenShot(void);

		/**
		 * reload all cached textures
		 */
		void reload(void);

	private:
		ImageLoader(void);
		ImageLoader(const ImageLoader &cc);

		bool loadImageToHandle(std::string filname, bool degamma, GLuint handle);
		bool loadImageToHandle3D(std::string filname, bool degamma, GLuint handle);

		string texturePathPrefix;

		ImageCache cache;
		ImageCache cache3D;

		GLint currentTextureQuality;
		bool texOptionsChanged;
		GLfloat maxAnisotropy;
		bool useAF;
	
	};
}

#endif
