#ifndef SHADER_MANAGER_HPP_
#define SHADER_MANAGER_HPP_

#include <map>
#include "Shader.hpp"


namespace kocmoc
{

	typedef std::map<std::string, Shader* > ShaderCache;

	/**
	 * A simple manager that manages loading and reloading of shaders at runtime.
	 * Also supports caching of shaders.
	 */
	class ShaderManager
	{
	public:

		/**
		 * Destroy the singleton instance
		 */
		static void Destroy(void);

		/**
		 * Get the singleton instance
		 */
		static ShaderManager& getInstance(void);

		/**
		 * Load a shader. 
		 * This is basically the same as directly constructing a Shader, but
		 * with the added support for caching and mass reloading of all
		 * existing shaders.
		 *
		 * @return A pointer to a shader. This does not have to be a new Shader
		 * it can as well be an existing shader from the cache.
		 */
		Shader* load(const std::string &vertexShaderFile, const std::string &fragmentShaderFile);

		/**
		 * reload all shaders, i.e. read the files, compile and link.
		 */
		void reloadAll(void);

	private:
		ShaderManager(void);
		~ShaderManager(void);
		ShaderManager(const ShaderManager &cc);

		static ShaderManager *instance;

		ShaderCache cache;
	};
}
#endif