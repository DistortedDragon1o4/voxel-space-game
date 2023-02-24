#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <cerrno>
#include <vector>
#include <math.h>
#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_glfw.h"
#include "../include/imgui/imgui_impl_opengl3.h"
#include "../include/glm/glm.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assert.h>
#include <chrono>
#include <thread>
#include <future>

#include "chunkDataContainer.h"
#include "stb/stb_image.h"

#include "../include/shaderCompiler.h"
#include "../include/texture.h"
#include "../include/camera.h"
#include "../include/chunkList.h"
#include "../include/HUD.h"

#define PI 4 * atan(1)

int main() {

	const unsigned int height = 900;
	const unsigned int width = 1440;

	//Window
	glfwInit();

	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Voxel", NULL, NULL);
	if (!window) {
		std::cout << "Window creation failed successfully\n";
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	//ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	float size_pixels = 20.0f;
	ImFont* programFont = io.Fonts->AddFontFromFileTTF("assets/fonts/PixelOperator8.ttf", size_pixels);

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
	colors[ImGuiCol_Border]                 = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
	colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
	colors[ImGuiCol_FrameBg]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_TitleBg]                = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_CheckMark]              = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_SliderGrab]             = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_Button]                 = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ButtonHovered]          = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_ButtonActive]           = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
	colors[ImGuiCol_HeaderActive]           = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
	colors[ImGuiCol_Separator]              = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_ResizeGrip]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
	colors[ImGuiCol_TabUnfocused]           = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	//colors[ImGuiCol_DockingPreview]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	//colors[ImGuiCol_DockingEmptyBg]         = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderLight]       = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_DragDropTarget]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding                     = ImVec2(8.00f, 8.00f);
	style.FramePadding                      = ImVec2(5.00f, 2.00f);
	style.CellPadding                       = ImVec2(6.00f, 6.00f);
	style.ItemSpacing                       = ImVec2(6.00f, 6.00f);
	style.ItemInnerSpacing                  = ImVec2(6.00f, 6.00f);
	style.TouchExtraPadding                 = ImVec2(0.00f, 0.00f);
	style.IndentSpacing                     = 25;
	style.ScrollbarSize                     = 15;
	style.GrabMinSize                       = 10;
	style.WindowBorderSize                  = 1;
	style.ChildBorderSize                   = 1;
	style.PopupBorderSize                   = 1;
	style.FrameBorderSize                   = 1;
	style.TabBorderSize                     = 1;
	style.WindowRounding                    = 7;
	style.ChildRounding                     = 4;
	style.FrameRounding                     = 3;
	style.PopupRounding                     = 4;
	style.ScrollbarRounding                 = 9;
	style.GrabRounding                      = 3;
	style.LogSliderDeadzone                 = 4;
	style.TabRounding                       = 4;

	//Shaders
	const std::string vertexCode =
	#include "vertex.glsl"
	;
	const std::string fragmentCode =
	#include "fragment.glsl"
	;

	Shader shaderProgram(vertexCode.c_str(), fragmentCode.c_str());

	TextureArray tex(GL_TEXTURE0, "blocks/", 1, NUM_BLOCKS);
	tex.TexUnit(shaderProgram, "array", 0);

	const std::string vertexCodeHighlight =
	#include "vertexHighlight.glsl"
	;
	const std::string fragmentCodeHighlight =
	#include "fragmentHighlight.glsl"
	;

	Shader shaderProgramHighlight(vertexCodeHighlight.c_str(), fragmentCodeHighlight.c_str());

	//Shaders
	const std::string vertexCodeHUD =
	#include "vertexHUD.glsl"
	;
	const std::string fragmentCodeHUD =
	#include "fragmentHUD.glsl"
	;

	Shader shaderProgramHUD(vertexCodeHUD.c_str(), fragmentCodeHUD.c_str());

	GUIItem box;
	box.windowHeight = height;
	box.windowWidth = width;
	box.height = 4;
	box.width = 30;
	box.position = glm::ivec2(0, 0);
	box.color = glm::vec3(0.83f, 0.93f, 0.78f);
	box.genBox();

	GUIItem box2;
	box2.windowHeight = height;
	box2.windowWidth = width;
	box2.height = 30;
	box2.width = 4;
	box2.position = glm::ivec2(0, 0);
	box2.color = glm::vec3(1.0f, 0.23f, 0.48f);
	box2.genBox();

	HUD boxDraw;
	boxDraw.list.push_back(box);
	boxDraw.list.push_back(box2);
	boxDraw.generateMesh();

	std::vector <short> vertices {0, 1, 2};
	std::vector <uint> ebo {0, 1, 2};

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1;
	EBO EBO1;

	VBO1.Gen(vertices);
	EBO1.Gen(ebo);

	VAO1.LinkAttrib(VBO1, 0, 1, GL_SHORT, 1 * sizeof(short), (void*)0);

	VAO1.Unbind();

	//Uniforms
	int locChunkIDx = glGetUniformLocation(shaderProgram.ID, "chunkIDx");
	int locChunkIDy = glGetUniformLocation(shaderProgram.ID, "chunkIDy");
	int locChunkIDz = glGetUniformLocation(shaderProgram.ID, "chunkIDz");

	int locCamPos = glGetUniformLocation(shaderProgram.ID, "camPos");
	int locCamDir = glGetUniformLocation(shaderProgram.ID, "camDir");

	int locSunDir = glGetUniformLocation(shaderProgram.ID, "sunDir");

	int locPos = glGetUniformLocation(shaderProgramHighlight.ID, "Pos");

	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	glEnable(GL_FOG);
	//glEnable(GL_BLEND);

	Camera camera(width, height, glm::dvec3(0.0, 0.0, 0.0));

	ChunkList chunkLister;

	chunkLister.blockInit();

	chunkLister.createHighlightVAO();

	//Test test;

	chunkLister.camPosX = camera.Position.x;
	chunkLister.camPosY = camera.Position.y;
	chunkLister.camPosZ = camera.Position.z;

	//chunkLister.assignChunkID();

	//std::this_thread::sleep_for(std::chrono::seconds(5));

	std::thread calcLoaded(&ChunkList::calculateLoadedChunks, &chunkLister);

	std::thread assigner(&ChunkList::assignChunkID, &chunkLister);

	std::thread generator(&ChunkList::generateChunks, &chunkLister);

	std::thread builder0(&ChunkList::organiseChunks, &chunkLister, 0);
	std::thread builder1(&ChunkList::organiseChunks, &chunkLister, 1);
	std::thread builder2(&ChunkList::organiseChunks, &chunkLister, 2);
	std::thread builder3(&ChunkList::organiseChunks, &chunkLister, 3);
	std::thread builder4(&ChunkList::organiseChunks, &chunkLister, 4);
	std::thread builder5(&ChunkList::organiseChunks, &chunkLister, 5);
	std::thread builder6(&ChunkList::organiseChunks, &chunkLister, 6);
	std::thread builder7(&ChunkList::organiseChunks, &chunkLister, 7);

	int count = 0;
	
	glm::vec3 sunDir = glm::vec3(1.0, 0.0, 0.0);
	float increment = 0.003;
	unsigned long counter = 1;

	while (!glfwWindowShouldClose(window)) {


		glViewport(0, 0, width, height);

		glEnable(GL_CULL_FACE);
		camera.inputs(window);

		glEnable(GL_DEPTH_TEST);

		glClearColor(0.2f, 0.5f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		double crntTime = glfwGetTime();
		int fps = 1 / (crntTime - prevTime);
		//std::cout << fps << "\n";
		prevTime = crntTime;
		
		sunDir.x = cos(counter * increment);
		sunDir.y = sin(counter * increment);
		counter++;
		

		chunkLister.camPosX = camera.Position.x;
		chunkLister.camPosY = camera.Position.y;
		chunkLister.camPosZ = camera.Position.z;

		chunkLister.camDir = camera.Orientation;

		if (count % 10 == 0) {
			chunkLister.lookingAtBlock();
			count = 0;
		}

		count++;

		//chunkLister.chunkManager();
		//chunkLister.assignChunkID();
		chunkLister.putInVAOs();

		shaderProgram.Activate();
		camera.matrix(90.0, 0.001, 512.0, shaderProgram, "cameraMatrix");
		tex.Bind();

		glUniform3fv(locCamPos, 1, &glm::vec3(camera.Position)[0]);
		glUniform3fv(locCamDir, 1, &glm::vec3(camera.Orientation)[0]);
		glUniform3fv(locSunDir, 1, &glm::vec3(sunDir)[0]);

		for (int i = 0; i < (RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE); i++) {
			if (chunkLister.chunkWorldContainer[i].renderlck == 0 && chunkLister.chunkWorldContainer[i].unCompiledChunk == 0 && chunkLister.chunkWorldContainer[i].EBOsize != 0) {
				chunkLister.chunkWorldContainer[i].array.Bind();
				glUniform1i(locChunkIDx, chunkLister.chunkWorldContainer[i].chunkID[0]);
				glUniform1i(locChunkIDy, chunkLister.chunkWorldContainer[i].chunkID[1]);
				glUniform1i(locChunkIDz, chunkLister.chunkWorldContainer[i].chunkID[2]);
				glDrawElements(GL_TRIANGLES, chunkLister.chunkWorldContainer[i].EBOsize, GL_UNSIGNED_INT, 0);
				chunkLister.chunkWorldContainer[i].array.Unbind();
			}
		}

		glDisable(GL_CULL_FACE);

		shaderProgramHighlight.Activate();
		
		camera.matrix(90.0, 0.001, 512.0, shaderProgramHighlight, "cameraMatrix");
		glUniform3fv(locPos, 1, &glm::vec3(chunkLister.blockPos)[0]);

		chunkLister.highlightVAO.Bind();
		glDrawElements(GL_LINE_STRIP, chunkLister.EBOsize, GL_UNSIGNED_INT, 0);
		chunkLister.highlightVAO.Unbind();

		glDisable(GL_DEPTH_TEST);

		shaderProgramHUD.Activate();
		boxDraw.boxVAO.Bind();
		glDrawElements(GL_TRIANGLES, boxDraw.EBOsize, GL_UNSIGNED_INT, 0);
		boxDraw.boxVAO.Unbind();

		ImGui::PushFont(programFont);

		ImGui::ShowDemoWindow();

		ImGui::Begin("I live for testing!!");
		ImGui::Text("Hello World!");
		ImGui::End();

		ImGui::PopFont();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	chunkLister.run = 0;

	calcLoaded.join();
	assigner.join();
	generator.join();
	builder0.join();
	builder1.join();
	builder2.join();
	builder3.join();
	builder4.join();
	builder5.join();
	builder6.join();
	builder7.join();

	//Deleting things
	shaderProgram.Delete();
	tex.Delete();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	
	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
