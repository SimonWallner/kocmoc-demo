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
		GLuint loadImage(string filename);


		/**
		 * Take a screenshot and save it to disk
		 */
		void screenShot(void);

		/**
		 * Change the texture quality: nearest/bilinear
		 */
		//void changeTextureQuality(void);

		/**
		 * Change the mip map quality: off/nearest/linear
		 */
		//void changeMipMapQuality(void);

		/**
		 * Apply the changes to the texture quality from the right thread!
		 */
		//void contextThreadApplyChanges(void);

		/**
		 * en/disable anisotropic filtering
		 */
		//void toggleAnisotropicFiltering(void);

	private:
		ImageLoader(void);
		ImageLoader(const ImageLoader &cc);

		string texturePathPrefix;

		std::map<string, GLuint> cache;

		GLint currentTextureQuality;
		bool texOptionsChanged;
		GLfloat maxAnisotropy;
		bool useAF;
	
	};
}

#endif
