#include "util.hpp"
#include "Property.hpp"

#include <loader/ImageLoader.hpp>
#include <renderer/ShaderManager.hpp>
#include <renderer/RenderMesh.hpp>
#include <scene/PolyMesh.hpp>
#include <scene/LineGizmo.hpp>

#include <fstream>
#include <string>


using kocmoc::scene::PolyMesh;
using kocmoc::scene::LineGizmo;
using kocmoc::renderer::Shader;
using kocmoc::renderer::ShaderManager;
using kocmoc::renderer::RenderMesh;
using kocmoc::loader::ImageLoader;

using glm::vec4;
using glm::mat4;
using glm::vec3;

using std::string;
using std::vector;

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
		void tokenize(const string& str, vector<string >& tokens, const string& delimiter)
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

			RenderMesh* generateStars()
			{
				// generate starts, lots of stars
				// only in a sphere
				int starCount = Property("starsCount");
				float domain = Property("starsDomain");
				float size = Property("starsSize");

				
				unsigned int primitiveCount = starCount * 4; // 3 tris per tetraeder
				unsigned int vertexIndexCount = primitiveCount * 3; // 3 vertices per triangle
				unsigned int vertexCount = starCount * 4; // 4 vertices per tetraeder

				double *positions = new double[vertexCount * 3];
				unsigned int *vertexIndices = new unsigned int[vertexIndexCount];
				unsigned int *firstIndices = new unsigned int[primitiveCount + 1];
					
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

				PolyMesh::VertexAttribute positionAttribute(3, vertexCount*3, positions, vertexIndices, true);
				PolyMesh* stars = new PolyMesh(primitiveCount,vertexIndexCount, firstIndices, positionAttribute); 

			
				// add shader to poly
				Shader *shader = ShaderManager::getInstance().load("vertexColor.vert", "stars.frag");
				shader->addSemantic(Shader::VertexAttributeSemantic(symbolize("position"),
					VERTEX_ATTR_NAME_POSITION, 0));

				return new RenderMesh(stars, shader);
			}

			LineGizmo* generateOriginGizmo()
			{
				float *vertexPositions = new float[30];
				float *vertexColors = new float[30];

				vertexPositions[0] = 0.0f;
				vertexPositions[1] = 0.0f;
				vertexPositions[2] = 0.0f;
				vertexColors[0] = 1.0f;
				vertexColors[1] = 0.0f;
				vertexColors[2] = 0.0f;

				vertexPositions[3] = 1.0f;
				vertexPositions[4] = 0.0f;
				vertexPositions[5] = 0.0f;
				vertexColors[3] = 1.0f;
				vertexColors[4] = 0.0f;
				vertexColors[5] = 0.0f;

				vertexPositions[6] = 0.0f;
				vertexPositions[7] = 1.0f;
				vertexPositions[8] = 0.0f;
				vertexColors[6] = 0.0f;
				vertexColors[7] = 1.0f;
				vertexColors[8] = 0.0f;

				vertexPositions[9] = 0.0f;
				vertexPositions[10] = 0.0f;
				vertexPositions[11] = 1.0f;
				vertexColors[9] = 0.0f;
				vertexColors[10] = 0.0f;
				vertexColors[11] = 1.0f;


				vertexPositions[12] = 0.0f;
				vertexPositions[13] = 0.0f;
				vertexPositions[14] = 0.0f;
				vertexColors[12] = 0.5f;
				vertexColors[13] = 0.5f;
				vertexColors[14] = 0.5f;

				vertexPositions[15] = -1.0f;
				vertexPositions[16] = 0.0f;
				vertexPositions[17] = 0.0f;
				vertexColors[15] = 0.5f;
				vertexColors[16] = 0.5f;
				vertexColors[17] = 0.5f;

				vertexPositions[18] = 0.0f;
				vertexPositions[19] = -1.0f;
				vertexPositions[20] = 0.0f;
				vertexColors[18] = 0.5f;
				vertexColors[19] = 0.5f;
				vertexColors[20] = 0.5f;

				vertexPositions[21] = 0.0f;
				vertexPositions[22] = 0.0f;
				vertexPositions[23] = -1.0f;
				vertexColors[21] = 0.5f;
				vertexColors[22] = 0.5f;
				vertexColors[23] = 0.5f;


				vertexPositions[24] = 0.0f;
				vertexPositions[25] = 0.0f;
				vertexPositions[26] = 0.0f;
				vertexColors[24] = 0.0f;
				vertexColors[25] = 1.0f;
				vertexColors[26] = 0.0f;

				vertexPositions[27] = 0.0f;
				vertexPositions[28] = 0.0f;
				vertexPositions[29] = 0.0f;
				vertexColors[27] = 0.0f;
				vertexColors[28] = 0.0f;
				vertexColors[29] = 1.0f;

				unsigned int *indices =  new unsigned int[12];
				indices[0] = 0;
				indices[1] = 1;
				indices[2] = 8;
				indices[3] = 2;
				indices[4] = 9;
				indices[5] = 3;
				indices[6] = 4;
				indices[7] = 5;
				indices[8] = 4;
				indices[9] = 6;
				indices[10] = 4;
				indices[11] = 7;
				
				return  new LineGizmo(vertexPositions, vertexColors, 30, indices, 12);
			}

			LineGizmo* generateUnitCube()
			{
				float *vertexPositions = new float[24];
				float *vertexColors = new float[24];

				vertexPositions[0] = 0.5f;
				vertexPositions[1] = 0.5f;
				vertexPositions[2] = 0.5f;
				
				vertexPositions[3] = -0.5f;
				vertexPositions[4] = 0.5f;
				vertexPositions[5] = 0.5f;
				
				vertexPositions[6] = -0.5f;
				vertexPositions[7] = -0.5f;
				vertexPositions[8] = 0.5f;
				
				vertexPositions[9] = 0.5f;
				vertexPositions[10] = -0.5f;
				vertexPositions[11] = 0.5f;
				

				vertexPositions[12] = 0.5f;
				vertexPositions[13] = 0.5f;
				vertexPositions[14] = -0.5f;
				
				vertexPositions[15] = -0.5f;
				vertexPositions[16] = 0.5f;
				vertexPositions[17] = -0.5f;
				
				vertexPositions[18] = -0.5f;
				vertexPositions[19] = -0.5f;
				vertexPositions[20] = -0.5f;
				
				vertexPositions[21] = 0.5f;
				vertexPositions[22] = -0.5f;
				vertexPositions[23] = -0.5f;
				
				vertexColors[0] = 1.0f;
				vertexColors[1] = 1.0f;
				vertexColors[2] = 1.0f;

				vertexColors[3] = 1.0f;
				vertexColors[4] = 1.0f;
				vertexColors[5] = 1.0f;

				vertexColors[6] = 1.0f;
				vertexColors[7] = 1.0f;
				vertexColors[8] = 1.0f;

				vertexColors[9] = 1.0f;
				vertexColors[10] = 1.0f;
				vertexColors[11] = 1.0f;

				vertexColors[12] = 1.0f;
				vertexColors[13] = 1.0f;
				vertexColors[14] = 1.0f;

				vertexColors[15] = 1.0f;
				vertexColors[16] = 1.0f;
				vertexColors[17] = 1.0f;

				vertexColors[18] = 1.0f;
				vertexColors[19] = 1.0f;
				vertexColors[20] = 1.0f;

				vertexColors[21] = 1.0f;
				vertexColors[22] = 1.0f;
				vertexColors[23] = 1.0f;

				uint *indices =  new uint[24];
				indices[0] = 0;
				indices[1] = 1;

				indices[2] = 1;
				indices[3] = 2;

				indices[4] = 2;
				indices[5] = 3;

				indices[6] = 3;
				indices[7] = 0;


				indices[8] = 4;
				indices[9] = 5;

				indices[10] = 5;
				indices[11] = 6;

				indices[12] = 6;
				indices[13] = 7;

				indices[14] = 7;
				indices[15] = 4;


				indices[16] = 0;
				indices[17] = 4;

				indices[18] = 1;
				indices[19] = 5;

				indices[20] = 2;
				indices[21] = 6;

				indices[22] = 3;
				indices[23] = 7;
				
				return new LineGizmo(vertexPositions, vertexColors, 24, indices, 24);
			}

			RenderMesh* generateTriangle()
			{
				uint* indices = new uint[3];
				uint* fia = new uint[2];

				double* positions = new double[9];
				positions[0] = 2.0;
				positions[1] = 1.0;
				positions[2] = -1.0;

				positions[3] = 1.0;
				positions[4] = 0.0;
				positions[5] = 0.0;

				positions[6] = 2.0;
				positions[7] = -1.0;
				positions[8] = 0.0;

				indices[0] = 0;
				indices[1] = 1;
				indices[2] = 2;

				fia[0] = 0;
				fia[1] = 3;


				double* colors = new double[9];
				colors[0] = 1.0;
				colors[1] = 0.0;
				colors[2] = 0.0;

				colors[3] = 0.0;
				colors[4] = 1.0;
				colors[5] = 0.0;

				colors[6] = 0.0;
				colors[7] = 0.0;
				colors[8] = 1.0;

				double* uvs = new double[6];
				uvs[0] = 0.0;
				uvs[1] = 1.0;

				uvs[2] = 1.0;
				uvs[3] = 0.0;

				uvs[4] = 1.0;
				uvs[5] = 1.0;


				PolyMesh::VertexAttribute vertexPosition(3, 9, positions, indices, true);
				PolyMesh* mesh = new PolyMesh(1, 3, fia, vertexPosition);

				PolyMesh::VertexAttribute color(3, 9, colors, indices, true);
				mesh->addVertexAttribute(symbolize("color"), color);

				PolyMesh::VertexAttribute uv(3, 9, uvs, indices, true);
				mesh->addVertexAttribute(symbolize("uv"), uv);

				Shader *shader = ShaderManager::getInstance().load("textured.vert", "textured.frag");

				shader->addSemantic(Shader::VertexAttributeSemantic(symbolize("position"), VERTEX_ATTR_NAME_POSITION, 0));
				shader->addSemantic(Shader::VertexAttributeSemantic(symbolize("color"), VERTEX_ATTR_NAME_COLOR, 1));
				shader->addSemantic(Shader::VertexAttributeSemantic(symbolize("uv"), "inUV", 2));

				PolyMesh::Texture diffuse("uv_color.png");
				mesh->addTexture(symbolize("diffuse"), diffuse);

				return new RenderMesh(mesh, shader);
			}
		}

		namespace geometry
		{
			bool inside(double d, vec3 n, vec3 point)
			{
				double distance = ::glm::dot(point, n) - d;
				return (distance < 0);
			}


			double intersect(double d, vec3 n, vec3 p1, vec3 p2)
			{
				double d1 = ::glm::dot(p1, n) -d;
				double d2 = ::glm::dot(p2, n) -d;

				return d2 / (d1 + d2);
			}

			/**
			 * linear combination 
			 */
			vec3 mix(float r, vec3 v1, vec3 v2)
			{
				return (v1 * r) + (v2 * (1-r));
			}
		}
	}
}