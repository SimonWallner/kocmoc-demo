#include "LineGizmo.hpp"

#include <renderer/Shader.hpp>
#include <renderer/ShaderManager.hpp>
#include <camera/Camera.hpp>

using namespace kocmoc::scene;

using kocmoc::camera::Camera;
using kocmoc::renderer::ShaderManager;

using glm::mat4;

LineGizmo::LineGizmo(float* vertexPositions, float* vertexColors, uint length,
				uint* indices, uint _indexCount)
	: indexCount(_indexCount)
{
	shader = ShaderManager::getInstance().load("gizmo.vert", "gizmo.frag");
	
	glGenVertexArrays(1, &vaoHandle);
	GLuint* vboHandles = new GLuint[3];
	glGenBuffers(3, vboHandles);

	glBindVertexArray(vaoHandle);
	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
	glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), vertexPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
	glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), vertexColors, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_ATTR_INDEX_COLOR, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_COLOR);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandles[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void LineGizmo::draw(glm::mat4 leafTransform, Camera *camera)
{
	shader->bind();
	{
		GLint location;

		if ((location = shader->get_uniform_location("projectionMatrix")) >= 0)
		if ((location = shader->get_uniform_location("viewMatrix")) >= 0)
		if ((location = shader->get_uniform_location("modelMatrix")) >= 0)
	
		glBindVertexArray(vaoHandle);
		glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}
	shader->unbind();
}