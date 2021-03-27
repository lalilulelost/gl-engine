#pragma once

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GL/GL.h>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Mesh.hpp"
#include "Helper.hpp"

#include "Camera.hpp"


#include "Window.hpp"
#include "Input/InputSystem.hpp"


class App
{
	std::unique_ptr<ShaderProgram> shaderProgram;
	std::unique_ptr<Mesh> triangleMesh;
	std::unique_ptr<Mesh> cubeMesh;

	Input input;

public:
	App()
		: windowObject{ windowWidth, windowHeight, "App" }
	{
		initGL();
		input.setup(windowObject, camera);

		shaderProgram = std::make_unique<ShaderProgram>(ShaderProgram::ShaderTargetFilenamePairs{
				{GL_VERTEX_SHADER, "vertex_shader.vert"}, 
				{GL_FRAGMENT_SHADER, "fragment_shader.frag"} 
		});


		printGLError("After shader setup");

		triangleMesh = std::make_unique<Mesh>(
			std::vector<Vertex>{
				Vertex{ {0.5f, 0.5f, 0.0f}, {0.f, 0.f}, {1.f, 0.f, 0.f} },
				Vertex{ {0.5f, -0.5f, 0.0f}, {0.f, 0.f}, {0.f, 1.f, 0.f} },
				Vertex{ {-0.5f, 0.5f, 0.0f}, {0.f, 0.f}, {0.f, 0.f, 1.f} },
			},
			std::vector<unsigned int>{ 0, 1, 2 }
		);

		cubeMesh = std::make_unique<Mesh>(
			std::vector{
				// front
				Vertex{{-.5f, -.5f, .5f}, {0.f, 0.f},  {1.f, 0.f, 0.f}},
				Vertex{{.5f, -.5f, .5f}, {0.f, 0.f},  {0.f, 1.f, 0.f}},
				Vertex{{.5f, .5f, .5f}, {0.f, 0.f}, {0.f, 0.f, 1.f}},
				Vertex{{-.5f, .5f, .5f}, {0.f, 0.f}, {1.f, 1.f, 1.f}},
				// back 
				Vertex{{-.5f, -.5f, -.5f}, {0.f, 0.f}, {1.f, 0.f, 0.f}},
				Vertex{{.5f, -.5f, -.5f}, {0.f, 0.f}, {0.f, 1.f, 0.f}},
				Vertex{{.5f, .5f, -.5f}, {0.f, 0.f}, {0.f, 0.f, 1.f}},
				Vertex{{-.5f, .5f, -.5f}, {0.f, 0.f}, {1.f, 1.f, 1.f}}
			},
			// indices for index buffer
			std::vector<unsigned int>{
				// front
				0, 1, 2,
				2, 3, 0,
				// right
				1, 5, 6,
				6, 2, 1,
				// back
				7, 6, 5,
				5, 4, 7,
				// left
				4, 0, 3,
				3, 7, 4,
				// botttom
				4, 5, 1,
				1, 0, 4,
				// top
				3, 2, 6,
				6, 7, 3
			}
		);


		loop();
	}


	void initGL()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw std::exception("Failed to initialize GLAD");
		}

		glEnable(GL_DEPTH_TEST);

		glClearColor(0.f, 0, 0.f, 1.f);
		glViewport(0, 0, windowWidth, windowHeight);
	}

	void loop()
	{
		lastFrameTime = 0.0;

		while (!windowObject.shouldClose())
		{
			deltaTime = glfwGetTime() - lastFrameTime;
			std::cout << "FPS: " << 1 / deltaTime << std::endl;
			std::cout << "Total run time: " << glfwGetTime() << std::endl;
			
			input.processInput(deltaTime);

			render();

			lastFrameTime = glfwGetTime();
		}
	}


	void render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera.getMatrix();

		glm::mat4 projection = glm::perspective<float>(glm::radians(60.f), (float)windowWidth / (float)windowHeight, 1.f, 100.f);
		glm::mat4 view_projection =  projection * view;

		shaderProgram->setMatrix4x4(view_projection, "MVP");

		triangleMesh->draw(*shaderProgram);
		cubeMesh->draw(*shaderProgram);

		glfwSwapBuffers(windowObject.getAPIWindowPtr());
	}


	static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}


	const int windowWidth = 960;
	const int windowHeight = 540;


	double deltaTime;
	double lastFrameTime;
	glm::vec2 lastMousePosition;
	glm::vec2 mouseDelta;
	Window windowObject;
	Camera camera{ glm::vec3{0, 0, 3}, glm::vec3{0, 0, -1} };
};
