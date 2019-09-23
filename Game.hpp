#ifndef Game_hpp
#define Game_hpp

#include<SDL2/SDL.h>
#include<iostream>
#include<SDL2/SDL_ttf.h>

const int SCREEN_SIDE = 600;

class Game{
    public:
        //constructor
        Game();
        //deconstructor
        ~Game();

        void init(const char* title, int xpos, int ypos, int width, int height);
        void handleEvents();
        void update();
        void render();
        void clean();
        void finish();
        int showMenu(std::string path);
        void initAttributes(int dif);
        void resetAttributes(int dif);
        //int showMenu(std::string path);
        bool running(){
            return isRunning;
        };
        bool paused, win, lose;
        SDL_Window* getWindow(){
            return window;
        }
        SDL_Renderer* getRenderer(){
            return g_renderer;
        }
        static int mapsize,mineNum,tilesize, moveNum, mineLeftNum, level;
        static SDL_Renderer* g_renderer;
        static SDL_Event event;
        static TTF_Font* g_font;
    private:
        bool isRunning;
        SDL_Window* window;

};

#endif // Game_hpp
