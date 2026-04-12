#include <SDL3/SDL.h>
#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

class Window {
public:
    bool open;
public:
    Window() = default;
    ~Window() = default;

public:
    struct Sizes {
        float WindowWidth;
        float windowHeight;
        float ContentWidth;
        float ContentHeight;
    };
};