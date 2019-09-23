#include"Game.hpp"
#include<SDL2/SDL_image.h>
#include"Tile.hpp"
#include<string>
#include<iostream>
#include"text.hpp"

using namespace std;

Tile::Tile(){
}

Tile::~Tile(){
}

void Tile::init(int data,int xTile, int yTile,int tilesize){
    free();
    this->data = data;
    setRect(xTile*tilesize,yTile*tilesize,tilesize);
    open = false;
    state = 0;
    this->xTile = xTile;
    this->yTile = yTile;
}

void Tile::openTile(Game* game){
    SDL_Renderer* renderer = game->g_renderer;
    if(this->data == -1){
        game->lose = true;
        setTexture("mine.png",renderer);
        game->finish();
    }
    else{
        setTexture(to_string(data) + ".png" , renderer);
    }
    this->open = true;
    Game::moveNum--;
}

void Tile::flag(Game* game){
    setTexture("flag.png",game->getRenderer());
    game->mineLeftNum--;
    this->flaged = true;

}

void Tile::deflag(Game* game){
    setTexture("brick.png",game->getRenderer());
    game->mineLeftNum++;
    this->flaged = false;
}
void Tile::renderTile(Game* game){
    SDL_Renderer* renderer = game->getRenderer();
    SDL_RenderCopy(renderer,tex,NULL,Tile::getRect());
}

void Tile::free(){
    if( tex != NULL )
	{
		SDL_DestroyTexture( tex );
		tex = NULL;
		desR = {0,0,0,0};
	}
}

