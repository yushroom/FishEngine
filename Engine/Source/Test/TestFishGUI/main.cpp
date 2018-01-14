#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>

#include <FishGUI/FishGUI.hpp>
#include <GLFW/glfw3.h>

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

int main(void)
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
	window = glfwCreateWindow(800, 600, "Fish GUI", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	FishGUI::Init(window);

#if 0
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

	
	std::string email = "mail@example.com";
	bool enabled = true;
	
	auto f1 = [&email, &enabled]() {
		FishGUI::CheckBox("CheckBox", &enabled);
		FishGUI::Label("Login");
		
		FishGUI::InputText("Email", email);
		float value = 0.4f;
		FishGUI::Slider("Diameter", &value, 0.0f, 1.0f);
		if (FishGUI::Button("button 2")) {
			printf("button 2 clicked\n");
		}
	};
	
	auto f2 = [&email]() {
		FishGUI::InputText("Email", email);
		FishGUI::Label("AaBbCcDdEeFfGg");
		if (FishGUI::Button("button 1")) {
			printf("button 1 clicked\n");
		}
		if (FishGUI::Button("button 2")) {
			printf("button 2 clicked\n");
		}
	};
	
	auto top    = FishGUI::CreateTab("Top", FishGUI::TabPosition::Top, 60);
	top->m_resizable = false;
	auto right  = FishGUI::CreateTab("Right", FishGUI::TabPosition::Right, 280);
	auto bottom = FishGUI::CreateTab("Bottom", FishGUI::TabPosition::Bottom, 200);
	auto left   = FishGUI::CreateTab("Left", FishGUI::TabPosition::Left, 160);
	
	right->m_renderFunction = f1;
	bottom->m_renderFunction = f1;
	left->m_renderFunction = f2;
	
	while (!glfwWindowShouldClose(window))
	{
		FishGUI::Begin();
		FishGUI::End();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

