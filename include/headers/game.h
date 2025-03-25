#pragma once

#include <SDL2/SDL.h>
#include "glad.h"
#include "shader.h"

enum class GameState {PLAY, EXIT};

class Game {
    public:
        Game();
        ~Game();
        void run();

        static SDL_Renderer *_renderer;
        static SDL_Event     _event;
    private:
        void init(const char *title, int x, int y, int w, int h, Uint32 flag);
        void gameLoop();
        GameState _gameState;
        
        void handleEvents();
        void render(shader myShader);
        void update(shader myShader, float deltaTime);

        //FPS things
        int FPS;
        int _frameDelay;

        SDL_Window        *_window;
        SDL_GLContext     _context;
        int           _screenWidth;
        int           _screenHight;


        //OpenGL - data
        GLfloat verticies[72];
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        GLuint texture;
};


class Camera {
    public: 
        Camera();
        ~Camera();
    private:

};
