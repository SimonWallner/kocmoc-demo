#include "LineGizmo.hpp"

#include <renderer/Shader.hpp>
#include <renderer/ShaderManager.hpp>
#include <camera/Camera.hpp>

using namespace kocmoc::scene;

using kocmoc::camera::Camera;
using kocmoc::renderer::ShaderManager;
using kocmoc::renderer::Shader;

using glm::mat4;

LineGizmo::LineGizmo(float* vertexPositions, float* vertexColors, uint length,
				uint* indices, uint _indexCount)
	: indexCount(_indexCount)
{
	shader = ShaderManager::getInstance().load("vertexColor.vert", "vertexColor.frag");
	shader->addSemantic(Shader::VertexAttributeSemantic(symbolize("position"),
		VERTEX_ATTR_NAME_POSITION, 0));
	shader->addSemantic(Shader::VertexAttributeSemantic(symbolize("color"),
		VERTEX_ATTR_NAME_COLOR, 1));
	
	shader->upload();

	glGenVertexArrays(1, &vaoHandle);
	vboHandles = new GLuint[3];
	glGenBuffers(3, vboHandles);

	glBindVertexArray(vaoHandle);
	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
	glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), vertexPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
	glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), vertexColors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandles[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

LineGizmo::~LineGizmo()
{
	glDeleteBuffers(3, vboHandles);
	glDeleteVertexArrays(1, &vaoHandle);

	delete[] vboHandles;
}

void LineGizmo::draw(glm::mat4 leafTransform, Camera *camera)
{
	shader->bind();
	{
		GLint location;

		if ((location = shader->get_uniform_location("projectionMatrix")) >= 0)
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));

		if ((location = shader->get_uniform_location("viewMatrix")) >= 0)
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

		if ((location = shader->get_uniform_location("modelMatrix")) >= 0)
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(leafTransform));
	
		glBindVertexArray(vaoHandle);
		glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}
	shader->unbind();
}