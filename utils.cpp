#include "utils.hpp"
#include "Game.hpp"
#include <iostream>

void myUtils::initMap(int mineNum, int mapsize, Tile tiles[][25], SDL_Renderer* renderer,int tilesize){
    for(int i = 0 ; i < mapsize ; i++){
        for(int j = 0 ; j < mapsize ; j++){
            tiles[i][j].setData(0);
        }
    }
    bool placed[25][25] = {false};
    int cnt = 0;
    srand(time(NULL));
    while(cnt < mineNum){
        int n = rand()% (mapsize*mapsize) ;
        int y = n / mapsize;
        int x = n % mapsize;

        if(!placed[x][y]){
            placed[x][y] = true;
            tiles[x][y].init(-1,x,y,tilesize);
            cnt++;
        }
    }
    for(int i = 0 ; i < mapsize ; i++){
        for(int j = 0 ; j < mapsize ; j++){
            if(!placed[i][j]){
                int data = countAdjacentMine(i,j,tiles,mapsize);
                tiles[i][j].init(data,i,j,tilesize);
            }
            tiles[i][j].setTexture("brick.png",renderer);
        }
    }
}

int myUtils::countAdjacentMine(int x,int y,Tile tiles[][25],int mapsize){
    int counter = 0;
    if(isValid(x-1,y,mapsize)){
        if(tiles[x-1][y].isMine())
            counter++;
    }
    if(isValid(x-1,y-1,mapsize)){
        if(tiles[x-1][y-1].isMine())
            counter++;
    }
    if(isValid(x-1,y+1,mapsize)){
        if(tiles[x-1][y+1].isMine())
            counter++;
    }
    if(isValid(x,y-1,mapsize)){
        if(tiles[x][y-1].isMine())
            counter++;
    }
    if(isValid(x,y+1,mapsize)){
        if(tiles[x][y+1].isMine())
            counter++;
    }
    if(isValid(x+1,y,mapsize)){
        if(tiles[x+1][y].isMine())
            counter++;
    }
    if(isValid(x+1,y-1,mapsize)){
        if(tiles[x+1][y-1].isMine())
            counter++;
    }
    if(isValid(x+1,y+1,mapsize)){
        if(tiles[x+1][y+1].isMine())
            counter++;
    }

    return counter;
}

bool myUtils::isValid(int xTile, int yTile,int mapsize){
    if( (xTile >=0 && xTile < mapsize) && (yTile >=0 && yTile < mapsize) )
        return true;
    return false;
}

void myUtils::clickTile(int x,int y, Tile tiles[][25],int mapsize,Game* game){
    if(isValid(x,y,mapsize)){
        if(!tiles[x][y].isOpen()){
            tiles[x][y].openTile(game);
            tiles[x][y].open = true;

            if(tiles[x][y].getData() == 0){

                if(isValid(x-1,y,mapsize)){
                    clickTile(x-1,y,tiles,mapsize,game);
                }
                if(isValid(x-1,y-1,mapsize)){
                    clickTile(x-1,y-1,tiles,mapsize,game);
                }
                if(isValid(x-1,y+1,mapsize)){
                    clickTile(x-1,y+1,tiles,mapsize,game);
                }
                if(isValid(x,y-1,mapsize)){
                    clickTile(x,y-1,tiles,mapsize,game);
                }
                if(isValid(x,y+1,mapsize)){
                    clickTile(x,y+1,tiles,mapsize,game);
                }
                if(isValid(x+1,y,mapsize)){
                    clickTile(x+1,y,tiles,mapsize,game);
                }
                if(isValid(x+1,y-1,mapsize)){
                    clickTile(x+1,y-1,tiles,mapsize,game);
                }
                if(isValid(x+1,y+1,mapsize)){
                    clickTile(x+1,y+1,tiles,mapsize,game);
                }
            }
        }

    }
}

bool myUtils::checkFocus(int x, int y, SDL_Rect rect){
    if(x > rect.x && x < (rect.x + rect.w) && y > rect.y && y< (rect.y+rect.h) ){
        return true;
    }
    return false;

}

















