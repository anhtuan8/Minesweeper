/*This file includes utility functions and game constants*/

#ifndef utils_hpp
#define utils_hpp

#include <iostream>
#include <SDL2/SDL.h>
#include"Game.hpp"
#include"Tile.hpp"

namespace myUtils{
    void initMap(int mineNum, int mapsize, Tile tiles[][25], SDL_Renderer* renderer,int tilesize);
    int countAdjacentMine(int x, int y, Tile tiles[][25], int mapsize);
    bool isValid(int xTile, int yTile,int mapsize);
    void clickTile(int x,int y, Tile tiles[][25],int mapsize,Game* game);
    bool checkFocus(int x,int y,SDL_Rect rect);
}


#endif // utils_hpp
