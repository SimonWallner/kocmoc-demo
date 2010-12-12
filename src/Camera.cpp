#include "Camera.hpp"

void Camera::setupGizmo(void)
{
	gizmoShader = new Shader("vertexColor.vert", "vertexColor.frag");
	
	gizmo_vbos = new GLuint[2];
	glGenVertexArrays(1, &gizmo_vao);
	glGenBuffers(2, gizmo_vbos);

	GLfloat vertexPositions[] = {0.0f, 0.0f, 0.0f, 1.0f,
								1.0f, 0.0f, 0.0f, 1.0f,
								0.0f, 0.0f, 0.0f, 1.0f,
								0.0f, 1.0f, 0.0f, 1.0f,
								0.0f, 0.0f, 0.0f, 1.0f,
								0.0f, 0.0f, 1.0f, 1.0f};
	GLfloat vertexColors[] = {1.0f, 0.0f, 0.0f,
							1.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f,
							0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 1.0f,
							0.0f, 0.0f, 1.0f};

	glBindVertexArray(gizmo_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, gizmo_vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, 6 * 4 *sizeof(float), vertexPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, gizmo_vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), vertexColors, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_ATTR_INDEX_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_COLOR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Camera::drawGizmo()
{
	gizmoShader->bind();
	{
		GLint projectionMatrix_location = gizmoShader->get_uniform_location("projectionMatrix");		glUniformMatrix4fv(projectionMatrix_location, 1, GL_FALSE, glm::value_ptr(getProjectionMatrix()));

		mat4 untranslated = getUntraslatedViewMatrix();
		mat4 viewMatrix = glm::inverse(glm::translate(untranslated, vec3(0.8, 0.8, 1)));

		GLint viewMatrix_location = gizmoShader->get_uniform_location("viewMatrix");		glUniformMatrix4fv(viewMatrix_location, 1, GL_FALSE, glm::value_ptr(mat4(1.0f)));		GLint modelMatrix_location = gizmoShader->get_uniform_location("modelMatrix");		glUniformMatrix4fv(modelMatrix_location, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glBindVertexArray(gizmo_vao);
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_LINES, 0, 6);
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(0);
	}
	gizmoShader->unbind();
}