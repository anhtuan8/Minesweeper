#include"Game.hpp"

Game *game = nullptr;

using namespace std;

int main(int argc, const char* argv[]){

    const int FPS = 60;
    const int frameDelay = 1000/FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();

    int n = game->showMenu("background.png");

    if(n == -1){
        game->clean();
        return 0;
    }

    game->init("Minesweeper",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_SIDE+350,SCREEN_SIDE);

    game->initAttributes(n);

    while(game->running()){
        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }

        while(game->paused){
            game->handleEvents();
        }
    }
    game->clean();
    return 0;
}
