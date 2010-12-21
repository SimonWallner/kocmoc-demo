#ifndef SHADER_MANAGER_HPP_
#define SHADER_MANAGER_HPP_

#include <map>
#include "Shader.hpp"


namespace kocmoc
{

	typedef std::map<std::string, Shader* > ShaderCache;

	/**
	 * A simple manager that manages loading and reloading of shaders at runtime.
	 * Should also support caching, and it's a singleton.
	 */
	class ShaderManager
	{
	public:

		/**
		 * Destroy the singleton instance
		 */
		static void Destroy(void);

		static ShaderManager& getInstance(void);

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