#include "ShaderManager.hpp"

using namespace kocmoc;

using std::string;

ShaderManager* ShaderManager::instance = NULL;

ShaderManager& ShaderManager::getInstance()
{
	if(!instance)
		instance = new ShaderManager();
	return *instance;
}

void ShaderManager::Destroy()
{
	delete instance;
	instance = NULL;
}

ShaderManager::ShaderManager(void)
{
}

ShaderManager::~ShaderManager(void)
{
}


Shader* ShaderManager::load(const string &vertexShaderFile, const string &fragmentShaderFile)
{
	string key = vertexShaderFile + fragmentShaderFile;

	ShaderCache::const_iterator ci = cache.find(key);
	if (ci != cache.end()) // cache hit!
	{
		//if (_DEBUG)
		//	cout << " found in cache!" << endl;
		return ci->second;
	}

	Shader *shader = new Shader(vertexShaderFile, fragmentShaderFile);

	cache[key] = shader;
	return shader;
}

void ShaderManager::reloadAll()
{
	std::cout << "Reloading all shaders..." << std::endl;
	for (ShaderCache::iterator it = cache.begin(); it != cache.end(); it++)
	{
		(*it).second->reload();
	}
}