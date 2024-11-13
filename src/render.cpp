#include "render.hpp"
#include <iostream>

#include <SDL3/SDL.h>
#include "gl.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include "common.hpp"
#include "input.hpp"

constexpr int msPerFrame = 1000 / framesPerSecond;
uint64_t lastTime = 0, nextTime = lastTime+msPerFrame;

SDL_Event event;
SDL_Window* window;
SDL_GLContext gl_context;
SDL_AudioStream* stream;
GLuint gameTexture;
ImGuiIO io;
SDL_AudioDeviceID audioID;
bool isFullscreen = false;

constexpr SDL_AudioSpec spec = {.format = SDL_AUDIO_S16LE, .channels=2, .freq=audioSampleRate};

// Set height of the status bar at the bottom
constexpr float statusBarHeight = 20.0f;

void initAudio() {
	audioID = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
	stream = SDL_CreateAudioStream(&spec, nullptr);
	SDL_BindAudioStream(audioID, stream);
	
}

void deinitAudio() {
	SDL_CloseAudioDevice(audioID);
}

namespace render {
	
	void init() {
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		window = SDL_CreateWindow("Puma", 500, 500, 
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_SetWindowMinimumSize(window, screenWidth*3,screenHeight*3);
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight,0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
		
		initAudio();	
	}
	
	void updateInput() {
		auto kbState = SDL_GetKeyboardState(nullptr);
		for (int i = 0; i< numInputs; i++) {
			input::previouslyPressedInputs[i] = input::pressedInputs[i];
			input::pressedInputs[i] = kbState[input::inputMapping[i]];
			input::justPressedInputs[i] = (! input::previouslyPressedInputs[i]) && input::pressedInputs[i];
			input::justReleasedInputs[i] = input::previouslyPressedInputs[i] & (! input::pressedInputs[i]);
		}
	}
	
	bool frame(uint32_t* frameBuffer, int16_t* audioBuffer) {
		bool result = true;
		while(SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT) {
				result = false;
			} else if (event.type == SDL_EVENT_KEY_UP) {
                // Toggle fullscreen on F12 or Alt+Enter
                if (event.key.scancode == SDL_SCANCODE_F12 ||
                    (event.key.scancode == SDL_SCANCODE_RETURN && (event.key.mod & SDL_KMOD_ALT))) {
                    // Toggle between fullscreen and windowed
                    isFullscreen = !isFullscreen;
                    if (isFullscreen) {
                        SDL_SetWindowFullscreen(window, true);
                    } else {
                        SDL_SetWindowFullscreen(window, false);  // Set back to windowed
                    }
                }
				if(isFullscreen && event.key.scancode == SDL_SCANCODE_ESCAPE) {
					SDL_SetWindowFullscreen(window, false); 
				}
			}
		}
		if(!io.WantCaptureKeyboard) {
			updateInput();
		}
		
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, screenWidth, screenHeight, GL_BGRA, GL_UNSIGNED_BYTE, frameBuffer);
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(w, h));
		auto flags = ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoScrollWithMouse;
			//ImGuiWindowFlags_NoBackground
		if(!isFullscreen) {
			flags |= ImGuiWindowFlags_MenuBar;
			ImGui::GetStyle().WindowPadding = ImVec2(8, 8);
		} else {
			ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
		}
		ImGui::Begin("Puma", nullptr, flags);
		
		if (!isFullscreen &&ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				//if (ImGui::MenuItem("Open")) {
					// Open file action
				//}
				if (ImGui::MenuItem("Exit")) {
					result = false;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		
		// Get the size of available region
		ImVec2 availableSize = ImGui::GetContentRegionAvail();
		
		auto imageHeight = isFullscreen ? availableSize.y : availableSize.y - statusBarHeight;
		ImGui::BeginChild("ImageRegion", ImVec2(availableSize.x, imageHeight), false);
		ImVec2 imageSize = ImGui::GetContentRegionAvail();

		// Determine the maximum size that maintains the aspect ratio
		float scale = std::min(imageSize.x * 1.0f / screenWidth, imageSize.y * 1.0f / screenHeight);
		ImVec2 scaledSize = ImVec2(screenWidth * 1.0f * scale, screenHeight * 1.0f * scale);

		// Center the image in the available space if there’s extra room
		ImVec2 padding = ImVec2((imageSize.x - scaledSize.x) / 2.0f, (imageSize.y - scaledSize.y) / 2.0f);
		ImVec2 finalPos = ImVec2(ImGui::GetCursorPos().x+padding.x, ImGui::GetCursorPos().y+padding.y);
		ImGui::SetCursorPos(finalPos);

		// Render the image at the scaled size
		ImGui::Image(gameTexture, scaledSize);  
		ImGui::EndChild();
		
		if(!isFullscreen) {
			ImGui::SetCursorPosY(ImGui::GetWindowHeight() - statusBarHeight);
			ImGui::BeginChild("StatusBar", ImVec2(availableSize.x, statusBarHeight), false);
			ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);  // Example status bar text
			ImGui::EndChild();
		}
		
		
		ImGui::End();
		
		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
		
		SDL_PutAudioStreamData(stream, audioBuffer, samplesPerFrame*2*2);
		
		auto t = SDL_GetTicks();
		if(t<nextTime) {
			SDL_Delay(nextTime-t);
		}
		nextTime+= msPerFrame;
		return result;
	}
	
	
	
	void deinit() {
		deinitAudio();
		
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();

		SDL_GL_DestroyContext(gl_context);
		SDL_DestroyWindow(window);	
		SDL_Quit();
	}
	
}