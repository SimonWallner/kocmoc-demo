#include "ImageOverlay.hpp"

#include <renderer/Shader.hpp>
#include <renderer/ShaderManager.hpp>
#include <loader/ImageLoader.hpp>
#include <camera/Camera.hpp>

using namespace kocmoc;
using namespace scene;
using camera::Camera;
using renderer::Shader;


ImageOverlay::ImageOverlay(std::string fileName, int _width, int _height, Camera* overlayCam)
	: width(_width)
	, height(_height)
	, opacity(1.0f)
{
	textureHandle = loader::ImageLoader::getInstance().loadImage(fileName, true);
	shader = renderer::ShaderManager::getInstance().load("overlay.vert", "overlay.frag");
	shader->addSemantic(Shader::VertexAttributeSemantic(symbolize("position"),
					VERTEX_ATTR_NAME_POSITION, 0));
	shader->addSemantic(Shader::VertexAttributeSemantic(symbolize("uv0"),
					VERTEX_ATTR_NAME_UV0, 1));

	shader->upload();
	setupShader(overlayCam);
	createQuad();
}

void ImageOverlay::setupShader(Camera* overlayCamera)
{
	shader->bind();
	{
		GLint location;

		if ((location = shader->get_uniform_location("projectionMatrix")) >= 0)
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(overlayCamera->getProjectionMatrix()));

		if ((location = shader->get_uniform_location("sImage")) >= 0)
			glUniform1i(location, 0);
	}

	shader->unbind();
}

void ImageOverlay::draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	shader->bind();

	GLint location;
	if ((location = shader->get_uniform_location("opacity")) >= 0)
		glUniform1f(location, opacity);

	
	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
	
	shader->unbind();
}

void ImageOverlay::createQuad()
{
	GLfloat topX = - width/2;
	GLfloat topY = height/2;
	GLfloat bottomX = width/2;
	GLfloat bottomY = -height/2;

	GLfloat quadVertices[] = {bottomX,  bottomY, 0.0f,
		bottomX, topY, 0.0f,
		topX, topY, 0.0f,
		topX, bottomY, 0.0f};

	GLfloat quadTexCoord[] = {	1, 0,
								1, 1,
								0, 1,
								0, 0};

	vboHandles = new GLuint[2];
	glGenBuffers(2, vboHandles);
	glGenVertexArrays(1, &vaoHandle);

	glBindVertexArray(vaoHandle);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 *sizeof(float), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), quadTexCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ImageOverlay::setOpacity(float _opacity)
{
	opacity = _opacity;//	pow(_opacity, 1.0f/2.2f);
}