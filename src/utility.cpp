#include "utility.hpp"

#include <fstream>
#include "Shader.hpp"
#include "ImageLoader.hpp"
#include "PropertiesFileParser.hpp"

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
				int starCount;
				float domain, size;
				util::PropertiesFileParser::GetInstance().getProperty("starsCount", &starCount);
				util::PropertiesFileParser::GetInstance().getProperty("starsDomain", &domain);
				util::PropertiesFileParser::GetInstance().getProperty("starsSize", &size);
				

				PolyMesh *stars = new PolyMesh(starCount * 4, starCount * 4 * 4, starCount * 4);

				double *positions = new double[stars->getVertexCount() * 3];
				unsigned int *vertexIndices = new unsigned int[stars->getVertexIndexCount()];
				unsigned int *firstIndices = new unsigned int[stars->getPrimitiveCount()];
					
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

				stars->setFirstIndexArray(firstIndices);
				stars->setVertexIndexArray(vertexIndices);
				stars->setVertexPositions(positions);

				// add shader to poly
				Shader *shader = new Shader("base.vert", "base.frag");
				stars->setShader(shader);

				// add texture
				GLuint tex = ImageLoader::getInstance().loadImage("color.png");
				stars->setTexture(tex);

				return stars;
			}

			PolyMesh* generateGizmo()
			{
				double vertexPositions[] = {0.0f, 0.0f, 0.0f,
											1.0f, 0.0f, 0.0f,
											0.0f, 1.0f, 0.0f,
											0.0f, 0.0f, 1.0f};
				unsigned int indices[] = {0, 1, 0, 1, 0, 3};
				
				unsigned int fia[] = {0, 2, 4, 6};

				PolyMesh *mesh = new PolyMesh(3, 6, 4);
				mesh->setFirstIndexArray(fia);
				mesh->setVertexIndexArray(indices);
				mesh->setVertexPositions(vertexPositions);

				// add shader to poly
				Shader *shader = new Shader("vertexColor.vert", "vertexColor.frag");
				mesh->setShader(shader);

				// add texture
				GLuint tex = ImageLoader::getInstance().loadImage("color.png");
				mesh->setTexture(tex);

				return mesh;
			}
		}	
	}
}