#include <SDL2/SDL.h>
#include "glad.h"
#include "game.h"
#include <iostream>

#undef main

void checkLib() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) exit(1);
    std::cout << "SDL2 has been initialized successfully!\n";
    
    SDL_Window* _window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    if(!_window) exit(1);
    std::cout << "Window has been created successfully!\n";

    SDL_Renderer *_renderer;
    _renderer = SDL_CreateRenderer(_window, -1, 0);
    if(!_renderer) exit(1);
    std::cout << "Render has been created successfully!\n";

    SDL_GLContext _context = SDL_GL_CreateContext(_window);
    if(!_context) exit(1);
    std::cout << "Context has been created successfully!\n";

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) exit(1);
    std::cout << "GLAD has been initialized successfully!\n";

    SDL_DestroyRenderer(_renderer);
    _renderer = nullptr;

    SDL_GL_DeleteContext(_context);
    _context = nullptr;
    
    SDL_DestroyWindow(_window);
    _window = nullptr;
}

int main(int argc, char* argv[]) {

    checkLib();

    Game game;
    
    game.run();

    return 0;
}
