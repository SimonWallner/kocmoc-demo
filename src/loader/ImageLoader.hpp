#ifndef KOCMOC_LOADER_IMAGE_LOADER_H_
#define KOCMOC_LOADER_IMAGE_LOADER_H_

#include <common.hpp>

#include <IL/il.h>

#include <map>

namespace kocmoc
{
	namespace loader
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
		
		typedef std::map<std::string, CacheInfo> ImageCache;

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
			 * and returns a texture handle. The images are remove from the system
			 * memory after they have been pushed to the GPU.
			 * 
			 * Only files are supported that are supported by the underlying devIl lib.
			 *
			 * @param filename the filename of the texture including the extension
			 *		like 'texture.png'
			 * @return The texture handle. If an error occurs, \c0 is
			 *		returned instead
			 */
			GLuint loadImage(std::string filename, bool degamma = false);
			GLuint loadImage3D(std::string filename, bool degamma = false);


			/**
			 * Take a screenshot and save it to disk
			 * @param sequence if it is true, the screenshots are numbered in 
			 *		a sequence.
			 */
			void screenShot(unsigned int isSequence = false);

			/**
			 * reload all cached textures
			 */
			void reload(void);

		private:
			ImageLoader(void);
			ImageLoader(const ImageLoader &cc);

			bool loadImageToHandle(std::string filname, bool degamma, GLuint handle);
			bool loadImageToHandle3D(std::string filname, bool degamma, GLuint handle);

			std::string texturePathPrefix;

			ImageCache cache;
			ImageCache cache3D;

			GLfloat maxAnisotropy;
			bool useAF;

			unsigned int frameSequenceNumber;
		};
	}
}

#endif