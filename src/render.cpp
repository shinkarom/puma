#include "render.hpp"

#include <SDL3/SDL.h>
#include "gl.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

SDL_Event event;
SDL_Window* window;
SDL_GLContext gl_context;
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
		io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		
		ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
		ImGui_ImplOpenGL3_Init("#version 130");
	}
	
	bool frame() {
		bool result = true;
		SDL_PollEvent(&event);
		ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            result = false;
        }
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
		
        ImGui::ShowDemoWindow(&show_demo_window);
		
		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
		//SDL_Delay(100);
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