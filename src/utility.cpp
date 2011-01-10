#include "utility.hpp"

#include <fstream>
#include "Shader.hpp"
#include "ImageLoader.hpp"
#include "Property.hpp"
#include "ShaderManager.hpp"

namespace kocmoc
{
	namespace util
	{

		bool file_exists(const string &filename)
		{
			std::ifstream ifile(filename.c_str());
			return ifile;
		}

		string read_file(const string &filename)
		{
			std::ifstream ifile(filename.c_str());

			return string(std::istreambuf_iterator<char>(ifile),
				std::istreambuf_iterator<char>());
		}

		//void get_errors(void)
		//{
		//	GLenum error = glGetError();
		//
		//	if (error != GL_NO_ERROR) {
		//		switch (error) {
		//		case GL_INVALID_ENUM:
		//			cerr << "GL: enum argument out of range." << endl;
		//			break;
		//		case GL_INVALID_VALUE:
		//			cerr << "GL: Numeric argument out of range." << endl;
		//			break;
		//		case GL_INVALID_OPERATION:
		//			cerr << "GL: Operation illegal in current state." << endl;
		//			break;
		//		case GL_INVALID_FRAMEBUFFER_OPERATION:
		//			cerr << "GL: Framebuffer object is not complete." << endl;
		//			break;
		//		case GL_OUT_OF_MEMORY:
		//			cerr << "GL: Not enough memory left to execute command." << endl;
		//			break;
		//		default:
		//			cerr << "GL: Unknown error." << endl;
		//		}
		//	}
		//}

		void safeDeleteArray(void* p)
		{
			if (p != NULL)
			{
				delete[] p;
				p = NULL;
			}
		}

		namespace generator
		{
			PolyMesh* generateStars()
			{
				// generate starts, lots of stars
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

					mat4 transform = glm::translate((rand() * domain)/RAND_MAX - (domain / 2),
						(rand() * domain)/RAND_MAX - (domain / 2),
						(rand() * domain)/RAND_MAX - (domain / 2)) * scale * rotation;

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
					
					// 1 - 2 - 3
					firstIndices[i*4+1] = i*12+3;
					vertexIndices[i*12+3] = i*4 +1;
					vertexIndices[i*12+4] = i*4 +2;
					vertexIndices[i*12+5] = i*4 +3;

					// 3 - 0 - 1
					firstIndices[i*4+2] = i*12+6;
					vertexIndices[i*12+6] = i*4 +3;
					vertexIndices[i*12+7] = i*4 +0;
					vertexIndices[i*12+8] = i*4 +1;

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
				Shader *shader = ShaderManager::getInstance().load("base.vert", "base.frag");
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