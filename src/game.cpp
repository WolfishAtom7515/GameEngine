#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "game.h"
#include "shader.h"
#include "glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

//gameObject* player;
// System_Mng  game_system;
// Entity     *new_player = game_system.addEnity();
// Entity     *sec_player = game_system.addEnity();


SDL_Event     Game::_event;
SDL_Renderer *Game::_renderer;


//camera vects
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);


void processMouseMotion(SDL_Event& event);

Game::Game() {
    _window      = nullptr;
    _renderer    = nullptr;
    _screenWidth = 1920;
    _screenHight = 1080;

    FPS = 60;
    _frameDelay = 1000 / FPS;
    _gameState  = GameState::PLAY;
    
}

Game::~Game() {
    std::cout << "The Exit" << '\n'; 
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void Game::run() {
    init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    gameLoop();
}

void Game::init(const char *title, int x, int y, int w, int h, Uint32 flag) {
    
    _window = SDL_CreateWindow(title , x, y, w, h, flag);
    if(!_window) exit(1);
    std::cout << "Window has been created successfully!\n";

    _renderer = SDL_CreateRenderer(_window, -1, 0);
    if(!_renderer) exit(1);
    std::cout << "Render has been created successfully!\n";

    _context = SDL_GL_CreateContext(_window);
    if(!_context) exit(1);
    std::cout << "Context has been created successfully!\n";

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    
    glViewport(0, 0, _screenWidth, _screenHight);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 

    int fullScreenWidth = 1920;
    int fullScreenHeight = 1080;
    SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    // verticies
    GLfloat tempVertices[] = {
    // positions           // colors           // texture coords
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // Bottom-left-back
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom-right-back
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Top-right-back
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // Top-left-back

    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom-left-front
     0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom-right-front
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top-right-front
    -0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 1.0f  // Top-left-front
    };

    memcpy(verticies, tempVertices, sizeof(tempVertices));

    unsigned int indices[] = {
    // Back face
    0, 1, 2,
    2, 3, 0,
    // Front face
    4, 5, 6,
    6, 7, 4,
    // Left face
    4, 0, 3,
    3, 7, 4,
    // Right face
    1, 5, 6,
    6, 2, 1,
    // Top face
    3, 2, 6,
    6, 7, 3,
    // Bottom face
    4, 5, 1,
    1, 0, 4
    };

    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO 
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    // EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0 );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3* sizeof(float)) );
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);  


    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("../src/textures/Pix3.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    SDL_GL_SetSwapInterval(1);


    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)w/(float)h, 0.1f, 100.0f);
}

void Game::gameLoop() {
    int    t = 0;
    Uint32 frameStart;
    int    frameTime;
    float  deltaTime = 0.0f;
    Uint32 lastFrameTime = 0;

    shader myShader("../src/shaders/shader.vs","../src/shaders/shader.fs");

    //write code here if u want


    while (_gameState != GameState::EXIT) {
        frameStart = SDL_GetTicks();

        frameTime = SDL_GetTicks() - frameStart;
        deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0f; // Time in seconds
        lastFrameTime = SDL_GetTicks();        
        
        handleEvents();
        update(myShader, deltaTime);

        if (_gameState != GameState::EXIT && frameTime < _frameDelay) { 
            render(myShader);  // Pass deltaTime to render
            SDL_Delay(_frameDelay - frameTime);

            // Use to see the program working
            //std::cout << t++ << '\n';
        }
    }   
}

void Game::render(shader myShader) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    myShader.use();

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(_window);
}

void Game::update(shader myShader, float deltaTime) {

    const float cameraSens = 5.0f * deltaTime;
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    if (keystates[SDL_SCANCODE_W]) {
        cameraPos += cameraSens * cameraFront;
    }
    if (keystates[SDL_SCANCODE_S]) {
        cameraPos -= cameraSens * cameraFront;
    }
    if (keystates[SDL_SCANCODE_A]) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSens;
    }
    if (keystates[SDL_SCANCODE_D]) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSens;
    }


    // matrix manipulation
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)SDL_GetTicks() / 2000, glm::vec3(1.0f, 1.0f, 1.0f)); 

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

    GLuint modelLoc = glGetUniformLocation(myShader.getId(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    GLuint viewLoc = glGetUniformLocation(myShader.getId(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLuint projectionLoc = glGetUniformLocation(myShader.getId(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Game::handleEvents() {
    // user intput
    const float cameraSens = 0.05f;

    while (SDL_PollEvent(&_event)) {
        switch (_event.type) {
        case SDL_QUIT:
            _gameState = GameState::EXIT;
            break;

        case SDL_WINDOWEVENT:
            switch (_event.window.event) {

            case SDL_WINDOWEVENT_SIZE_CHANGED:
                SDL_GetWindowSize(_window, &_screenWidth, &_screenHight);
                glViewport(0, 0, _screenWidth, _screenHight);
                std::cout << "Window resized: " << _screenWidth << "x" << _screenHight << '\n';
                break;

            }
            break;
        case SDL_KEYDOWN:
            if (_event.key.keysym.sym == SDLK_h) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                std::cout << "Switch!" << '\n';
            }
            if (_event.key.keysym.sym == SDLK_j) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                std::cout << "Switch back!" << '\n';
            }
            break;
        case SDL_MOUSEMOTION:
            processMouseMotion(_event); 
            break;
        }

    }
}

float yaw = -90.0f;
float pitch = 0.0f;  
float lastX = 960.0f;
float lastY = 540.0f;
bool firstMouse = true;

void processMouseMotion(SDL_Event& event)
{

    if (event.type == SDL_MOUSEMOTION)
    {
        float xoffset = static_cast<float>(event.motion.xrel);
        float yoffset = -static_cast<float>(event.motion.yrel);

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;


        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;  
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
        
        std::cout << "yaw : " << yaw << "& z : " << cos(glm::radians(pitch)) << " camera up: "<< cameraUp.y <<'\n';
    }
}