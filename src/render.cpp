#include "render.hpp"
#include <iostream>

#include <SDL3/SDL.h>
#include "gl.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include "common.hpp"

SDL_Event event;
SDL_Window* window;
SDL_GLContext gl_context;
SDL_AudioStream* stream;
GLuint gameTexture;
ImGuiIO io;
bool show_demo_window = true;

namespace render {
	
	void init() {
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		window = SDL_CreateWindow("Puma", 500, 500, 
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		gl_context = SDL_GL_CreateContext(window);
		gladLoadGL(SDL_GL_GetProcAddress);
		
		
		ImGui::CreateContext();
		SDL_GL_MakeCurrent(window, gl_context);
		SDL_GL_SetSwapInterval(1); 
		io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable multi-viewport support
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable docking if desired
		io.IniFilename = nullptr;
		
		ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
		ImGui_ImplOpenGL3_Init("#version 130");
		
		glGenTextures(1, &gameTexture);
		glBindTexture(GL_TEXTURE_2D, gameTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight,0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
	}
	
	bool frame(uint32_t* frameBuffer) {
		bool result = true;
		SDL_PollEvent(&event);
		ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            result = false;
        }
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, screenWidth, screenHeight, GL_BGRA, GL_UNSIGNED_BYTE, frameBuffer);
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(w, h));
		ImGui::Begin("Puma", nullptr, 
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_MenuBar
			//ImGuiWindowFlags_NoBackground
		);
		//ImGui::Text("Hello");
		ImGui::Image(gameTexture, ImVec2(screenWidth, screenHeight));
        //ImGui::ShowDemoWindow(&show_demo_window);
		//ImGui::Text("Bye");
		ImGui::End();
		
		ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y - 20));
ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 20));

ImGui::Begin("Status Bar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings);

ImGui::Text("Status Bar");

ImGui::End();
		
		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
		SDL_Delay(10);
		return result;
	}
	
	void deinit() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();

		SDL_GL_DestroyContext(gl_context);
		SDL_DestroyWindow(window);	
		SDL_Quit();
	}
	
}