#include "util.hpp"
#include "Property.hpp"

#include <loader/ImageLoader.hpp>
#include <renderer/ShaderManager.hpp>
#include <scene/PolyMesh.hpp>

#include <fstream>


using kocmoc::scene::PolyMesh;
using kocmoc::renderer::Shader;
using kocmoc::renderer::ShaderManager;
using kocmoc::loader::ImageLoader;

using glm::vec4;
using glm::mat4;
using glm::vec3;

namespace kocmoc
{
	namespace util
	{

		bool file_exists(const std::string &filename)
		{
			std::ifstream ifile(filename.c_str());
			return ifile;
		}

		std::string read_file(const std::string &filename)
		{
			std::ifstream ifile(filename.c_str());

			return std::string(std::istreambuf_iterator<char>(ifile),
				std::istreambuf_iterator<char>());
		}

		void safeDeleteArray(void* p)
		{
			if (p != NULL)
			{
				delete[] p;
				p = NULL;
			}
		}


		// code taken from http://www.oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html
		void tokenize(const std::string& str, std::vector<std::string >& tokens, const std::string& delimiter)
		{
			// Skip delimiters at beginning.
			std::string::size_type lastPos = str.find_first_not_of(delimiter, 0);
			// Find first "non-delimiter".
			std::string::size_type pos = str.find_first_of(delimiter, lastPos);

			while (std::string::npos != pos || std::string::npos != lastPos)
			{
				// Found a token, add it to the vector.
				tokens.push_back(str.substr(lastPos, pos - lastPos));
				// Skip delimiters.  Note the "not_of"
				lastPos = str.find_first_not_of(delimiter, pos);
				// Find next "non-delimiter"
				pos = str.find_first_of(delimiter, lastPos);
			}
		}

		namespace generator
		{

			PolyMesh* generateStars()
			{
				// generate starts, lots of stars
				// only in a sphere
				int starCount = Property("starsCount");
				float domain = Property("starsDomain");
				float size = Property("starsSize");

				
				unsigned int primitiveCount = starCount * 4; // 3 tris per tetraeder
				unsigned int vertexIndexCount = primitiveCount * 3; // 3 vertices per triangle
				unsigned int vertexCount = starCount * 4; // 4 vertices per tetraeder
				PolyMesh *stars = new PolyMesh(primitiveCount, vertexIndexCount, vertexCount);

				double *positions = new double[stars->getVertexCount() * 3];
				unsigned int *vertexIndices = new unsigned int[stars->getVertexIndexCount()];
				unsigned int *firstIndices = new unsigned int[stars->getPrimitiveCount() + 1];
					
				mat4 scale = glm::gtx::transform::scale(size, size, size);

				for (int i = 0; i < starCount; i++)
				{
					vec4 v0 = vec4(0, 0, 0, 1);
					vec4 v1 = vec4(0, 1, 1, 1);
					vec4 v2 = vec4(1, 1, 0, 1);
					vec4 v3 = vec4(1, 0, 1, 1);

					mat4 rotation = glm::gtx::transform::rotate(((float)rand() * 360)/RAND_MAX,
						(float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX);
					
					vec3 translation = vec3((rand() * domain)/RAND_MAX - (domain / 2),
						(rand() * domain)/RAND_MAX - (domain / 2),
						(rand() * domain)/RAND_MAX - (domain / 2));

					mat4 transform = glm::translate(translation) * scale * rotation;

					v0 = transform * v0;
					v1 = transform * v1;
					v2 = transform * v2;
					v3 = transform * v3;

					// ad vertices
					positions[i*12] = v0.x;
					positions[i*12+1] = v0.y;
					positions[i*12+2] = v0.z;
					positions[i*12+3] = v1.x;
					positions[i*12+4] = v1.y;
					positions[i*12+5] = v1.z;
					positions[i*12+6] = v2.x;
					positions[i*12+7] = v2.y;
					positions[i*12+8] = v2.z;
					positions[i*12+9] = v3.x;
					positions[i*12+10] = v3.y;
					positions[i*12+11] = v3.z;
					

					// add primitives and indices, 
					// 0 - 1 - 2
					firstIndices[i * 4] = i*12;
					vertexIndices[i*12] = i*4;
					vertexIndices[i*12+1] = i*4 +1;
					vertexIndices[i*12+2] = i*4 +2;
					
					//3 - 2 - 1
					firstIndices[i*4+1] = i*12+3;
					vertexIndices[i*12+3] = i*4 +3;
					vertexIndices[i*12+4] = i*4 +2;
					vertexIndices[i*12+5] = i*4 +1;

					//1 - 0 - 3
					firstIndices[i*4+2] = i*12+6;
					vertexIndices[i*12+6] = i*4 +1;
					vertexIndices[i*12+7] = i*4 +0;
					vertexIndices[i*12+8] = i*4 +3;

					// 0 - 2 - 3
					firstIndices[i*4+3] = i*12+9;
					vertexIndices[i*12+9] = i*4 +0;
					vertexIndices[i*12+10] = i*4 +2;
					vertexIndices[i*12+11] = i*4 +3;
				}

				// add last first index
				firstIndices[primitiveCount] = primitiveCount * 3;

				stars->setFirstIndexArray(firstIndices);
				stars->setVertexIndexArray(vertexIndices);
				stars->setVertexPositions(positions);

				// add shader to poly
				Shader *shader = ShaderManager::getInstance().load("base.vert", "stars.frag");
				//Shader *shader = new Shader("base.vert", "base.frag");
				stars->setShader(shader);

				// add texture
				GLuint tex = ImageLoader::getInstance().loadImage("color.png");
				stars->setDiffuseTexture(tex);

				return stars;
			}

			PolyMesh* generateGizmo()
			{
				double *vertexPositions = new double[12];
				vertexPositions[0] = 0.0f;
				vertexPositions[1] = 0.0f;
				vertexPositions[2] = 0.0f;

				vertexPositions[3] = 1.0f;
				vertexPositions[4] = 0.0f;
				vertexPositions[5] = 0.0f;

				vertexPositions[6] = 0.0f;
				vertexPositions[7] = 1.0f;
				vertexPositions[8] = 0.0f;

				vertexPositions[9] = 0.0f;
				vertexPositions[10] = 0.0f;
				vertexPositions[11] = 1.0f;

				unsigned int *indices =  new unsigned int[6];
				indices[0] = 0;
				indices[1] = 1;
				indices[2] = 0;
				indices[3] = 2;
				indices[4] = 0;
				indices[5] = 3;
				
				unsigned int *fia = new unsigned int[4];
				fia[0] = 0;
				fia[1] = 2;
				fia[2] = 4;
				fia[3] = 6;

				unsigned int primitiveCount = 3;
				unsigned int vertexIndexCount = 6;
				unsigned int vertexCount = 4;
				PolyMesh *mesh = new PolyMesh(primitiveCount, vertexIndexCount, vertexCount);

				mesh->setFirstIndexArray(fia);
				mesh->setVertexIndexArray(indices);
				mesh->setVertexPositions(vertexPositions);

				// add shader to poly
				Shader *shader = new Shader("vertexColor.vert", "vertexColor.frag");
				mesh->setShader(shader);

				// add texture
				GLuint tex = ImageLoader::getInstance().loadImage("color.png");
				mesh->setDiffuseTexture(tex);

				return mesh;
			}
		}
	}
}